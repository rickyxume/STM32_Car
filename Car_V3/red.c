#include "red.h"

	
void Red_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟
	
	//配置电机引脚
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//选择对应的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB端口
	

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟
	
	//配置红外引脚11
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 ;	//选择对应的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//配置GPIO模式,浮空输入       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PC端口
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PA端口时钟
	
	//配置红外引脚10
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;	//选择对应的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//配置GPIO模式,浮空输入       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口

}

//读取红外的信号LR
//延时的同时检测红外，一旦发生障碍物，就停止并跳出延时
void DelayCheck(int ms)
{
	while(ms--)
	{
		delay_ms(1); //延时1ms
		if(L==0 || R==0) //一有一边检测到障碍物就停下来
		{
			CarStop();
			return;
		}
	}
}

void Red_Avoidance()
{
	if(L==0){//如果L返回低电平，则检测到左边有障碍物
		do{
			CarStop();delay_ms(100);
			CarBackRight(900);delay_ms(200);CarRight(999);delay_ms(100);DelayCheck(500);
		}while(L==0);//一直后退+右转到左边没有障碍物
						
	}
	if(R==0){//如果R返回低电平，则检测到右边有障碍物
		do{
			CarStop();delay_ms(100);
			CarBackLeft(900);delay_ms(200);CarLeft(999);delay_ms(100);DelayCheck(500);
		}while(R==0);//一直后退+左转到右边没有障碍物
	}
	if(R==0&&L==0){
		CarStop();delay_ms(100);
		CarBack(900);delay_ms(500);
		CarRight(999);DelayCheck(500);
	}
	if(R&&L){ //两边都没检测到障碍物
		CarGo(700);//往前
		delay_ms(200);
		DelayCheck(300);
	}
}