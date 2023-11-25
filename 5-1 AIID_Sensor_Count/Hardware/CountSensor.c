#include "CountSensor.h"
#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count = 0;

void CountSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//	使能时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
	//	初始化GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	//	配置AFIO
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource14);
	//	配置EXTI
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//	指定中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//	配置指定中断线使能状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//	配置指定中断线模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//	配置指定中断线触发信号的有效边沿
	EXTI_Init(&EXTI_InitStructure);
	//	配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	配置NVIC分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//	指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//	配置指定中断通道使能状态
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//	配置指定抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//	配置指定响应优先级
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}


void EXTI15_10_IRQHandler(void)	//	EXTI15_10
{
	if( SET == EXTI_GetITStatus(EXTI_Line14) )	//	根据中断标志位判断是不是对应的中断源触发中断
	{
		++CountSensor_Count;
		EXTI_ClearITPendingBit(EXTI_Line14);	//	清除中断源标志位，避免重复进入
	}
}
