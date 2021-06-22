#include <stm32f10x.h>
#include "delay.h"
#include "blue.h"
#include "motor.h"
//#include "stm32f10x_tim.h"

u8 blue_flag;
int main()
{
	GPIO_init_Init(); //���ų�ʼ��
	delay_init(); // �ӳٳ�ʼ��
	USART_Config();	//��������
	//TIM3_PWM_Init();	//���pwm   TIM3
	//delay_ms(50);
	while(1){
			blue_flag = USART_ReceiveData(USART1);
			switch(blue_flag){
				case '0': CarStop();     break  ;
				case '1': CarLeft();     break  ;
				case '2': CarRight();    break  ;
				case '3': CarBack();     break  ;
				case '4': CarGo();    		break  ;
				case '5': CarLeftAround(); break  ;
				case '6': CarRightAround();break  ;
				default: CarStop();        break  ;  
			}		  
		}
	return 0;
}

