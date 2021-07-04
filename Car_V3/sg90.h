#ifndef __SG90_H
#define	__SG90_H

#include "stm32f10x.h"
#include "delay.h"
   
//这里使用定时器TIM3通道4的占空比调整舵机角度，下面转向角度顾名思义
#define   SG90_Right_90		TIM_SetCompare4(TIM3, 195)
#define   SG90_Right_60		TIM_SetCompare4(TIM3, 192)
#define   SG90_Right_45		TIM_SetCompare4(TIM3, 190)
#define   SG90_Front		TIM_SetCompare4(TIM3, 185)		
#define   SG90_Left_45		TIM_SetCompare4(TIM3, 180)
#define   SG90_Left_60		TIM_SetCompare4(TIM3, 178)
#define   SG90_Left_90		TIM_SetCompare4(TIM3, 175)

////这个可能需要根据实际情况，把舵机位置调整正确
//#define SG90_Left_45 TIM3->CCR4 = 1500//舵机转动初始位置的0°//改变定时器的计数设定值  使其PWM输出高电平为1ms
//#define SG90_Left_60 TIM3->CCR4 = 1667
////#define SG90_Trun_Left675 TIM3->CCR4 = 1750
////#define SG90_Turn_Left75 TIM3->CCR4 = 1833
////#define SG90_Turn1_Left90 TIM3->CCR4 = 2000

void SG90_pwm_init(void);  //舵机pwm初始化

#endif
