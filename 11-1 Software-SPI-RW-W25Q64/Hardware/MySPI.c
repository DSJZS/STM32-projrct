#include "MySPI.h"
#include "stm32f10x.h"                  // Device header

/**
  *@brief		写SS
  *@param		BitValue	写入的值
  *@retval		无
  **/
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

/**
  *@brief		写SCK
  *@param		BitValue	写入的值
  *@retval		无
  **/
void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

/**
  *@brief		写MOSI
  *@param		BitValue	写入的值
  *@retval		无
  **/
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

/**
  *@brief		读MISO
  *@param		无
  *@retval		读出的值
  **/
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6);
}

/**
  *@brief		软件SPI初始化
  *@param		无
  *@retval		无
  **/
void MySPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//	输出引脚为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//	输入引脚为上拉输入(或者浮空输入)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	MySPI_W_SS(1);	//	SS置高电平，默认不选中从机
	MySPI_W_SCK(0);	//	SPI模式0下，空闲状态下SCK为低电平
}

/**
  *@brief		软件SPI起始条件
  *@param		无
  *@retval		无
  **/
void MySPI_Start(void)
{
	MySPI_W_SS(0);	//	SS置低电平，表示选中从机
}

/**
  *@brief		软件SPI终止条件
  *@param		无
  *@retval		无
  **/
void MySPI_Stop(void)
{
	MySPI_W_SS(1);	//	SS置高电平，表示不选中从机
}

/**
  *@brief		软件SPI交换字节
  *@param		ByteSend	发送的字节
  *@retval		接收的字节数据
  **/
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByreReceive = 0;	//	存放接收的字节数据
	uint8_t i = 0;	//	循环计数变量
	
	for(i=0;i<8;++i)
	{
		MySPI_W_MOSI( ByteSend & (0x80>>i) );	//	移出数据
		MySPI_W_SCK(1);	//	拉高电平，形成上升沿
		if(	1 == MySPI_R_MISO() ){	ByreReceive |= (0x80>>i); }	//	移入数据
		MySPI_W_SCK(0);	//	拉低电平，形成下降沿
	}
	
	return ByreReceive;
}
