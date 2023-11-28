#include "IC.h"
#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	使能GPIOA
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);	// 使能TIM3的时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//	配置GPIO口为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//	TIM3_CH1对应PA6引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig( TIM3);	//	选择内部时钟模式驱动时基单元
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	RepetitionCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	//	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	//	配置输入捕获的滤波器
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，选择上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	//	输入捕获不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//	选择直连通道
	TIM_ICInit( TIM3, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger( TIM3, TIM_TS_TI1FP1);	//	配置 TRGI触发源为TI1FP1
	TIM_SelectSlaveMode( TIM3, TIM_SlaveMode_Reset);	//	选择从模式为Reset，清零CNT
	
	TIM_Cmd( TIM3, ENABLE);
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1( TIM3) + 1);
}
