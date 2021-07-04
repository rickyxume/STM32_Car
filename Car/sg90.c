#include "sg90.h"

/////////////////////////舵机控制配置--定时器输出PWM//////////////////////////////////
/*
	1、舵机为PWM舵机
	2、该舵机要求提供的PWM周期为20ms
	3、舵机控制角度为0~180°  初始位置自己调试看看
	4、调整其20ms pwm周期内的高电平比例可控制其输出角度
	5、0.5ms~2ms的高电平周期对应的转动角度是0~180°
	6、PWM脉冲的输出使用如下的定时器3--TIM3的输出通道4,因为舵机连接芯片引脚为PB1  
	   查手册《STM32F103_数据手册（中文）.pdf》19页 和 《小车原理图》可知PB1有TIM3_CH4的复用功能
	7、所以要配置TIM3  和 TIM3的输出通道4  以及用到的GPIOB1引脚
	8、初始化步骤如下
		①使能GPIOB   AFIO(用了引脚复用功能)  TIM3  三者时钟
		②初始化PB1为复用推挽输出功能
		③初始化TIM3的时基单元的预分频系数为72---因为给定时器TIM3提供的时钟为72Mhz  72分频为72Mhz/72 = 1Mhz
						  自动重装载值为20000---因为上面设置给到TIM3的时钟为1Mhz  20000个脉冲刚好是20ms
						  计数模式为递增计数 ---这样的话定时器计数器的值从0开始来一个脉冲就+1
		④初始化TIM3的输出通道4---配置其为输出PWM1模式功能
							  ---极性高
							  ---默认的CCR为500  
		⑤开启定时器TIM3
*/

//橙色信号线　　
//红色正极　
//棕褐色负极


//具体的周期计算公式为：周期=（arr+1）*（psc+1）/CLK。其中CLK为计数器的时钟频率，
//我的是72MHZ，也就是72000000。最后计算结果单位为秒，结果为0.02s，也就是20ms。

//PWM周期为20ms = (7200*200)/72000000=0.02
//所以TIM_Period = 199，TIM_Prescaler = 7199
//t = 0.5ms——————-舵机会转动 0 °
//t = 1.0ms——————-舵机会转动 45°
//t = 1.5ms——————-舵机会转动 90°
//t = 2.0ms——————-舵机会转动 135°
//t = 2.5ms——————-舵机会转动180°
//PWM占空比是指在一个周期内，信号处于高电平的时间占据整个信号周期的百分比，
//由于PWM周期为20ms，所以（以舵机会转动 45°为例），占空比就应该为1ms/20ms = 5%，
//所以TIM_SetCompare1的 TIMx 捕获比较 1 寄存器值就为200-200*5% = 190
void SG90_PWM_init(void)
{
	GPIO_InitTypeDef  bbb;
	TIM_TimeBaseInitTypeDef ccc;
	TIM_OCInitTypeDef ddd;
	
	//1、使能时钟  GPIOB1   AFIO  TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//2、GPIO PB1初始化
	bbb.GPIO_Pin  = GPIO_Pin_1;//TX
	bbb.GPIO_Speed  = GPIO_Speed_50MHz;//
	bbb.GPIO_Mode   = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB,&bbb);
	
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	//3、基本定时器TIM3初始化（分频系数  计数模式  自动重装载值  分频因子）
	ccc.TIM_Prescaler = 72-1;//72Mhz/72 = 1Mhz  1个脉冲对应1us
	ccc.TIM_CounterMode = TIM_CounterMode_Up;//递增计数模式
	ccc.TIM_Period      = 20000-1;//ARR 20000  自动重装载值  刚好周期为20ms
	ccc.TIM_ClockDivision = TIM_CKD_DIV1;//上面的预分频系数够了  这里设置为1分频即不分频
	TIM_TimeBaseInit(TIM3,&ccc);
	
	//4、定时器TIM3输出通道4的初始化
	ddd.TIM_OCMode = TIM_OCMode_PWM1;//PWM模式1---递增模式下  CNT<CCR时输出有效电平   CNT>CCR时输出另一种电平
	ddd.TIM_OCPolarity = TIM_OCPolarity_High;//极性高----高电平有效  配合上面的模式，即左边输出高 右边输出低
	ddd.TIM_Pulse      = 500;// 比较值  CCR500*1us = 500us = 0.5ms高电平  默认舵机转到0°位置
	ddd.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3,&ddd);//注意这里是OC4---对应通道4

	//5、开启定时器3计数器--开始工作计数
	TIM_Cmd(TIM3,ENABLE);
}

//void SG90_PWM_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;   
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//	/* 开启时钟 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

//	/*  配置GPIO的模式和IO口 */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;// PB1
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	//TIM3定时器初始化
//	TIM_TimeBaseInitStructure.TIM_Period = 199; //PWM 频率=72000/(199+1)=36Khz//设置自动重装载寄存器周期的值
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;//设置用来作为TIMx时钟频率预分频值
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);


//	//PWM初始化	  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

//	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
//	//注意此处初始化时TIM_OC1Init而不是TIM_OCInit，否则会出错。因为固件库的版本不一样。
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//使能或者失能TIMx在CCR1上的预装载寄存器
//	TIM_Cmd(TIM3,ENABLE);//使能或者失能TIMx外设
//}

