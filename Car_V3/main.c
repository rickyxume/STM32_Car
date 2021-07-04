#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "motor.h"
#include "sg90.h"
#include "hcsr04.h"
#include "red.h"
#include "blue.h"
#include "delay.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Motor_Init();     //���motor��ʼ�� 
	Bluetooth_Config();//�������ó�ʼ��
	SystemInit();	    //����ϵͳʱ��Ϊ72M 
	delay_init();     //��ʱ��ʼ��		
	Red_Config();     //��ʼ����������
	TIM4_PWM_Init();	//���pwm���� TIM4
	SG90_pwm_init();  //���SG-90��ʼ�� pwmʵ��ת�� TIM3
	HC_SR04_Init();   //������HC-SR04��ʼ������ʱ��TIM2
	
	char blue_flag=0;//����һ��������ʱ����������ڽ������ݵı���
	while(1){			
		
		SG90_Front;//ת�����
		//���ú��⴫�����򵥱���
		if(R==0&&L==0){CarBack(800);delay_ms(50);DelayCheck(100);} //������Ҷ����ϰ������һ�£���ͣ�ж�
		if(R==0){CarBackLeft(100);delay_ms(50);DelayCheck(100);}   //����ұ����ϰ����󵹳�һ�£���ͣ�ж�
		if(L==0){CarBackRight(100);delay_ms(50);DelayCheck(100);}  //���������ϰ����ҵ���һ�£���ͣ�ж�
		//С�����������ֻ��������͵����ݣ����������
		blue_flag = USART_ReceiveData(USART1);
		//�����ն��������ַ���0�������־�ͣ����
		//������һֱ���У��൱����while(1)ѭ��
		switch(blue_flag){
			case 'A': //������
				delay_ms(10); Ultrasonic_Avoidance();break;
			case 'B': //����
				delay_ms(10); Red_Avoidance();break;
			case 'C': //�����¹���
				CarTest();delay_ms(500);break;
			case '0': //ֹͣ
				delay_ms(10); CarStop(); delay_ms(10); break;
			case '1'://��ת
				delay_ms(10); CarLeft(999); break;
			case '2': //��ת
				delay_ms(10); CarRight(999); break;
			case '3': //����
				delay_ms(10); CarBack(999); break;
			case '4': //ǰ��
				delay_ms(10); CarGo(999); break;
			case '5': //��ǰ��ת
				delay_ms(10); CarForwardLeft(100); break;
			case '6': //��ǰ��ת
				delay_ms(10); CarForwardRight(100); break;
			case '7': //�󵹳�
				delay_ms(10); CarBackLeft(999); break;
			case '8': //�ҵ���
				delay_ms(10); CarBackRight(999); break;
			default:  //Ĭ��ֹͣ
				delay_ms(10); CarStop(); delay_ms(10); break;
				}	
	}
	
}
