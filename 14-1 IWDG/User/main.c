#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"


int main(void)
{
	OLED_Init();
	Key_Init();
	
	OLED_ShowString( 1, 1, "IWDG TEST");
	
	if(SET == RCC_GetFlagStatus(RCC_FLAG_IWDGRST))	//	判断是不是独立看门狗复位
	{
		OLED_ShowString( 2, 1, "IWDGRST");
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
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//	解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_16);	//	写(设置)预分频器，这里设置为16分频
	IWDG_SetReload(2499);	// 写(设置)重装值
	//	超时时间：TIWDG = 0.025ms × 16 × (2499 + 1) = 1000ms = 1s
	
	IWDG_ReloadCounter();	// 重装载计数器(喂狗)，确保第一个周期为1s
	IWDG_Enable();	//	启动独立看门狗(独立看门狗启动后无法关闭)

	while(1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();	// 重装载计数器(喂狗)
		
		OLED_ShowString( 4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString( 4, 1, "    ");
		Delay_ms(600);
	}
}
