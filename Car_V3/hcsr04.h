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

void HC_SR04_Init(void);  //������ģ��������ó�ʼ��
float GetDistance(void);  //��ຯ��������ֵ��Ϊ����
void NVIC_Config(void);   //�ж�����

void Ultrasonic_Avoidance(void);

#endif

