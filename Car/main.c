#include <stm32f10x.h>
#include "delay.h"
#include "blue.h"
#include "motor.h"

u8 blue_flag;//定义一个用来临时存放串口接收数据的变量
int main()
{
	GPIO_init_Init(); //引脚初始化
	delay_init(); // 延迟初始化
	USART_Config();	//蓝牙串口
	TIM4_PWM_Init();	//电机pwm调速TIM4
	//主逻辑
	while(1){
		blue_flag = USART_ReceiveData(USART1);
		switch(blue_flag){
			case '0': 
				delay_ms(10); 
				CarStop();     
				break;
			case '1':
				delay_ms(10); 
				CarLeft(300);     
				break;
			case '2': 
				delay_ms(10); 
				CarRight(300);    
				break;
			case '3': 
				delay_ms(10); 
				CarBack(500);     
				break;
			case '4': 
				delay_ms(10); 
				CarGo(100);
				break;
			case '5': 
				delay_ms(10); 
				CarForwardLeft(100); 
				break;
			case '6': 
				delay_ms(10); 
				CarForwardRight(100);
				break;
			case '7': 
				delay_ms(10); 
				CarBackLeft(100);
				break;
			case '8': 
				delay_ms(10); 
				CarBackRight(100);
				break;
			case 'l': 
				delay_ms(10); 
				CarLeftAround(100);
				break;
			case 'r': 
				delay_ms(10); 
				CarRightAround(100);
				break;
			case '9':
				delay_ms(500);
				CarGo(100);
				delay_ms(500); 	
				CarLeft(300);
				delay_ms(500); 
				CarRight(300); 
				delay_ms(500);
				CarGo(100);	
				delay_ms(1000); 
				CarLeftAround(100); 
				delay_ms(1000); 
				CarRightAround(100);
				delay_ms(500); 
				CarBack(500); 
				delay_ms(1000); 
				CarBackLeft(100);
				delay_ms(1000); 
				CarBack(500); 
				delay_ms(1000); 
				CarBackRight(100);
				delay_ms(1000); 
				CarStop(); 
				break;
			default: 
				delay_ms(10); 	
				CarStop();
				break;
		}		  
	}		
	return 0;
}

