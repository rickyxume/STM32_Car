#ifndef __SG90_H
#define	__SG90_H

#include "stm32f10x.h"
#include "delay.h"
   
//����ʹ�ö�ʱ��TIM3ͨ��4��ռ�ձȵ�������Ƕȣ�����ת��Ƕȹ���˼��
#define   SG90_Right_90		TIM_SetCompare4(TIM3, 195)
#define   SG90_Right_60		TIM_SetCompare4(TIM3, 192)
#define   SG90_Right_45		TIM_SetCompare4(TIM3, 190)
#define   SG90_Front		TIM_SetCompare4(TIM3, 185)		
#define   SG90_Left_45		TIM_SetCompare4(TIM3, 180)
#define   SG90_Left_60		TIM_SetCompare4(TIM3, 178)
#define   SG90_Left_90		TIM_SetCompare4(TIM3, 175)

////���������Ҫ����ʵ��������Ѷ��λ�õ�����ȷ
//#define SG90_Left_45 TIM3->CCR4 = 1500//���ת����ʼλ�õ�0��//�ı䶨ʱ���ļ����趨ֵ  ʹ��PWM����ߵ�ƽΪ1ms
//#define SG90_Left_60 TIM3->CCR4 = 1667
////#define SG90_Trun_Left675 TIM3->CCR4 = 1750
////#define SG90_Turn_Left75 TIM3->CCR4 = 1833
////#define SG90_Turn1_Left90 TIM3->CCR4 = 2000

void SG90_pwm_init(void);  //���pwm��ʼ��

#endif
