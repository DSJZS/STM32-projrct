#include "Serial.h"
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPacket[100] = {0};	//	接收数据缓冲区
uint8_t Serial_RxFlag = 0;	//	接收标志位

/**
  *@brief		串口(发送和接收)初始化
  *@param		无
  *@retval		无
  **/
void Serial_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);	//	使能USART1的时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	使能GPIOA的时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//	TX引脚选择复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//	USART1_TX 是 PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//	RX引脚选择复用上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//	USART1_RX 是 PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;	//	波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//	不使用流控
//	USART_InitStructure.USART_Mode = USART_Mode_Tx;	//	开启发送功能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//	开启发送和接收功能
	USART_InitStructure.USART_Parity = USART_Parity_No;	//	不校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//	选择一位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//	无需校验，所以选择8位字长
	USART_Init( USART1, &USART_InitStructure);
	
	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE);	//	开启USART1的接收中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	配置NVIC分组为分组2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//	指定USART1通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//	开启USART1通道中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//	配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//	配置响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd( USART1, ENABLE);
}

/**
  *@brief		串口发送一字节数据
  *@param		Byte	要发送的字节数据
  *@retval		无
  **/
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData( USART1, Byte);	//	将Byte写入DR发送(同时会清除TXE标志位)
	while( RESET == USART_GetFlagStatus( USART1, USART_FLAG_TXE) );	//	根据标志位等待数据发送完毕
}

/**
  *@brief		串口发送一个由一字节数据构成的数组
  *@param		Array	数组首元素地址
  *@param		Length	数组长度
  *@retval		无
  **/
void Serial_SendArray( uint8_t* Array, uint16_t Length)
{
	uint16_t i = 0;
	for(i=0;i<Length;++i)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
  *@brief		串口发送字符串
  *@param		String	字符串首元素地址	
  *@retval		
  **/
void Serial_SendString( char* String)
{
	uint16_t i = 0;
	for(i=0; '\0' != String[i] ; ++i)
	{
		Serial_SendByte(String[i]);
	}
}

/**
  *@brief		次方计算
  *@param		X	幂
  *@param		Y	指数
  *@retval		X^Y
  **/
static uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}
/**
  *@brief		串口发送一个无符号数字
  *@param		Number	要发送的数字
  *@param		Length	数字位数
  *@retval		无
  **/
void Serial_SendNumber( uint32_t Number, uint8_t Length)
{
	while(Length)
	{
		Serial_SendByte( Number / Serial_Pow( 10, Length-1) %10 + '0');	//	由于要显示字符，需要加上'0'偏移
		--Length;
	}
}

/**
  *@brief		串口的格式化发送
  *@param		format	格式化字符串
  *@param		...		可变参数列表
  *@retval		无
  **/
void Serial_Printf(char* format, ...)
{
	char String[100];
	va_list arg;	//	定义参数列表变量
	va_start( arg, format);	//	从format处接收参数表并放入arg
	vsprintf( String, format, arg);	//	格式化输入
	va_end(arg);	//	释放参数表
	Serial_SendString(String);
}

///**
//  *@brief		将发送数据缓冲区里的数据加上包头包尾构成数据包并发送出去
//  *@param		无
//  *@retval		无
//  **/
//void Serial_SendPacket(void)
//{
//	Serial_SendByte(0xFF);
//	Serial_SendArray( Serial_TxPacket, 4);
//	Serial_SendByte(0xFE);
//}

///**
//  *@brief		获取Rx标志，并将其清空
//  *@param		无
//  *@retval		Serial_RxFlag的值
//  **/
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	return 0;
//}

/**
  *@brief		USART1中断函数
  *@param		无
  *@retval		无
  **/
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;	//	状态位
	static uint8_t pRxPacket = 0;	//	载荷数据读取次数
	if(SET == USART_GetITStatus( USART1, USART_IT_RXNE))
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		if(RxState == 0)	//	包头
		{
			if(RxData == '@' && Serial_RxFlag == 0)	//	
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)	//	载荷
		{
			if(RxData == '\r')
			{	
				RxState = 2;
			}
			else
			{
				Serial_RxPacket[pRxPacket] = RxData;
				++pRxPacket;
			}
		}
		else if(RxState == 2)	//	包尾
		{
			if(RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';	//	添加上字符串结束标志
				Serial_RxFlag = 1;
			}
		}
		USART_ClearITPendingBit( USART1, USART_IT_RXNE);
	}
}
