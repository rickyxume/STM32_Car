#include "delay.h"
#include "blue.h"
#include "motor.h"
#include "hc_sr04.h"
#include "sg90.h"


int main()
{
	float length_res[5];  //用来存放测距结果
	
//	float forward_distance=0;
//	float left45_distance=0;
//	float left60_distance=0;
//	float right45_distance=0;
//	float right60_distance=0;
	
	GPIO_init_Init(); //4个电机控制引脚和电机供电使能引脚PB5初始化
	delay_init(); // 延迟初始化,使用精准延时前需要加这个初始化才能用
	USART_Config();	//蓝牙串口1的初始化---蓝牙数据收发
	TIM4_PWM_Init(1000-1,0);	//电机pwm调速 TIM4
	HCSR04_Init(); //HC_SR04超声波初始化配置 TIM2
	SG90_PWM_init();  // 舵机PWM输出控制---定时器TIM3的初始化
//	
//	while(1)
//	{
//		SG90_Turn_Forward;
//		delay_ms(300);
//		forward_distance = HCSR04_GetDistance();  //测前方距离放在数组里
//		USART_SendData(USART1,(char)HCSR04_GetDistance());
//		
//		if(left45_distance<45.00)     //如果前方距离小于45厘米  停车测左右距离
//				{
//					CarStop();  delay_ms(1000);	
//					SG90_Turn_Left45;      //舵机左转45度测距
//					delay_ms(300);		
//					left45_distance = HCSR04_GetDistance();    //把测量结果放进数组
//					
//					SG90_Turn_Left60;      //舵机左转60度测距
//					delay_ms(300);		
//					left60_distance = HCSR04_GetDistance();    //把测量结果放进数组
//					
//					
//					SG90_Turn_Right45;     //舵机右转45度测距
//					delay_ms(300);
//					right45_distance = HCSR04_GetDistance();     //把测量结果放进数组
//					
//					SG90_Turn_Right60;     //舵机右转60度测距
//					delay_ms(300);
//					right60_distance = HCSR04_GetDistance();     //把测量结果放进数组			
//					
//					SG90_Turn_Forward;//转正方向
//					delay_ms(100);
//					
//					
//					if(left45_distance<right45_distance || left60_distance<right60_distance)
//					{//如果左边距离小于右边，说明左边窄了
//						do{
//							SG90_Turn_Forward;delay_ms(100);
//							forward_distance = HCSR04_GetDistance();  //重复测前方的距离同时右转					
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
//								//卡住了
//								CarBack(100);delay_ms(200);
//							}
//							
//							SG90_Turn_Forward;delay_ms(100);
//							CarStop();  delay_ms(1000);	
//							}while(forward_distance<10.00);		//一直转到前方距离大于10cm
//					}else{ // 否则，右边窄了，要左转
//							do{
//									SG90_Turn_Forward;delay_ms(100);
//									forward_distance =HCSR04_GetDistance();  //重复测前方的距离同时左转					
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
//										//卡住了
//										CarBack(100);delay_ms(200);
//									}
//									CarStop();  delay_ms(1000);	
//									}while(forward_distance<10.00);		//一直转到前方距离大于10cm
//					}
//					if(forward_distance>45.00)       //如果前方距离大于45cm  前进
//					{
//						SG90_Turn_Forward;delay_ms(100);
//						CarGo(500);delay_ms(1000);	
//					}
//				}				
//	}
while(1)
	{	
		SG90_Turn_Forward;   //舵机摆正
		delay_ms(100);
        length_res[0] =Senor_Using();  //测前方距离放在数组里
		USART_SendData(USART1,(char)length_res[0]);
		delay_ms(100);
		
		if(length_res[0]>45.00)       //如果前方距离大于30cm  前进
			{
			CarGo();
			}	
				
		if(length_res[0]<45.00)     //如果前方距离小于30厘米  停车测左右距离
			{
			CarStop();  
			SG90_Turn_Left45;      //舵机左转45度测距
			delay_ms(300);		
			length_res[1] =Senor_Using();    //把测量结果放进数组
			
            SG90_Turn_Right45;     //舵机右转45度测距
			delay_ms(300);	
            length_res[4] =Senor_Using();     //把测量结果放进数组				
				
			SG90_Turn_Forward;           //舵机摆正
			delay_ms(100); 
			CarBack();
			delay_ms(100);
			if(length_res[1]>length_res[4])    //如果左边的距离大于右边的距离
				{
					do                        //舵机摆正
					{
					SG90_Turn_Forward;
					delay_ms(100);
					length_res[0] =Senor_Using();	//重复测前方的距离同时左转			
					CarLeft();
					delay_ms(100);
					}
					while(length_res[0]<35.00);		//一直转到前方距离大于30cm		
				}
		   if(length_res[1]<length_res[4])    //如果右边的距离大于左边的距离
				{
					do
					{
					SG90_Turn_Forward;
					delay_ms(100);
					length_res[0] =Senor_Using();  //重复测前方的距离同时右转						
					CarRight();
					delay_ms(100);
					}
					while(length_res[0]<35.00);		//一直转到前方距离大于30cm
				}
				
			}		
	
	}	
	

	
//	//主逻辑
//	while(1){
//		SG90_Front;   //舵机摆正
//		delay_ms(100);
//    length_res[0] =Senor_Using();  //测前方距离放在数组里
//		delay_ms(10);
//		
//		if(length_res[0]>20.00)       //如果前方距离大于30cm  前进
//			{
//				CarGo(100);
//			}
//				
//		if(length_res[0]<20.00)     //如果前方距离小于30厘米  停车测左右距离
//			{
//				CarStop();  
//				SG90_Left_45;      //舵机左转45度测距
//				delay_ms(100);		
//				length_res[1] =Senor_Using();    //把测量结果放进数组
//				
//				SG90_Right_45;     //舵机右转45度测距
//				delay_ms(100);	
//				length_res[4] =Senor_Using();     //把测量结果放进数组				
//					
//				SG90_Front;           //舵机摆正
//				delay_ms(100); 
//				if(length_res[1]>length_res[4])    //如果左边的距离大于右边的距离
//					{
//						do                        //舵机摆正
//						{
//						SG90_Front;
//						delay_ms(100);
//						length_res[0] =Senor_Using();	//重复测前方的距离同时左转				
//						CarLeft(100);
//						delay_ms(500);		
//						}
//						while(length_res[0]<15.00);		//一直转到前方距离大于30cm		
//					}

//				 if(length_res[1]<length_res[4])    //如果右边的距离大于左边的距离
//					{
//						do
//						{
//						SG90_Front;
//						delay_ms(100);
//						length_res[0] =Senor_Using();  //重复测前方的距离同时右转					
//						CarRight(100);
//						delay_ms(500);	
//						}
//						while(length_res[0]<15.00);		//一直转到前方距离大于30cm
//					}
//					
//			}		
//	}		
	return 0;
}

