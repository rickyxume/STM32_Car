#include "hcsr04.h"


//��¼��ʱ���������
uint overcount=0;

//�����ж����ȼ�
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

//��ʼ��ģ���GPIO�Լ���ʼ����ʱ��TIM2
void HC_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//TRIG�����ź�
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_11; //LC1 --> PB11
	GPIO_Init(GPIOB, &GPIO_InitStructer);

	//ECOH�����ź�
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_12;//LC2 --> PB12
	GPIO_Init(GPIOB, & GPIO_InitStructer);

	//��ʱ��TIM2��ʼ��
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStructer.TIM_Period=999;//1000-1��ʱ����Ϊ1000
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; //72-1��Ƶϵ��72
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//���������ж�
	NVIC_Config();
	TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��ʹ��

}



float GetDistance(void)
{
	float length=0,sum=0;
	u16 tim;
	uint i=0;
	//�ο����ϵĲ����룬����ʹ���˾�ֵ�˲���������5��ȡƽ��ֵʹ�ò����ȶ�׼ȷ
	while(i!=5)/*��5�����ݼ���һ��ƽ��ֵ*/
	{
		PBout(11)=1; //��װ�ú��������=1�������źţ���Ϊ�����ź�
		delay_us(20); //�ߵ�ƽ�ź���Ҫ����10us
		PBout(11)=0;
		/*�ȴ������ź�*/
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET);
		TIM_Cmd(TIM2,ENABLE);//�����źŵ�����������ʱ������

		i+=1; //ÿ�յ�һ�λ����ź�+1���յ�5�ξͼ����ֵ
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==SET);//�����ź���ʧ
		TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��

		tim=TIM_GetCounter(TIM2);//��ȡ��TIM2���Ĵ����еļ���ֵ��һ�߼�������ź�ʱ��

		length=(tim+overcount*1000)/58.0;//ͨ�������źż������

		sum=length+sum;
		TIM2->CNT=0; //��TIM2�����Ĵ����ļ���ֵ����
		overcount=0; //�ж������������
		delay_ms(10);
	}
	length=sum/5;
	return length;//������Ϊ��������ֵ����λcm
}



void TIM2_IRQHandler(void) //�жϣ��������źźܳ�ʱ������ֵ������ظ����������ж��������������
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	 {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����жϱ�־
		overcount++;
	 }
}

//������ϸ���	
void red_helper()
{
	CarStop();//��ֹͣ�ȶ���������С���ĺ��⴫�����ж�һ�������ϰ������
	if(R==0&&L==0){CarBack(800);delay_ms(100);DelayCheck(10);} //������Ҷ��ǵ͵�ƽ�������Ҷ����ϰ�����ô�ͺ���һ��
	if(R==0){CarLeft(999);delay_ms(100);}  //����ұ����ϰ��������תһ��
	if(L==0){CarRight(999);delay_ms(100);} //���������ϰ��������תһ��
	if(R&&L){ 														 //�������1�������Ǹߵ�ƽ������û�м�⵽�ϰ������ǰ��һ��
		CarGo(999);delay_ms(200); //һ��ʼ�ٶȿ�һ�㣬֮���ٽ�������
		//delay_ms(200);DelayCheck(300);
	}
}

//����������
void Ultrasonic_Avoidance()
{
	float distancF=0;
	float distanceR45=0;
	float distanceR60=0;
	float distanceR90=0;
	float distanceL45=0;
	float distanceL60=0;
	float distanceL90=0;
	//while(1) //ȥ��whileѭ����ʹ���������Կ���С��
	//	{			//�����ȥ������С��һֱ����whileѭ���У����ֻ���Գ�������������Լ���
			red_helper();//������ϸ��������ú����ж�һ��С�������ϰ����������������Ӧ
			
			//��ʼ���������
			SG90_Front; delay_ms(100); //�����������һ���ӳٷ���
			distancF = GetDistance();  //��ǰ������
			
			USART_SendData(USART1,(char)GetDistance());//�������豸��������
	
			delay_ms(100);//��ʱ����
			if(distancF>30.00)       //���ǰ���������30cm 
			{
				CarStop();//��ͣ�����ú�������߽������ϰ���
				if(R==0&&L==0){CarBack(800);delay_ms(100);DelayCheck(10);} //���߶����ϰ��򵹳��������ϰ���ͣһ��
				if(R==0){CarLeft(999);delay_ms(100);}	//����ұ����ϰ���󵹳�
				if(L==0){CarRight(999);delay_ms(100);}//���������ϰ���ҵ���
				if(R&&L){ CarGo(200);delay_ms(200);}  //������Ҷ�û��⵽�ϰ�������ǰ��
				distancF = GetDistance(); //һֱ���ֱ������������ǰ���������30cm��
			}
			
			if(distancF<30.00)     //���ǰ������С��30����  ͣ�������Ҿ���
			{
					CarStop();    delay_ms(100);
					CarBack(800); delay_ms(10);

					CarStop(); delay_ms(10);SG90_Left_45; delay_ms(300);//�����ת45�Ȳ��
					distanceL45 = GetDistance();//�������������
				
					//CarStop(); delay_ms(10);SG90_Left_60; delay_ms(150);
					//distanceL60 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Left_90; delay_ms(300);//�����ת90�Ȳ��
					distanceL90 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Right_45; delay_ms(300);//�����ת45�Ȳ��
					distanceR45 = GetDistance();
					
//					CarStop(); delay_ms(10);SG90_Right_60; delay_ms(150);
//					distanceR60 = GetDistance();
					
					CarStop(); delay_ms(10);SG90_Right_90; delay_ms(300);//�����ת90�Ȳ��
					distanceR90 = GetDistance();
					SG90_Front; delay_ms(50);  //�������
					CarBack(800); delay_ms(100);//�Ⱥ���һ���ٽ�����һ������
					
//					//���60���ұ�60�����
//					if(distanceL60 < distanceR60)
//					{								
//						do
//							{
//							SG90_Front;delay_ms(100);//�������
//							distancF = GetDistance();	//�ظ���ǰ���ľ���ͬʱ		
//							CarRight(999);delay_ms(50);//��ת	
//							}while(distancF<25.00);		//һֱת��ǰ���������25cm
//					}else{//���60���ұ�60���볤
//						do{
//							SG90_Front;delay_ms(100);//�������
//							distancF = GetDistance();	//�ظ���ǰ���ľ���ͬʱ 
//							CarLeft(999);delay_ms(50);//��ת
//							}while(distancF<25.00);		//һֱת��ǰ���������25cm
//					}
					
					//���45���ұ�45�����
					if(distanceL45 < distanceR45)
						{								
							do{
								SG90_Front;delay_ms(100);	//�������
								distancF = GetDistance();	//�ظ���ǰ���ľ���		
								CarRight(999);delay_ms(40);	//��ת	
							}while(distancF<35.00);		//һֱת��ǰ���������35cm
					}else{//���45���ұ�45���볤
							do{
								SG90_Front;delay_ms(100);	//�������
								distancF = GetDistance();	//�ظ���ǰ���ľ���ͬʱ
								CarLeft(999);delay_ms(40);	//��ת
								}while(distancF<35.00);		//һֱת��ǰ���������35cm
					}
					
					if(distanceL90<10 && distanceR90<10)//������90̫խ&&�ұ�90Ҳ̫խ
					{
							CarBack(200);delay_ms(100);//����һ��
					}
				}
	//}	//while����

}