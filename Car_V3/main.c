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
	Motor_Init();     //电机motor初始化 
	Bluetooth_Config();//蓝牙配置初始化
	SystemInit();	    //配置系统时钟为72M 
	delay_init();     //延时初始化		
	Red_Config();     //初始化红外配置
	TIM4_PWM_Init();	//电机pwm调速 TIM4
	SG90_pwm_init();  //舵机SG-90初始化 pwm实现转向 TIM3
	HC_SR04_Init();   //超声波HC-SR04初始化，定时器TIM2
	
	char blue_flag=0;//定义一个用来临时存放蓝牙串口接收数据的变量
	while(1){			
		
		SG90_Front;//转正舵机
		//先用红外传感器简单避障
		if(R==0&&L==0){CarBack(800);delay_ms(50);DelayCheck(100);} //如果左右都有障碍物，后退一下，暂停判断
		if(R==0){CarBackLeft(100);delay_ms(50);DelayCheck(100);}   //如果右边有障碍，左倒车一下，暂停判断
		if(L==0){CarBackRight(100);delay_ms(50);DelayCheck(100);}  //如过左边有障碍，右倒车一下，暂停判断
		//小车监听接收手机蓝牙发送的数据，存进变量里
		blue_flag = USART_ReceiveData(USART1);
		//蓝牙终端设置松手发送0，即松手就停车，
		//长按就一直进行，相当于做while(1)循环
		switch(blue_flag){
			case 'A': //超声波
				delay_ms(10); Ultrasonic_Avoidance();break;
			case 'B': //红外
				delay_ms(10); Red_Avoidance();break;
			case 'C': //测试新功能
				CarTest();delay_ms(500);break;
			case '0': //停止
				delay_ms(10); CarStop(); delay_ms(10); break;
			case '1'://左转
				delay_ms(10); CarLeft(999); break;
			case '2': //右转
				delay_ms(10); CarRight(999); break;
			case '3': //后退
				delay_ms(10); CarBack(999); break;
			case '4': //前进
				delay_ms(10); CarGo(999); break;
			case '5': //向前左转
				delay_ms(10); CarForwardLeft(100); break;
			case '6': //向前右转
				delay_ms(10); CarForwardRight(100); break;
			case '7': //左倒车
				delay_ms(10); CarBackLeft(999); break;
			case '8': //右倒车
				delay_ms(10); CarBackRight(999); break;
			default:  //默认停止
				delay_ms(10); CarStop(); delay_ms(10); break;
				}	
	}
	
}
