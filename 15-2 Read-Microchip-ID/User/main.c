#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

uint8_t KeyNum = 0;

int main(void)
{
	OLED_Init();
	
	OLED_ShowString( 1, 1, "F_SIZE:");
	OLED_ShowHexNum( 1, 8, *((__IO uint16_t *)(0x1FFFF7E0)), 4);//	读取闪存容量寄存器获取FLASH容量
	
	OLED_ShowString( 2, 1, "U_ID:");
	OLED_ShowHexNum( 2, 6, *((__IO uint16_t *)( 0x1FFFF7E8+0x00)),4);//	读取产品唯一身份标识寄存器获取芯片ID第一个半字
	OLED_ShowHexNum( 2,11, *((__IO uint16_t *)( 0x1FFFF7E8+0x02)),4);//	读取产品唯一身份标识寄存器获取芯片ID第二个半字
	OLED_ShowHexNum( 3, 1, *((__IO uint32_t *)( 0x1FFFF7E8+0x04)),8);//	读取产品唯一身份标识寄存器获取芯片ID第三、四个半字
	OLED_ShowHexNum( 4, 1, *((__IO uint32_t *)( 0x1FFFF7E8+0x08)),8);//	读取产品唯一身份标识寄存器获取芯片ID第五、六个半字
	while(1)
	{
		
	}
}
