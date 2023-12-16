#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
	uint32_t Alarm = 0;
	
	OLED_Init();	
	MyRTC_Init();

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);	//	使能PWR的时钟
	PWR_WakeUpPinCmd(ENABLE);	// 使能位于PA0的WKUP引脚
	
	OLED_ShowString( 1, 1, "CNT :");	//	显示秒计数器
	OLED_ShowString( 2, 1, "ALR :");	//	显示闹钟值
	OLED_ShowString( 3, 1, "ALRF:");	//	显示闹钟标志位
	
	Alarm = RTC_GetCounter() + 10;		//	设置闹钟值为当前时间十秒后
	RTC_SetAlarm(Alarm);	
	OLED_ShowNum( 2, 6, Alarm, 10);
	
	while(1)
	{
		OLED_ShowNum( 1, 6, RTC_GetCounter(), 10);
		OLED_ShowNum( 3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);
		
		OLED_ShowString( 4, 9, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString( 4, 9, "       ");
		Delay_ms(100);
		
		OLED_Clear();
		
		PWR_EnterSTANDBYMode();	//	进入待机模式
	}
}
