#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	OLED_Init();
	CountSensor_Init();
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);	//	使能电源控制外设
	
	OLED_ShowString(1,1,"Count:");
	while(1)
	{
		OLED_ShowNum( 1, 7, CountSensor_Get(), 5);
		
		OLED_ShowString( 2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString( 2, 1, "       ");
		Delay_ms(100);
		
		PWR_EnterSTOPMode( PWR_Regulator_ON, PWR_STOPEntry_WFI);	//	进入停止模式(开启电源调节器，WFI指令进入)
		SystemInit();	//	重新启动HSE，配置72MHz主频
	}
}
