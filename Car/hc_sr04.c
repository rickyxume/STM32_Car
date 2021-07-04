#include "hc_sr04.h"
#include "helper.h"

/*2020.11.16
* HC-SR04���������ģ��
* ��������ԭ��* ��TRIG��������10us�ĸߵ�ƽ�źŴ������
							 * ģ���Զ�����8��40KHz�ķ������Զ�����Ƿ����źŷ���
							 * ���źŷ��أ�ͨ��Echo�������һ���ߵ�ƽ���ߵ�ƽ�ĳ���ʱ����ǳ������ӷ��䵽���ص�ʱ��
							 * �������� = ���ߵ�ƽ�ĳ���ʱ�� * ���٣�340m/s����/ 2
*/


#define HCSR04_PORT			GPIOB
#define HCSR04_CLK			RCC_APB2Periph_GPIOB
#define HCSR04_TRIG_PIN GPIO_Pin_11 //���崥��TRIG�˿� LC1 --> PB11
#define HCSR04_ECHO_PIN GPIO_Pin_12 //�ز�����ECHO�˿� LC2 --> PB12

#define SONAR_SOUND_SPEED 340       //<�����Ĵ����ٶȣ���λΪm/s



/*��¼��ʱ���������*/
unsigned int overcount=0;

/*�����ж����ȼ�*/
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

/*��ʼ��ģ���GPIO�Լ���ʼ����ʱ��TIM2*/
void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd( HCSR04_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*TRIG�����ź�*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructer.GPIO_Pin=HCSR04_TRIG_PIN; //LC1 --> PB11
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructer);

	/*ECOH�����ź�*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStructer.GPIO_Pin=HCSR04_ECHO_PIN;//LC2 --> PB12
	GPIO_Init(HCSR04_PORT, & GPIO_InitStructer);

	/*��ʱ��TIM2��ʼ��*/
	//TIM_DeInit(TIM2);
	//���ʱ��ʱ�õĶ�ʱ�������� TIM3  TIM4 �Ѿ�����  TIM2
	TIM_TimeBaseInitStructer.TIM_Period = 65536-1;//ARR 20000  16Bit��ʱ�� �Զ���װ��ֵ  �պ�����Ϊ20ms
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; //��Ƶϵ��72=1HMz 1�������Ӧ1us
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//�����Ԥ��Ƶϵ������  ��������Ϊ1��Ƶ������Ƶ
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//���������ж�
	NVIC_Config();
	TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��ʹ��

}

float Senor_Using(void)
{
	float length=0,sum=0;
	u16 tim;
	unsigned int i=0;
	/*��5�����ݼ���һ��ƽ��ֵ*/
	while(i!=5)
	{
		PBout(11)=1; //�����źţ���Ϊ�����ź�
		delay_us(20); //�ߵ�ƽ�źų���10us
		PBout(11)=0;
//		GPIO_SetBits(HCSR04_PORT, HCSR04_TRIG_PIN);
//		delay_us(10);
//		GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG_PIN);	
		/*�ȴ������ź�*/
		while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO_PIN)==RESET);
		TIM_Cmd(TIM2,ENABLE);//�����źŵ�����������ʱ������

		i+=1; //ÿ�յ�һ�λ����ź�+1���յ�5�ξͼ����ֵ
		while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO_PIN)==SET);//�����ź���ʧ
		TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��

		tim=TIM_GetCounter(TIM2);//��ȡ��TIM2���Ĵ����еļ���ֵ��һ�߼�������ź�ʱ��

		length=(tim+overcount*1000)/58.0;//ͨ�������źż������

		sum=length+sum;
		TIM2->CNT=0; //��TIM2�����Ĵ����ļ���ֵ����
		overcount=0; //�ж������������
		delay_ms(10);
	}
	length=sum/5;
	return length;//������Ϊ��������ֵ
}



void TIM2_IRQHandler(void) //�жϣ��������źźܳ�ʱ������ֵ������ظ����������ж��������������
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	 {
		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����жϱ�־
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
				//����һ������10us�ĸߵ�ƽ������������
				GPIO_SetBits(HCSR04_PORT, HCSR04_TRIG_PIN);
				delay_us(10);
				GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG_PIN);			
		//  delay_ms(SONAR_MAX_WAIT_TIME);
				//��ѯ���ȴ�ECHO�������ߣ�����˲�俪ʼ��ʱ
				while (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN) == RESET )
					{	}
				//��SONAR_MAX_WAIT_TIMEʱ���ڵȴ��Ƿ��л���
				
				if (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN))
				{ //���ܵ�����
						TIM3->CNT = 0;//��TIM2�����Ĵ����ļ���ֵ����
						TIM_Cmd(TIM3, ENABLE); 
						// �ߵ�ƽ��ʼ��ʱ
						while (GPIO_ReadInputDataBit(HCSR04_PORT, HCSR04_ECHO_PIN) == SET)
						{  }                      //��ѯ
						TIM_Cmd(TIM3, DISABLE); //ECHO�ߵ�ƽ���� ��������
						p[i] = TIM_GetCounter(TIM3)*1e-6; //����λ��usת��Ϊs				
				}
				time_s+=p[i];
				delay_ms(10);
			}
    time_s=time_s/5;
    return ( 100*time_s * SONAR_SOUND_SPEED / 2); //���ؾ��� ��λcm
		//������룺cnt * 1/10000 * 340 / 2(��λ��m) 
}
