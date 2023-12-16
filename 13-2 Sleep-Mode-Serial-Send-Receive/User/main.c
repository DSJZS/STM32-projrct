#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t Serial_RxData = 0;	//	存放接收的数据

int main(void)
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1,1,"RxData:");
	while(1)
	{
		if(Serial_GetRxFlag())
		{
			Serial_RxData = Serial_GetRxData();
			Serial_SendByte(Serial_RxData);
			OLED_ShowHexNum(1,8,Serial_RxData,2);
		}
		
		OLED_ShowString( 2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString( 2, 1, "       ");
		Delay_ms(100);
		
		__WFI();	//	WFI指令，默认为立刻睡眠，进入睡眠模式
	}
}
