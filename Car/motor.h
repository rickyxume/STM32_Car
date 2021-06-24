#ifndef __MOTOR_H
#define	__MOTOR_H


#include "stm32f10x.h"

// ���Ƶ������ʵ��С�����ֱ���ת��
void TIM4_PWM_Init(void);
void CarTest(int);
void CarGo(int);
void CarStop(void);
void CarBack(int);
void CarLeft(int);
void CarRight(int);
void CarForwardRight(int);
void CarForwardLeft(int);
void CarLeftAround(int);
void CarRightAround(int);
void CarBackLeft(int);
void CarBackRight(int);
void GPIO_init_Init(void);


#endif
