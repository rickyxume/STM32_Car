#include "motor.h"


//ռ�ձȷ�������ֵԽ���ٶ�Խ��
////�����
//#define LB(v) TIM_SetCompare1(TIM4 , v)
////��ǰ��
//#define LF(v) TIM_SetCompare2(TIM4 , v)
////�Һ���
//#define RB(v) TIM_SetCompare3(TIM4 , v)
////��ǰ��
//#define RF(v) TIM_SetCompare4(TIM4 , v)

//��ʦ�ķ�����speedԽ��Խ�죬��Χ�Ҹ�Ϊ��0-999
#define LB(Speed) TIM4->CCR1 = Speed;
#define LF(Speed) TIM4->CCR2 = Speed;
#define RB(Speed) TIM4->CCR3 = Speed;
#define RF(Speed) TIM4->CCR4 = Speed;
	
//ǰ��
void CarGo(int v)
{
	LB(0);//��ֵԽ���ٶ�Խ��
	LF(v);
	RB(0);
	RF(v);
	
}
//ֹͣ
void CarStop(void)
{
	LB(0);
	LF(0);
	RB(0);
	RF(0);
}
//����
void CarBack(int v)
{
	LB(v);
	LF(0);
	RB(v);
	RF(0);
}

//��ת
void CarLeft(int v)
{
	LB(v);
	LF(0);
	RB(0);
	RF(v);
}

//��ת
void CarRight(int v)
{
	LB(0);
	LF(v);
	RB(v);
	RF(0);
}

//��ǰ��ת
void CarForwardLeft(int v)
{
	LB(v+500);
	LF(999);
	RB(v);
	RF(999);
}

//��ǰ��ת
void CarForwardRight(int v)
{
	LB(v);
	LF(999);
	RB(999);
	RF(v+500);
}


//�����󵹳�
void CarBackLeft(int v)
{
	LB(v-500);
	LF(0);
	RB(v);
	RF(0);
}

//�����ҵ���
void CarBackRight(int v)
{
	LB(v);
	LF(0);
	RB(v-500);
	RF(0);
}

//���������¹���
void CarTest()
{
	while(1){
	Ultrasonic_Avoidance();
	};
}

void TIM4_PWM_Init(void)  //TIM4��pwm���ú���Ӧ����������
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
  
  //�˿ڸ���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;   //��ʼ��Ҫ�õ�A6/A7��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  
  
  //PWMͨ��1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC1Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWMͨ��2
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC2Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWMͨ��3
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OC3Init(TIM4 , &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Enable);
  
  //PWMͨ��4
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

	//1.��ʼ��ʱ��  APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
	//2.Ӳ����ʼ��������������Ǹ���������ʼ����
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//PB6 PB7 PB8 PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//3.��ʼ����ϣ��ߵ͵�ƽ�Ϳ���ֱ�Ӹ���
	//�Ȱ�С��ֹͣ
	GPIO_ResetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	//ʹ�� MVEN �����������,����Ҫ������ȻС������
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
