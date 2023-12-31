#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed = 0;

int main(void)
{
	OLED_Init();
	Timer_Init();	//	TIM2
	Encoder_Init();	//	TIM3
	OLED_ShowString( 1, 1, "Speed:");

	while(1)
	{
		OLED_ShowSignedNum( 1, 7, Speed, 5);
	}
}

void TIM2_IRQHandler(void)
{
	if(SET == TIM_GetITStatus( TIM2, TIM_IT_Update))
	{
		Speed = Encoder_Get();
		TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	}
}
