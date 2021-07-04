#ifndef __HCSR04_H
#define __HCSR04_H 	

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "sg90.h"
#include "motor.h"
#include "red.h"

#define uint unsigned int
#define TRIG_Send PBout(11)
#define ECHO_Reci PBin(12)

void HC_SR04_Init(void);  //超声波模块相关配置初始化
float GetDistance(void);  //测距函数，返回值即为距离
void NVIC_Config(void);   //中断配置

void Ultrasonic_Avoidance(void);

#endif

