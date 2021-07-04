#include "motor.h"


//占空比方法，数值越大速度越慢
////左后退
//#define LB(v) TIM_SetCompare1(TIM4 , v)
////左前进
//#define LF(v) TIM_SetCompare2(TIM4 , v)
////右后退
//#define RB(v) TIM_SetCompare3(TIM4 , v)
////右前进
//#define RF(v) TIM_SetCompare4(TIM4 , v)

//老师的方法，speed越大越快，范围我改为了0-999
#define LB(Speed) TIM4->CCR1 = Speed;
#define LF(Speed) TIM4->CCR2 = Speed;
#define RB(Speed) TIM4->CCR3 = Speed;
#define RF(Speed) TIM4->CCR4 = Speed;
	
//前进
void CarGo(int v)
{
	LB(0);//数值越大速度越快
	LF(v);
	RB(0);
	RF(v);
	
}
//停止
void CarStop(void)
{
	LB(0);
	LF(0);
	RB(0);
	RF(0);
}
//后退
void CarBack(int v)
{
	LB(v);
	LF(0);
	RB(v);
	RF(0);
}

//左转
void CarLeft(int v)
{
	LB(v);
	LF(0);
	RB(0);
	RF(v);
}

//右转
void CarRight(int v)
{
	LB(0);
	LF(v);
	RB(v);
	RF(0);
}

//向前左转
void CarForwardLeft(int v)
{
	LB(v+500);
	LF(999);
	RB(v);
	RF(999);
}

//向前右转
void CarForwardRight(int v)
{
	LB(v);
	LF(999);
	RB(999);
	RF(v+500);
}


//后向左倒车
void CarBackLeft(int v)
{
	LB(v-500);
	LF(0);
	RB(v);
	RF(0);
}

//后向右倒车
void CarBackRight(int v)
{
	LB(v);
	LF(0);
	RB(v-500);
	RF(0);
}

//用来测试新功能
void CarTest()
{
	while(1){
	Ultrasonic_Avoidance();
	};
}

void TIM4_PWM_Init(void)  //TIM4的pwm设置和相应的引脚设置
{
  GPIO_InitTypeDef GPIO_InitStructure;

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);// | RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
  
  
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseStructure);
  
  //端口复用
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;   //初始化要用的A6/A7口
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  
  
  //PWM通道1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC1Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWM通道2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC2Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWM通道3
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC3Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWM通道4
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC4Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  TIM_Cmd(TIM4 , ENABLE);
}



void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//1.初始化时钟  APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
	//2.硬件初始化《本身操作的那个外设对象初始化》
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//PB6 PB7 PB8 PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//3.初始化完毕，高低电平就可以直接赋予
	//先把小车停止
	GPIO_ResetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	//使能 MVEN 电机驱动供电,必须要给，不然小车不动
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
