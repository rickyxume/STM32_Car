#include "delay.h"
#include "blue.h"
#include "motor.h"
#include "hc_sr04.h"
#include "sg90.h"


int main()
{
	float length_res[5];  //������Ų����
	
//	float forward_distance=0;
//	float left45_distance=0;
//	float left60_distance=0;
//	float right45_distance=0;
//	float right60_distance=0;
	
	GPIO_init_Init(); //4������������ź͵������ʹ������PB5��ʼ��
	delay_init(); // �ӳٳ�ʼ��,ʹ�þ�׼��ʱǰ��Ҫ�������ʼ��������
	USART_Config();	//��������1�ĳ�ʼ��---���������շ�
	TIM4_PWM_Init(1000-1,0);	//���pwm���� TIM4
	HCSR04_Init(); //HC_SR04��������ʼ������ TIM2
	SG90_PWM_init();  // ���PWM�������---��ʱ��TIM3�ĳ�ʼ��
//	
//	while(1)
//	{
//		SG90_Turn_Forward;
//		delay_ms(300);
//		forward_distance = HCSR04_GetDistance();  //��ǰ���������������
//		USART_SendData(USART1,(char)HCSR04_GetDistance());
//		
//		if(left45_distance<45.00)     //���ǰ������С��45����  ͣ�������Ҿ���
//				{
//					CarStop();  delay_ms(1000);	
//					SG90_Turn_Left45;      //�����ת45�Ȳ��
//					delay_ms(300);		
//					left45_distance = HCSR04_GetDistance();    //�Ѳ�������Ž�����
//					
//					SG90_Turn_Left60;      //�����ת60�Ȳ��
//					delay_ms(300);		
//					left60_distance = HCSR04_GetDistance();    //�Ѳ�������Ž�����
//					
//					
//					SG90_Turn_Right45;     //�����ת45�Ȳ��
//					delay_ms(300);
//					right45_distance = HCSR04_GetDistance();     //�Ѳ�������Ž�����
//					
//					SG90_Turn_Right60;     //�����ת60�Ȳ��
//					delay_ms(300);
//					right60_distance = HCSR04_GetDistance();     //�Ѳ�������Ž�����			
//					
//					SG90_Turn_Forward;//ת������
//					delay_ms(100);
//					
//					
//					if(left45_distance<right45_distance || left60_distance<right60_distance)
//					{//�����߾���С���ұߣ�˵�����խ��
//						do{
//							SG90_Turn_Forward;delay_ms(100);
//							forward_distance = HCSR04_GetDistance();  //�ظ���ǰ���ľ���ͬʱ��ת					
//							CarRight(100);delay_ms(300);
//							
//							SG90_Turn_Forward;delay_ms(100);
//							if(HCSR04_GetDistance()<15)
//							{
//								CarLeftAround(100);delay_ms(1000);
//							}else{
//								SG90_Turn_Forward;delay_ms(100);
//								CarForwardRight(100);delay_ms(500);
//							}
//							
//							if(HCSR04_GetDistance()<forward_distance+5 || HCSR04_GetDistance()>forward_distance-5)
//							{
//								//��ס��
//								CarBack(100);delay_ms(200);
//							}
//							
//							SG90_Turn_Forward;delay_ms(100);
//							CarStop();  delay_ms(1000);	
//							}while(forward_distance<10.00);		//һֱת��ǰ���������10cm
//					}else{ // �����ұ�խ�ˣ�Ҫ��ת
//							do{
//									SG90_Turn_Forward;delay_ms(100);
//									forward_distance =HCSR04_GetDistance();  //�ظ���ǰ���ľ���ͬʱ��ת					
//									CarLeft(100);delay_ms(300);
//								
//									SG90_Turn_Forward;delay_ms(100);
//								if(HCSR04_GetDistance()<30)
//									{
//									CarRightAround(100);delay_ms(1000);
//									}else{
//										SG90_Turn_Forward;delay_ms(100);
//										CarForwardLeft(100);delay_ms(500);
//									}
//									SG90_Turn_Forward;delay_ms(100);
//									
//									if(HCSR04_GetDistance()<forward_distance+5 || HCSR04_GetDistance()>forward_distance-5)
//									{
//										//��ס��
//										CarBack(100);delay_ms(200);
//									}
//									CarStop();  delay_ms(1000);	
//									}while(forward_distance<10.00);		//һֱת��ǰ���������10cm
//					}
//					if(forward_distance>45.00)       //���ǰ���������45cm  ǰ��
//					{
//						SG90_Turn_Forward;delay_ms(100);
//						CarGo(500);delay_ms(1000);	
//					}
//				}				
//	}
while(1)
	{	
		SG90_Turn_Forward;   //�������
		delay_ms(100);
        length_res[0] =Senor_Using();  //��ǰ���������������
		USART_SendData(USART1,(char)length_res[0]);
		delay_ms(100);
		
		if(length_res[0]>45.00)       //���ǰ���������30cm  ǰ��
			{
			CarGo();
			}	
				
		if(length_res[0]<45.00)     //���ǰ������С��30����  ͣ�������Ҿ���
			{
			CarStop();  
			SG90_Turn_Left45;      //�����ת45�Ȳ��
			delay_ms(300);		
			length_res[1] =Senor_Using();    //�Ѳ�������Ž�����
			
            SG90_Turn_Right45;     //�����ת45�Ȳ��
			delay_ms(300);	
            length_res[4] =Senor_Using();     //�Ѳ�������Ž�����				
				
			SG90_Turn_Forward;           //�������
			delay_ms(100); 
			CarBack();
			delay_ms(100);
			if(length_res[1]>length_res[4])    //�����ߵľ�������ұߵľ���
				{
					do                        //�������
					{
					SG90_Turn_Forward;
					delay_ms(100);
					length_res[0] =Senor_Using();	//�ظ���ǰ���ľ���ͬʱ��ת			
					CarLeft();
					delay_ms(100);
					}
					while(length_res[0]<35.00);		//һֱת��ǰ���������30cm		
				}
		   if(length_res[1]<length_res[4])    //����ұߵľ��������ߵľ���
				{
					do
					{
					SG90_Turn_Forward;
					delay_ms(100);
					length_res[0] =Senor_Using();  //�ظ���ǰ���ľ���ͬʱ��ת						
					CarRight();
					delay_ms(100);
					}
					while(length_res[0]<35.00);		//һֱת��ǰ���������30cm
				}
				
			}		
	
	}	
	

	
//	//���߼�
//	while(1){
//		SG90_Front;   //�������
//		delay_ms(100);
//    length_res[0] =Senor_Using();  //��ǰ���������������
//		delay_ms(10);
//		
//		if(length_res[0]>20.00)       //���ǰ���������30cm  ǰ��
//			{
//				CarGo(100);
//			}
//				
//		if(length_res[0]<20.00)     //���ǰ������С��30����  ͣ�������Ҿ���
//			{
//				CarStop();  
//				SG90_Left_45;      //�����ת45�Ȳ��
//				delay_ms(100);		
//				length_res[1] =Senor_Using();    //�Ѳ�������Ž�����
//				
//				SG90_Right_45;     //�����ת45�Ȳ��
//				delay_ms(100);	
//				length_res[4] =Senor_Using();     //�Ѳ�������Ž�����				
//					
//				SG90_Front;           //�������
//				delay_ms(100); 
//				if(length_res[1]>length_res[4])    //�����ߵľ�������ұߵľ���
//					{
//						do                        //�������
//						{
//						SG90_Front;
//						delay_ms(100);
//						length_res[0] =Senor_Using();	//�ظ���ǰ���ľ���ͬʱ��ת				
//						CarLeft(100);
//						delay_ms(500);		
//						}
//						while(length_res[0]<15.00);		//һֱת��ǰ���������30cm		
//					}

//				 if(length_res[1]<length_res[4])    //����ұߵľ��������ߵľ���
//					{
//						do
//						{
//						SG90_Front;
//						delay_ms(100);
//						length_res[0] =Senor_Using();  //�ظ���ǰ���ľ���ͬʱ��ת					
//						CarRight(100);
//						delay_ms(500);	
//						}
//						while(length_res[0]<15.00);		//һֱת��ǰ���������30cm
//					}
//					
//			}		
//	}		
	return 0;
}

