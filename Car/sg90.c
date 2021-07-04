#include "sg90.h"

/////////////////////////�����������--��ʱ�����PWM//////////////////////////////////
/*
	1�����ΪPWM���
	2���ö��Ҫ���ṩ��PWM����Ϊ20ms
	3��������ƽǶ�Ϊ0~180��  ��ʼλ���Լ����Կ���
	4��������20ms pwm�����ڵĸߵ�ƽ�����ɿ���������Ƕ�
	5��0.5ms~2ms�ĸߵ�ƽ���ڶ�Ӧ��ת���Ƕ���0~180��
	6��PWM��������ʹ�����µĶ�ʱ��3--TIM3�����ͨ��4,��Ϊ�������оƬ����ΪPB1  
	   ���ֲᡶSTM32F103_�����ֲᣨ���ģ�.pdf��19ҳ �� ��С��ԭ��ͼ����֪PB1��TIM3_CH4�ĸ��ù���
	7������Ҫ����TIM3  �� TIM3�����ͨ��4  �Լ��õ���GPIOB1����
	8����ʼ����������
		��ʹ��GPIOB   AFIO(�������Ÿ��ù���)  TIM3  ����ʱ��
		�ڳ�ʼ��PB1Ϊ���������������
		�۳�ʼ��TIM3��ʱ����Ԫ��Ԥ��Ƶϵ��Ϊ72---��Ϊ����ʱ��TIM3�ṩ��ʱ��Ϊ72Mhz  72��ƵΪ72Mhz/72 = 1Mhz
						  �Զ���װ��ֵΪ20000---��Ϊ�������ø���TIM3��ʱ��Ϊ1Mhz  20000������պ���20ms
						  ����ģʽΪ�������� ---�����Ļ���ʱ����������ֵ��0��ʼ��һ�������+1
		�ܳ�ʼ��TIM3�����ͨ��4---������Ϊ���PWM1ģʽ����
							  ---���Ը�
							  ---Ĭ�ϵ�CCRΪ500  
		�ݿ�����ʱ��TIM3
*/

//��ɫ�ź��ߡ���
//��ɫ������
//�غ�ɫ����


//��������ڼ��㹫ʽΪ������=��arr+1��*��psc+1��/CLK������CLKΪ��������ʱ��Ƶ�ʣ�
//�ҵ���72MHZ��Ҳ����72000000������������λΪ�룬���Ϊ0.02s��Ҳ����20ms��

//PWM����Ϊ20ms = (7200*200)/72000000=0.02
//����TIM_Period = 199��TIM_Prescaler = 7199
//t = 0.5ms������������-�����ת�� 0 ��
//t = 1.0ms������������-�����ת�� 45��
//t = 1.5ms������������-�����ת�� 90��
//t = 2.0ms������������-�����ת�� 135��
//t = 2.5ms������������-�����ת��180��
//PWMռ�ձ���ָ��һ�������ڣ��źŴ��ڸߵ�ƽ��ʱ��ռ�������ź����ڵİٷֱȣ�
//����PWM����Ϊ20ms�����ԣ��Զ����ת�� 45��Ϊ������ռ�ձȾ�Ӧ��Ϊ1ms/20ms = 5%��
//����TIM_SetCompare1�� TIMx ����Ƚ� 1 �Ĵ���ֵ��Ϊ200-200*5% = 190
void SG90_PWM_init(void)
{
	GPIO_InitTypeDef  bbb;
	TIM_TimeBaseInitTypeDef ccc;
	TIM_OCInitTypeDef ddd;
	
	//1��ʹ��ʱ��  GPIOB1   AFIO  TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//2��GPIO PB1��ʼ��
	bbb.GPIO_Pin  = GPIO_Pin_1;//TX
	bbb.GPIO_Speed  = GPIO_Speed_50MHz;//
	bbb.GPIO_Mode   = GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOB,&bbb);
	
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	//3��������ʱ��TIM3��ʼ������Ƶϵ��  ����ģʽ  �Զ���װ��ֵ  ��Ƶ���ӣ�
	ccc.TIM_Prescaler = 72-1;//72Mhz/72 = 1Mhz  1�������Ӧ1us
	ccc.TIM_CounterMode = TIM_CounterMode_Up;//��������ģʽ
	ccc.TIM_Period      = 20000-1;//ARR 20000  �Զ���װ��ֵ  �պ�����Ϊ20ms
	ccc.TIM_ClockDivision = TIM_CKD_DIV1;//�����Ԥ��Ƶϵ������  ��������Ϊ1��Ƶ������Ƶ
	TIM_TimeBaseInit(TIM3,&ccc);
	
	//4����ʱ��TIM3���ͨ��4�ĳ�ʼ��
	ddd.TIM_OCMode = TIM_OCMode_PWM1;//PWMģʽ1---����ģʽ��  CNT<CCRʱ�����Ч��ƽ   CNT>CCRʱ�����һ�ֵ�ƽ
	ddd.TIM_OCPolarity = TIM_OCPolarity_High;//���Ը�----�ߵ�ƽ��Ч  ��������ģʽ������������ �ұ������
	ddd.TIM_Pulse      = 500;// �Ƚ�ֵ  CCR500*1us = 500us = 0.5ms�ߵ�ƽ  Ĭ�϶��ת��0��λ��
	ddd.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4Init(TIM3,&ddd);//ע��������OC4---��Ӧͨ��4

	//5��������ʱ��3������--��ʼ��������
	TIM_Cmd(TIM3,ENABLE);
}

//void SG90_PWM_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;   
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	
//	/* ����ʱ�� */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

//	/*  ����GPIO��ģʽ��IO�� */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;// PB1
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	//TIM3��ʱ����ʼ��
//	TIM_TimeBaseInitStructure.TIM_Period = 199; //PWM Ƶ��=72000/(199+1)=36Khz//�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);


//	//PWM��ʼ��	  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;

//	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
//	//ע��˴���ʼ��ʱTIM_OC1Init������TIM_OCInit������������Ϊ�̼���İ汾��һ����
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
//	TIM_Cmd(TIM3,ENABLE);//ʹ�ܻ���ʧ��TIMx����
//}

