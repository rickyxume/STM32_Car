#include "hcsr04.h"


//记录定时器溢出次数
uint overcount=0;

//设置中断优先级
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

//初始化模块的GPIO以及初始化定时器TIM2
void HC_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//TRIG触发信号
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_11; //LC1 --> PB11
	GPIO_Init(GPIOB, &GPIO_InitStructer);

	//ECOH回响信号
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_12;//LC2 --> PB12
	GPIO_Init(GPIOB, & GPIO_InitStructer);

	//定时器TIM2初始化
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStructer.TIM_Period=999;//1000-1定时周期为1000
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; //72-1分频系数72
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//开启更新中断
	NVIC_Config();
	TIM_Cmd(TIM2,DISABLE);//关闭定时器使能

}



float GetDistance(void)
{
	float length=0,sum=0;
	u16 tim;
	uint i=0;
	//参考网上的测距代码，这里使用了均值滤波操作，测5次取平均值使得测距更稳定准确
	while(i!=5)/*测5次数据计算一次平均值*/
	{
		PBout(11)=1; //封装好函数，输出=1即拉高信号，作为触发信号
		delay_us(20); //高电平信号需要超过10us
		PBout(11)=0;
		/*等待回响信号*/
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET);
		TIM_Cmd(TIM2,ENABLE);//回响信号到来，开启定时器计数

		i+=1; //每收到一次回响信号+1，收到5次就计算均值
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==SET);//回响信号消失
		TIM_Cmd(TIM2,DISABLE);//关闭定时器

		tim=TIM_GetCounter(TIM2);//获取计TIM2数寄存器中的计数值，一边计算回响信号时间

		length=(tim+overcount*1000)/58.0;//通过回响信号计算距离

		sum=length+sum;
		TIM2->CNT=0; //将TIM2计数寄存器的计数值清零
		overcount=0; //中断溢出次数清零
		delay_ms(10);
	}
	length=sum/5;
	return length;//距离作为函数返回值，单位cm
}



void TIM2_IRQHandler(void) //中断，当回响信号很长时，计数值溢出后重复计数，用中断来保存溢出次数
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	 {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志
		overcount++;
	 }
}

//红外避障辅助	
void red_helper()
{
	CarStop();//先停止稳定下来，让小车的红外传感器判断一下左右障碍物情况
	if(R==0&&L==0){CarBack(800);delay_ms(100);DelayCheck(10);} //如果左右都是低电平，即左右都有障碍，那么就后退一下
	if(R==0){CarLeft(999);delay_ms(100);}  //如果右边有障碍物，就向左转一点
	if(L==0){CarRight(999);delay_ms(100);} //如果左边有障碍物，就向右转一点
	if(R&&L){ 														 //如果都是1，即都是高电平，两边没有检测到障碍物，就往前走一点
		CarGo(999);delay_ms(200); //一开始速度快一点，之后再谨慎慢速
		//delay_ms(200);DelayCheck(300);
	}
}

//超声波避障
void Ultrasonic_Avoidance()
{
	float distancF=0;
	float distanceR45=0;
	float distanceR60=0;
	float distanceR90=0;
	float distanceL45=0;
	float distanceL60=0;
	float distanceL90=0;
	//while(1) //去掉while循环，使得蓝牙可以控制小车
	//	{			//如果不去掉会让小车一直处于while循环中，如果只测试超声波功能则可以加上
			red_helper();//红外避障辅助，先用红外判断一下小车附近障碍物情况，并做出反应
			
			//开始超声波测距
			SG90_Front; delay_ms(100); //舵机摆正，加一点延迟防抖
			distancF = GetDistance();  //测前方距离
			
			USART_SendData(USART1,(char)GetDistance());//给蓝牙设备发送数据
	
			delay_ms(100);//给时间测距
			if(distancF>30.00)       //如果前方距离大于30cm 
			{
				CarStop();//先停下来用红外测两边近距离障碍物
				if(R==0&&L==0){CarBack(800);delay_ms(100);DelayCheck(10);} //两边都有障碍则倒车，遇到障碍就停一下
				if(R==0){CarLeft(999);delay_ms(100);}	//如果右边有障碍物，左倒车
				if(L==0){CarRight(999);delay_ms(100);}//如果左边有障碍物，右倒车
				if(R&&L){ CarGo(200);delay_ms(200);}  //如果左右都没检测到障碍就慢速前进
				distancF = GetDistance(); //一直测距直至满足条件（前方距离大于30cm）
			}
			
			if(distancF<30.00)     //如果前方距离小于30厘米  停车测左右距离
			{
					CarStop();    delay_ms(100);
					CarBack(800); delay_ms(10);

					CarStop(); delay_ms(10);SG90_Left_45; delay_ms(300);//舵机左转45度测距
					distanceL45 = GetDistance();//测量结果存下来
				
					//CarStop(); delay_ms(10);SG90_Left_60; delay_ms(150);
					//distanceL60 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Left_90; delay_ms(300);//舵机左转90度测距
					distanceL90 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Right_45; delay_ms(300);//舵机右转45度测距
					distanceR45 = GetDistance();
					
//					CarStop(); delay_ms(10);SG90_Right_60; delay_ms(150);
//					distanceR60 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Right_90; delay_ms(300);//舵机右转90度测距
					distanceR90 = GetDistance();
					SG90_Front; delay_ms(50);  //舵机摆正
					CarBack(800); delay_ms(100);//先后退一点再进行下一步操作
					
//					//左边60比右边60距离短
//					if(distanceL60 < distanceR60)
//					{								
//						do
//							{
//							SG90_Front;delay_ms(100);//舵机摆正
//							distancF = GetDistance();	//重复测前方的距离同时		
//							CarRight(999);delay_ms(50);//右转	
//							}while(distancF<25.00);		//一直转到前方距离大于25cm
//					}else{//左边60比右边60距离长
//						do{
//							SG90_Front;delay_ms(100);//舵机摆正
//							distancF = GetDistance();	//重复测前方的距离同时 
//							CarLeft(999);delay_ms(50);//左转
//							}while(distancF<25.00);		//一直转到前方距离大于25cm
//					}
					
					//左边45比右边45距离短
					if(distanceL45 < distanceR45)
						{								
							do{
								SG90_Front;delay_ms(100);	//舵机摆正
								distancF = GetDistance();	//重复测前方的距离		
								CarRight(999);delay_ms(40);	//右转	
							}while(distancF<35.00);		//一直转到前方距离大于35cm
					}else{//左边45比右边45距离长
							do{
								SG90_Front;delay_ms(100);	//舵机摆正
								distancF = GetDistance();	//重复测前方的距离同时
								CarLeft(999);delay_ms(40);	//左转
								}while(distancF<35.00);		//一直转到前方距离大于35cm
					}
					
					if(distanceL90<10 && distanceR90<10)//如果左边90太窄&&右边90也太窄
					{
							CarBack(200);delay_ms(100);//后退一点
					}
				}
	//}	//while结束

}