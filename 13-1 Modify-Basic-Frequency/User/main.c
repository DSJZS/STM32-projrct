#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

uint8_t KeyNum = 0;

/*
在system_stm32f10x.c中
启用	SYSCLK_FREQ_36MHz
而非	SYSCLK_FREQ_72MHz
*/

int main(void)
{
	OLED_Init();
	OLED_ShowString( 1, 1, "SYSCLK:");
	OLED_ShowNum( 1, 8, SystemCoreClock, 8);

	while(1)
	{
		OLED_ShowString( 2, 1, "Running");
		Delay_ms(500);
		OLED_ShowString( 2, 1, "       ");
		Delay_ms(500);
	}
}
