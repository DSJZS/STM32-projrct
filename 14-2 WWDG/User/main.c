#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"


int main(void)
{
	OLED_Init();
	Key_Init();
	
	OLED_ShowString( 1, 1, "WWDG TEST");
	
	if(SET == RCC_GetFlagStatus(RCC_FLAG_WWDGRST))	//	判断是不是窗口看门狗复位
	{
		OLED_ShowString( 2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString( 2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();	//	清除标志位
	}
	else
	{
		OLED_ShowString( 3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString( 3, 1, "   ");
		Delay_ms(100);
	}
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_WWDG, ENABLE);	//	使能窗口看门狗时钟
	WWDG_SetPrescaler(WWDG_Prescaler_8);	// 写(设置)预分频器，这里选择8分频
	WWDG_SetWindowValue(21 | 0x40);	// 写(设置)窗口值
	WWDG_Enable(54 | 0x40);	// 使能(启动)窗口看门狗(同时会“喂狗”避免一启动就复位)
	
//	超时时间：
//	TWWDG = 1/36M × 4096 × 8 × (54 + 1) = 0.05006s = 50.06ms
	
//	窗口时间：
//	TWIN = 1/36M × 4096 × 8 × (54 - 21) = 0.03004s = 30.04ms
	
	while(1)
	{
		Key_GetNum();
		
		OLED_ShowString( 4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString( 4, 1, "    ");
		Delay_ms(20);
		
		WWDG_SetCounter(54 | 0x40);	// 写(设置)计数器(喂狗)
	}
}
