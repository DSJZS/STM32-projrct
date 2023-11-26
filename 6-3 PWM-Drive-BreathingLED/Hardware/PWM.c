#include "PWM.h"
#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	使能GPIOA
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);	// 使能TIM2的时钟
	
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);	// 使能AFIO的时钟
//	GPIO_PinRemapConfig( GPIO_PartialRemap1_TIM2, ENABLE);	//	使能TIM2的部分重映射，
															//	使CH1的端口从PA0 -> PA15
//	GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//	解除JTAG调式端口复用
																//	(为了使PA15能被CH1使用)
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//	配置GPIO口为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//	TIM2_CH1对应PA0引脚
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//	TIM2_CH1重映射后对应PA15引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig( TIM2);	//	选择内部时钟模式驱动时基单元
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	RepetitionCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);	//	为输出比较初始化结构体赋初值，避免无关变量造成干扰
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//	设置输出比较模式为PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//	配置高极性，有效电平为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//	比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;	//	CCR捕获/比较寄存器的值
	TIM_OC1Init( TIM2, &TIM_OCInitStructure);	//	初始化输出比较通道1
	
	TIM_Cmd( TIM2, ENABLE);	//	使能计数器
}

void PWM_SetCpmpare1(uint16_t Compare)
{
	TIM_SetCompare1( TIM2, Compare);
}
