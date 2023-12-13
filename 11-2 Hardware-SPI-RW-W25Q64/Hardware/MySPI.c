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
  *@brief		软件SPI初始化
  *@param		无
  *@retval		无
  **/
void MySPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);	//	使能SPI1
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//	输出引脚为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//	PA4为软件模拟的SS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//	输出引脚为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;	//	PA5是SPI1的SCK，而PA7是MOSI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//	输出引脚为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//	PA6是SPI1的MISO
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//	选中SPI模式，这里为主机模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//	选中双线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//	配置8位数据帧
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//	高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//	配置分频系数为，得出SCK频率562,500Hz
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//	时钟极性，选择空闲时低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//	时钟相位，选择第一个边沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//	NSS引脚选择软件模式
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//	CRC校验多项式，这里为默认值7
	SPI_Init( SPI1, &SPI_InitStructure);
	
	SPI_Cmd( SPI1, ENABLE);	//	使能SPI1
	
	MySPI_W_SS(1);	//	SS引脚默认输出高电平不选中从机
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
	while(SET != SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_TXE));	//	等待发送数据寄存器(缓冲区)为空
	SPI_I2S_SendData( SPI1, ByteSend);	//	SPI1发送ByteSend到从机，并清除TXE标志位
	while(SET != SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE));	//	等待接收数据寄存器(缓冲区)非空
	return SPI_I2S_ReceiveData(SPI1);	//	返回接收的数据，并清除RXNE标志位
}
