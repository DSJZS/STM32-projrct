#include "Timer.h"
#include "stm32f10x.h"                  // Device header

void Timer_Init(void)	//	T = 1s
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	使能GPIOA
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);	// 使能TIM2的时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//	配置GPIO口为上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//	TIM2_ETR对应PA0引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	TIM_ETRClockMode2Config( TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);	
	//	选择外部时钟模式2驱动时基单元，不分频，
	//	并且触发的极性不反向(高电平或者上升沿有效)，滤波采样频率为 0x0F
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	RepetitionCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);
	TIM_ClearFlag( TIM2, TIM_IT_Update);// 由于TIM_TimeBaseInit会生成一个更新事件。
	//				来重新装载预分频器和重复计数器的值
	//				并且更新事件和更新中断是同时产生，所以会产生更新中断并且置更新中断标志位
	//				初始化函数完后便会立刻进入中断，这是不好的，所以要在TIM_TimeBaseInit之后清除标志位

	
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);	//	使能更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	配置NVIC中断分组
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//	指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//	配置指定中断通道使能状态
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//	配置指定抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//	配置指定响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd( TIM2, ENABLE);	//	使能计数器
}

uint16_t Timer_GetCount(void)
{
	return TIM_GetCounter( TIM2);
}

/*	定时器2更新中断函数模板
void TIM2_IRQHandler(void)
{
	if(SET == TIM_GetITStatus( TIM2, TIM_IT_Update))
	{
		TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	}
}
*/
