#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//	使能GPIOA的时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//	选择推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//	选择0号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//	选择550MHz速度
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay_ms(500);
	}
}
