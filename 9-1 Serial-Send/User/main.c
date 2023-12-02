#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t MyArray[] = { 0x42, 0x43, 0x44, 0x45};

int main(void)
{
	OLED_Init();
	Serial_Init();
	
	Serial_SendByte('A');
	Serial_SendArray( MyArray, sizeof(MyArray)/sizeof(MyArray[0]));
	Serial_SendNumber( 12345, 5);	
	Serial_SendString("Hello World!!!\r\n");
	Serial_Printf("测试->\r\nNum=%d\r\n",66);
	
	while(1)
	{
		
	}
}
