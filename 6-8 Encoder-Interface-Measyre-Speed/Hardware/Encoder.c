#include "Encoder.h"
#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
												//(但是没用，因为计数方向现在由编码器接口托管)
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	RepetitionCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure);
	//	配置通道1
	TIM_ICStructInit( &TIM_ICInitStructure);	//	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	//	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	//	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
															//编码器上升沿和下降沿均有效
															//此处Rising表示高低电平极性不反转
															//和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	//	配置通道2
	TIM_ICInit( TIM3, &TIM_ICInitStructure);
	
	TIM_ICStructInit( &TIM_ICInitStructure);	//	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;	//	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	//	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
															//编码器上升沿和下降沿均有效
															//此处Rising表示高低电平极性不反转
															//和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	TIM_ICInit( TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig( TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//	配置编码器接口，在TI1和TI2上计数，且极性均不反向
	
	TIM_Cmd( TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp = (int16_t)(TIM_GetCounter( TIM3));
	TIM_SetCounter( TIM3, 0);
	return Temp;
}
