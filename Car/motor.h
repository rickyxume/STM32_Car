#ifndef __MOTOR_H
#define	__MOTOR_H


#include <stm32f10x.h>
#include "stm32f10x_tim.h"


// 控制电机引脚实现小车车轮变速转动
void TIM4_PWM_Init(u16,u16);
void GPIO_init_Init(void);

//void CarTest(int);
//void CarGo(int);
//void CarStop(void);
//void CarBack(int);
//void CarLeft(int);
//void CarRight(int);
//void CarForwardRight(int);
//void CarForwardLeft(int);
//void CarLeftAround(int);
//void CarRightAround(int);
//void CarBackLeft(int);
//void CarBackRight(int);

void CarGo(void);
void CarStop(void);
void CarBack(void);
void CarLeft(void);
void CarBigLeft(void);  //大右转
void CarRight(void);
void CarBigRight(void);   //大左转
#endif
