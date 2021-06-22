#ifndef __MOTOR_H
#define	__MOTOR_H


#include "stm32f10x.h"


//void TIM3_PWM_Init(void);
void CarGo(void);
void CarStop(void);
void CarBack(void);
void CarLeft(void);
void CarBigLeft(void);
void CarRight(void);
void CarBigRight(void); 
void CarLeftAround(void);
void CarRightAround(void);
void GPIO_init_Init(void);


#endif
