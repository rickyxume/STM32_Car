#include "blue.h"


static void NVIC_Configuration(void)		//中断配置
{
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个用来初始化中断优先级的结构体
  //****配置USART1的中断优先级----有关中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//USART1全局中断号，类似大家的身份证号  让芯片知道是哪个中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3 //第一优先级数值 0x00~0x0F (0-16) 数字越小 优先级越高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3 //第二优先级数值 0x00~0x0F (0-16) 数字越小 优先级越高
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


void USART_Config(void) //串口配置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	

	//配置用到的USART1  PA9--TX--AF_PP（复用推挽输出）  PA10--RX--IN_FLOATING(浮空输入模式)
	//  USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10     
	
		/* USARTx configured as follow:
		 - BaudRate = 9600 baud  波特率
		 - Word Length = 8 Bits  数据长度
		 - One Stop Bit          停止位
		 - No parity             校验方式
		 - Hardware flow control disabled (RTS and CTS signals) 硬件控制流
		 - Receive and transmit enabled                         使能发送和接收
	*/

	
	// 配置串口的工作参数
	
	USART_InitStructure.USART_BaudRate = 9600; // 配置波特率(蓝牙一致的波特率)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//有效数据长度8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1bit停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不要校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不需要硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

	USART_Init(USART1, &USART_InitStructure);
	
	// 串口中断优先级配置，内容见上面NVIC_Configuration代码
	NVIC_Configuration();
	
	// 使能串口接收中断,串口1有数据过来就会跳转到中断函数运行代码
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断	
	// 使能串口
	USART_Cmd(USART1, ENABLE);                  
  // 清除发送完成标志
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

//串口的中断服务函数 只要串口1有数据接收到  就跳转到这里来
void USART1_IRQHandler(void)
{//先读取并判断串口的接收数据中断标志位有没有触发 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)      //检查指定的 USART1 中断发生与否
		{ 
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //清除 USART1 的中断待处理位
		}
}


