#include "blue.h"


static void NVIC_Configuration(void)		//�ж�����
{
	NVIC_InitTypeDef NVIC_InitStructure; //����һ��������ʼ���ж����ȼ��Ľṹ��
  //****����USART1���ж����ȼ�----�й��ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//USART1ȫ���жϺţ����ƴ�ҵ����֤��  ��оƬ֪�����ĸ��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3 //��һ���ȼ���ֵ 0x00~0x0F (0-16) ����ԽС ���ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3 //�ڶ����ȼ���ֵ 0x00~0x0F (0-16) ����ԽС ���ȼ�Խ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}


void USART_Config(void) //��������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	

	//�����õ���USART1  PA9--TX--AF_PP���������������  PA10--RX--IN_FLOATING(��������ģʽ)
	//  USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10     
	
		/* USARTx configured as follow:
		 - BaudRate = 9600 baud  ������
		 - Word Length = 8 Bits  ���ݳ���
		 - One Stop Bit          ֹͣλ
		 - No parity             У�鷽ʽ
		 - Hardware flow control disabled (RTS and CTS signals) Ӳ��������
		 - Receive and transmit enabled                         ʹ�ܷ��ͺͽ���
	*/

	
	// ���ô��ڵĹ�������
	
	USART_InitStructure.USART_BaudRate = 9600; // ���ò�����(����һ�µĲ�����)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��Ч���ݳ���8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1bitֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��ҪУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����ҪӲ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);
	
	// �����ж����ȼ����ã����ݼ�����NVIC_Configuration����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�,����1�����ݹ����ͻ���ת���жϺ������д���
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�	
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);                  
  // ���������ɱ�־
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

//���ڵ��жϷ����� ֻҪ����1�����ݽ��յ�  ����ת��������
void USART1_IRQHandler(void)
{//�ȶ�ȡ���жϴ��ڵĽ��������жϱ�־λ��û�д��� 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)      //���ָ���� USART1 �жϷ������
		{ 
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //��� USART1 ���жϴ�����λ
		}
}


