#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//	使能GPIOA的时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//	选择推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			//	选择所有引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//	选择550MHz速度
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while(1)
	{
		int i = 0;
		for(i=0;i<8;++i)
		{
			GPIO_Write(GPIOA,~(0x0001<<i));
			Delay_ms(500);
		}
	}
}
