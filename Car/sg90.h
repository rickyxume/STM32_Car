#ifndef __SG90_H
#define	__SG90_H

#include "stm32f10x.h"
#include "delay.h"
   
//#define   SG90_Right_90     TIM_SetCompare2(TIM3, 195)		
//#define   SG90_Right_45		TIM_SetCompare4(TIM3, 190)		
//#define   SG90_Front		TIM_SetCompare4(TIM3, 185)		
//#define   SG90_Left_45		TIM_SetCompare4(TIM3, 175)		
//#define   SG90_Left_90		TIM_SetCompare2(TIM3, 175)

////这个可能需要根据实际情况调整
//#define SG90_Turn_Forward TIM3->CCR4 = 1000 //舵机转动初始位置的0°//改变定时器的计数设定值  使其PWM输出高电平为1ms
//#define SG90_Turn_Right45 TIM3->CCR4 = 500
//#define SG90_Turn_Right60 TIM3->CCR4 = 333 //500-167
//#define SG90_Trun_Right675 TIM3->CCR4 = 250
////#define SG90_Turn_Right75 TIM3->CCR4 = 167 //500-333

//#define SG90_Turn_Left45 TIM3->CCR4 = 1500
//#define SG90_Turn_Left60 TIM3->CCR4 = 1667
//#define SG90_Trun_Left675 TIM3->CCR4 = 1750
//#define SG90_Turn_Left75 TIM3->CCR4 = 1833
////#define SG90_Turn1_Left90 TIM3->CCR4 = 2000


		
//日强的小车
#define SG90_Turn_Forward TIM3->CCR4 = 1667 //舵机转动初始位置的0°//改变定时器的计数设定值  使其PWM输出高电平为1ms
#define SG90_Turn_Right45 TIM3->CCR4 = 1167+200
#define SG90_Turn_Right60 TIM3->CCR4 = 1000+200
//#define SG90_Trun_Right675 TIM3->CCR4 = 250
//#define SG90_Turn_Right75 TIM3->CCR4 = 167 //500-333

#define SG90_Turn_Left45 TIM3->CCR4 = 1667+500-200
#define SG90_Turn_Left60 TIM3->CCR4 = 1667+500+167-200
//#define SG90_Trun_Left675 TIM3->CCR4 = 1750
//#define SG90_Turn_Left75 TIM3->CCR4 = 1833
//#define SG90_Turn1_Left90 TIM3->CCR4 = 2000


void SG90_PWM_init(void);  //舵机pwm初始化

#endif
