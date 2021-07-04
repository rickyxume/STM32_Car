#ifndef __RED_H
#define	__RED_H

#include "stm32f10x.h"
#include "delay.h"
#include "sg90.h"
#include "motor.h"

//读红外模块返回的值
#define L GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define R  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)


void Red_Config(void);
void Red_Avoidance(void);
void DelayCheck(int);
#endif

