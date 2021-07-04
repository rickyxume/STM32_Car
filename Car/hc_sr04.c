#include "hc_sr04.h"
#include "helper.h"

/*2020.11.16
* HC-SR04超声波测距模块
* 基本工作原理：* 给TRIG引脚至少10us的高电平信号触发测距
							 * 模块自动发送8个40KHz的方波，自动检测是否有信号返回
							 * 有信号返回，通过Echo引脚输出一个高电平，高电平的持续时间就是超声波从发射到返回的时间
							 * 测量距离 = （高电平的持续时间 * 声速（340m/s））/ 2
*/


#define HCSR04_PORT			GPIOB
#define HCSR04_CLK			RCC_APB2Periph_GPIOB
#define HCSR04_TRIG_PIN GPIO_Pin_11 //脉冲触发TRIG端口 LC1 --> PB11
#define HCSR04_ECHO_PIN GPIO_Pin_12 //回波接收ECHO端口 LC2 --> PB12

#define SONAR_SOUND_SPEED 340       //<声音的传播速度，单位为m/s



/*记录定时器溢出次数*/
unsigned int overcount=0;

/*设置中断优先级*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructer;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructer.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;

	NVIC_Init(&NVIC_InitStructer);
}

/*初始化模块的GPIO以及初始化定时器TIM2*/
void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd( HCSR04_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*TRIG触发信号*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructer.GPIO_Pin=HCSR04_TRIG_PIN; //LC1 --> PB11
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructer);

	/*ECOH回响信号*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructer.GPIO_Pin=HCSR04_ECHO_PIN;//LC2 --> PB12
	GPIO_Init(HCSR04_PORT, & GPIO_InitStructer);

	/*定时器TIM2初始化*/
	//TIM_DeInit(TIM2);
	//测距时计时用的定时器的配置 TIM3  TIM4 已经用了  TIM2
	TIM_TimeBaseInitStructer.TIM_Period = 65536-1;//ARR 20000  16Bit定时器 自动重装载值  刚好周期为20ms
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; //分频系数72=1HMz 1个脉冲对应1us
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//上面的预分频系数够了  这里设置为1分频即不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启更新中断
	NVIC_Config();
	TIM_Cmd(TIM2,DISABLE);//关闭定时器使能

}

float Senor_Using(void)
{
	float length=0,sum=0;
	u16 tim;
	unsigned int i=0;
	/*测5次数据计算一次平均值*/
	while(i!=5)
	{
		PBout(11)=1; //拉高信号，作为触发信号
		delay_us(20); //高电平信号超过10us
		PBout(11)=0;
//		GPIO_SetBits(HCSR04_PORT, HCSR04_TRIG_PIN);
//		delay_us(10);
//		GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG_PIN);	
		/*等待回响信号*/
		while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO_PIN)==RESET);
		TIM_Cmd(TIM2,ENABLE);//回响信号到来，开启定时器计数

		i+=1; //每收到一次回响信号+1，收到5次就计算均值
		while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO_PIN)==SET);//回响信号消失
		TIM_Cmd(TIM2,DISABLE);//关闭定时器

		tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间

		length=(tim+overcount*1000)/58.0;//通过回响信号计算距离

		sum=length+sum;
		TIM2->CNT=0; //将TIM2计数寄存器的计数值清零
		overcount=0; //中断溢出次数清零
		delay_ms(10);
	}
	length=sum/5;
	return length;//距离作为函数返回值
}



void TIM2_IRQHandler(void) //中断，当回响信号很长时，计数值溢出后重复计数，用中断来保存溢出次数
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	 {
		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志
		 overcount++;
	 }
}

float HCSR04_GetDistance(void)
{
    float time_s = 0;
	  int i;
	  float p[5];
		for(i=0;i<5;i++)
		{
				//触发一个至少10us的高电平，启动超声波
				GPIO_SetBits(HCSR04_PORT, HCSR04_TRIG_PIN);
				delay_us(10);
				GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG_PIN);			
		//  delay_ms(SONAR_MAX_WAIT_TIME);
				//轮询，等待ECHO引脚拉高，拉高瞬间开始计时
				while (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN) == RESET )
					{	}
				//在SONAR_MAX_WAIT_TIME时间内等待是否有回声
				
				if (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN))
				{ //接受到回声
						TIM3->CNT = 0;//将TIM2计数寄存器的计数值清零
						TIM_Cmd(TIM3, ENABLE); 
						// 高电平开始计时
						while (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN) == SET)
						{  }                      //轮询
						TIM_Cmd(TIM3, DISABLE); //ECHO高电平结束 结束计数
						p[i] = TIM_GetCounter(TIM3)*1e-6; //将单位由us转换为s				
				}
				time_s+=p[i];
				delay_ms(10);
			}
    time_s=time_s/5;
    return ( 100*time_s * SONAR_SOUND_SPEED / 2); //返回距离 单位cm
		//计算距离：cnt * 1/10000 * 340 / 2(单位：m) 
}
