#include "red.h"

	
void Red_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��
	
	//���õ������
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//ѡ���Ӧ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB�˿�
	

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PA�˿�ʱ��
	
	//���ú�������11
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 ;	//ѡ���Ӧ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����GPIOģʽ,��������       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PC�˿�
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PA�˿�ʱ��
	
	//���ú�������10
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;	//ѡ���Ӧ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����GPIOģʽ,��������       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PC�˿�

}

//��ȡ������ź�LR
//��ʱ��ͬʱ�����⣬һ�������ϰ����ֹͣ��������ʱ
void DelayCheck(int ms)
{
	while(ms--)
	{
		delay_ms(1); //��ʱ1ms
		if(L==0 || R==0) //һ��һ�߼�⵽�ϰ����ͣ����
		{
			CarStop();
			return;
		}
	}
}

void Red_Avoidance()
{
	if(L==0){//���L���ص͵�ƽ�����⵽������ϰ���
		do{
			CarStop();delay_ms(100);
			CarBackRight(900);delay_ms(200);CarRight(999);delay_ms(100);DelayCheck(500);
		}while(L==0);//һֱ����+��ת�����û���ϰ���
						
	}
	if(R==0){//���R���ص͵�ƽ�����⵽�ұ����ϰ���
		do{
			CarStop();delay_ms(100);
			CarBackLeft(900);delay_ms(200);CarLeft(999);delay_ms(100);DelayCheck(500);
		}while(R==0);//һֱ����+��ת���ұ�û���ϰ���
	}
	if(R==0&&L==0){
		CarStop();delay_ms(100);
		CarBack(900);delay_ms(500);
		CarRight(999);DelayCheck(500);
	}
	if(R&&L){ //���߶�û��⵽�ϰ���
		CarGo(700);//��ǰ
		delay_ms(200);
		DelayCheck(300);
	}
}