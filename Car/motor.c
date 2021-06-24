#include <stm32f10x.h>
#include "stm32f10x_tim.h"

// ��ǰ��
#define LA1 GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LA0 GPIO_ResetBits(GPIOB,GPIO_Pin_6)
// �����
#define LB1 GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LB0 GPIO_ResetBits(GPIOB,GPIO_Pin_7)
// ��ǰ��
#define RA1 GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define RA0 GPIO_ResetBits(GPIOB,GPIO_Pin_8)
// �Һ���
#define RB1 GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define RB0 GPIO_ResetBits(GPIOB,GPIO_Pin_9)

//ռ�ձȣ���ֵԽ���ٶ�Խ��
//��ǰ��
#define LF(v) TIM_SetCompare2(TIM4 , v)
//�����
#define LB(v) TIM_SetCompare1(TIM4 , v)
//��ǰ��
#define RF(v) TIM_SetCompare4(TIM4 , v)
//�Һ���
#define RB(v) TIM_SetCompare3(TIM4 , v)

int my_TIM_Pulse = 1000;

void CarTest(int v)
{
	LF(v);
	LB(1000);
	RF(v+500);
	RB(1000);
}

//ǰ��
void CarGo(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//ֹͣ
void CarStop(void)
{
	LF(my_TIM_Pulse);
	LB(my_TIM_Pulse);
	RF(my_TIM_Pulse);
	RB(my_TIM_Pulse);
}

// ԭ��ת����
void CarLeftAround(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(v);
	RB(my_TIM_Pulse);
}

// ԭ��ת����
void CarRightAround(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(my_TIM_Pulse);
	RB(v);
}

// ����
void CarBack(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(my_TIM_Pulse);
	RB(v);
}

//���Ż�

//������ת
void CarLeft(int v)
{
	LF(900);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//������ת
void CarRight(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(900);
	RB(my_TIM_Pulse);
}

//��ǰ��ת
void CarForwardLeft(int v)
{
	LF(v+500);
	LB(my_TIM_Pulse);
	RF(v);
	RB(my_TIM_Pulse);
}

//��ǰ��ת
void CarForwardRight(int v)
{
	LF(v);
	LB(my_TIM_Pulse);
	RF(v+500);
	RB(my_TIM_Pulse);
}

//�����󵹳�
void CarBackLeft(int v)
{
	LF(my_TIM_Pulse);
	LB(v+800);
	RF(my_TIM_Pulse);
	RB(v);
}

//�����ҵ���
void CarBackRight(int v)
{
	LF(my_TIM_Pulse);
	LB(v);
	RF(my_TIM_Pulse);
	RB(v+800);
}

void TIM4_PWM_Init(void)
{
	//������ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure; 
	
	// TIM4ʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  //ʹ�� GPIOB ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//����IOģʽ
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� PB6/7/8/9
	
	//��ʼ��TIM4�ļ���ģʽ����Ƶֵ����װ��ֵ��
	TIM_TimeBaseStructure.TIM_Period = my_TIM_Pulse; //������һ�������¼���װ���Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //����TIM4ʱ��Ԥ��Ƶֵ PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseStructure); //���ݲ�����ʼ��TIM4
	
	//��ʼ��TIM4��PWMͨ��1234
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; // �Ƚ����N��ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ԣ�TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = my_TIM_Pulse;//ռ�ձȴ�С
	
	//����ʼ��TIM4_OC1234
	TIM_OC1Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC2Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC3Init(TIM4 , &TIM_OCInitStructure);
	TIM_OC4Init(TIM4 , &TIM_OCInitStructure);
	
	//ʹ��TIM4��OC1234�ϵ�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4 , TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIM4���Զ���װ�ؼĴ���
	
	//TIM_CtrlPWMOutputs(TIM4,ENABLE); //�����ʹ��
	
	//ʹ��TIM4��OC1234�ϵ�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4 , ENABLE); //ʹ��
}

//���õ��������  
void GPIO_init_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//����һ��������ʼ��GPIO�Ľṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOʱ��
	//2����ʼ�������4������PB6 PB7  PB8  PB9    �͵�Դʹ������ PB5
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8 | GPIO_Pin_9;//ѡ��Ҫ�õ�GPIO����
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ 
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;//��������ģʽΪ�������ģʽ
	GPIO_Init(GPIOB,&GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_5); //PB5 = 1��ʹ�ܵ�ع���	
}
