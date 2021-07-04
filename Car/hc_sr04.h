#ifndef __HC_SR04_H
#define	__HC_SR04_H

#include <stm32f10x.h>
#include "delay.h"
#include "blue.h"
#include <stdio.h>

////HC_SR04超声波初始化
//void HC_SR04_GPIO_Config(void);
////发送20us的脉冲触发信号
//void HC_SR04_start(void);
//void HC_SR04_TIM2_Init(u16,u16);
//float get_Distance(void);


float Senor_Using(void);
void HCSR04_Init(void);
float HCSR04_GetDistance(void);
#endif
