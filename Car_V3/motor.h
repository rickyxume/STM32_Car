#ifndef __MOTOR_H
#define	__MOTOR_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "sg90.h"
#include "blue.h"
#include "hcsr04.h"
#include "red.h"

void TIM4_PWM_Init(void);
void Motor_Init(void);
void CarGo(int);
void CarStop(void);
void CarBack(int);
void CarLeft(int);  //×ó×ª
void CarRight(int);  //ÓÒ×ª

void CarForwardLeft(int);
void CarForwardRight(int);
void CarBackLeft(int);
void CarBackRight(int);

void CarTest(void);

#endif
