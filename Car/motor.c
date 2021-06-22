#include <stm32f10x.h>
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

void CarGo(void)
{
	//时空比，数值越大速度越慢
	//TIM_SetCompare2(TIM3 , 300);
	//TIM_SetCompare4(TIM3 , 300);	
	//1010
	LA1;LB0;RA1;RB0;
}

void CarStop(void)
{
	//TIM_SetCompare2(TIM3 , 900);
	//TIM_SetCompare4(TIM3 , 900);
	//1111
	LA1;LB1;RA1;RB1;
}

void CarLeftAround(void)
{
	//TIM_SetCompare2(TIM3 , 400);
	//TIM_SetCompare4(TIM3 , 400);
	//0110
	LA0;LB1;RA1;RB0;
}

void CarRightAround(void)
{
	//TIM_SetCompare2(TIM3 , 400);
	//TIM_SetCompare4(TIM3 , 400);
	//1001
	LA1;LB0;RA0;RB1;
}


void CarBack(void)
{
	//TIM_SetCompare2(TIM3 , 300);
	//TIM_SetCompare4(TIM3 , 300);
	//0101
	LA0;LB1;RA0;RB1;
}

void CarLeft(void)
{
	//TIM_SetCompare2(TIM3 , 300);
	//TIM_SetCompare4(TIM3 , 900);
	//1010
	LA1;LB0;RA1;RB0;
}

void CarBigLeft(void)
{
	//TIM_SetCompare2(TIM3 , 100);
	//TIM_SetCompare4(TIM3 , 900);
	//1010
	LA1;LB0;RA1;RB0;
}

void CarRight(void)
{
	//TIM_SetCompare2(TIM3 , 900);
	//TIM_SetCompare4(TIM3 , 300);
	//1010
	LA1;LB0;RA1;RB0;
}

void CarBigRight(void)
{
	//TIM_SetCompare2(TIM3 , 900);
	//TIM_SetCompare4(TIM3 , 100);
	//1010
	LA1;LB0;RA1;RB0;
}



//void TIM3_PWM_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure; 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//	
//	
//	TIM_TimeBaseStructure.TIM_Period = 899;
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseStructure);
//	
//	//端口复用
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
//	GPIO_Init(GPIOA, &GPIO_InitStructure);   
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);   
//	

//	//PWM通道2
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_Pulse = 900;
//	TIM_OC2Init(TIM3 , &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM3 , TIM_OCPreload_Enable);
//	

//	//PWM通道4
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_Pulse = 900;
//	TIM_OC4Init(TIM3 , &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM3 , TIM_OCPreload_Enable);
//	
//	TIM_Cmd(TIM3 , ENABLE);
//}

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

