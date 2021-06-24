#include <stm32f10x.h>
#include "stm32f10x_tim.h"

// 左前轮
#define LA1 GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LA0 GPIO_ResetBits(GPIOB,GPIO_Pin_6)
// 左后轮
#define LB1 GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LB0 GPIO_ResetBits(GPIOB,GPIO_Pin_7)
// 右前轮
#define RA1 GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define RA0 GPIO_ResetBits(GPIOB,GPIO_Pin_8)
// 右后轮
#define RB1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define RB0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)

//占空比，数值越大速度越慢
//左前进
#define LF(v) TIM_SetCompare2(TIM4 , v)
//左后退
#define LB(v) TIM_SetCompare1(TIM4 , v)
//右前进
#define RF(v) TIM_SetCompare4(TIM4 , v)
//右后退
#define RB(v) TIM_SetCompare3(TIM4 , v)

int my_TIM_Pulse = 1000;

void CarTest(int v)
{
	LF(v);
	LB(1000);
	RF(v+500);
	RB(1000);
}

//前进
void CarGo(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//停止
void CarStop(void)
{
	LF(my_TIM_Pulse);
	LB(my_TIM_Pulse);
	RF(my_TIM_Pulse);
	RB(my_TIM_Pulse);
}

// 原地转向左
void CarLeftAround(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(v);
	RB(my_TIM_Pulse);
}

// 原地转向右
void CarRightAround(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(my_TIM_Pulse);
	RB(v);
}

// 后退
void CarBack(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(my_TIM_Pulse);
	RB(v);
}

//待优化

//差速左转
void CarLeft(int v)
{
	LF(900);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//差速右转
void CarRight(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(900);
	RB(my_TIM_Pulse);
}

//向前左转
void CarForwardLeft(int v)
{
	LF(v+500);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//向前右转
void CarForwardRight(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(v+500);
	RB(my_TIM_Pulse);
}

//后向左倒车
void CarBackLeft(int v)
{
	LF(my_TIM_Pulse);
	LB(v+800);
	RF(my_TIM_Pulse);
	RB(v);
}

//后向右倒车
void CarBackRight(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(my_TIM_Pulse);
	RB(v+800);
}

void TIM4_PWM_Init(void)
{
	//变量初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure; 
	
	// TIM4时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //使能 GPIOB 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//配置IO模式
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化 PB6/7/8/9
	
	//初始化TIM4的计数模式、分频值、重装载值等
	TIM_TimeBaseStructure.TIM_Period = my_TIM_Pulse; //设置下一个更新事件后，装入自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置TIM4时钟预分频值 PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseStructure); //根据参数初始化TIM4
	
	//初始化TIM4的PWM通道1234
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式：TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; // 比较输出N不使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性：TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = my_TIM_Pulse;//占空比大小
	
	//数初始化TIM4_OC1234
	TIM_OC1Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC2Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC3Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC4Init(TIM4 , &TIM_OCInitStructure);
	
	//使能TIM4在OC1234上的预装载寄存器
	TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIM4的自动重装载寄存器
	
	//TIM_CtrlPWMOutputs(TIM4,ENABLE); //主输出使能
	
	//使能TIM4在OC1234上的预装载寄存器
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4 , ENABLE); //使能
}

//配置电机的引脚  
void GPIO_init_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//定义一个用来初始化GPIO的结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIO时钟
	//2、初始化电机的4个引脚PB6 PB7  PB8  PB9    和电源使能引脚 PB5
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9;//选择要用的GPIO引脚
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;//设置引脚速度为50MHZ 
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;//设置引脚模式为推挽输出模式
	GPIO_Init(GPIOB,&GPIO_InitStructure);//调用库函数，初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_5); //PB5 = 1，使能电池供电	
}
