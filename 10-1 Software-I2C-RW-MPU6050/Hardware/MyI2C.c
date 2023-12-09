#include "MyI2C.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  *@brief		写SCL
  *@param		BitValue	写入的值
  *@retval		无
  **/
static void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);	//	根据单片机种类设置参数
}

/**
  *@brief		写SDA
  *@param		BitValue	写入的值
  *@retval		无
  **/
static void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);	//	根据单片机种类设置参数
}

/**
  *@brief		读SDA
  *@param		无
  *@retval		读取的值
  **/
static uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue = 0;
	BitValue = GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_11);
	Delay_us(10);	//	根据单片机种类设置参数
	return BitValue;
}

/**
  *@brief		软件I2C初始化
  *@param		无
  *@retval		无
  **/
void MyI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits( GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

/**
  *@brief		软件I2C起始条件
  *@param		无
  *@retval		无
  **/
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);	//	先确保SDA为高电平
	MyI2C_W_SCL(1);	//	之后再拉高SCL，避免SCL先拉高之后SDA可能由低到高被误判为终止条件
	MyI2C_W_SDA(0);	//	拉低SDA，触发起始条件
	MyI2C_W_SCL(0);	//	拉低SCL，便于连接后续时序
}

/**
  *@brief		软件I2C终止条件
  *@param		无
  *@retval		无
  **/
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);	//	确保SDA一开始为低电平，为后续上升沿做准备
	MyI2C_W_SCL(1);	//	之后再拉高SCL
	MyI2C_W_SDA(1);	//	拉高SDA，触发终止条件
	
}

/**
  *@brief		软件I2C发送一个字节
  *@param		Byte	待发送的字节
  *@retval		无
  **/
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i = 0;
	for(i=0;i<8;++i)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));	//	低电平时将数据放到SDA
		MyI2C_W_SCL(1);	//	拉高SCL，使从机读取SDA数据
		MyI2C_W_SCL(0);	//	拉低SCL
	}
}

/**
  *@brief		软件I2C接收一个字节
  *@param		无
  *@retval		接收的字节
  **/
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i=0,Byte=0x00;
	MyI2C_W_SDA(1);	//	主机释放SDA
	for(i=0;i<8;++i)
	{
		MyI2C_W_SCL(1);	//	拉高SCL接收数据
		if(MyI2C_R_SDA() == 1){Byte |= (0x80>>i);}	//	读取SDA上的数据
		MyI2C_W_SCL(0);	//	拉低SCL
	}
	return Byte;
}

/**
  *@brief		软件I2C发送应答
  *@param		AckBit	待发送的应答
  *@retval		无
  **/
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);	//	低电平时将数据放到SDA
	MyI2C_W_SCL(1);	//	拉高SCL，使从机读取SDA数据
	MyI2C_W_SCL(0);	//	拉低SCL
}

/**
  *@brief		软件I2C接收应答
  *@param		无
  *@retval		接收的应答
  **/
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit=0x00;
	MyI2C_W_SDA(1);	//	主机释放SDA

	MyI2C_W_SCL(1);	//	拉高SCL接收数据
	AckBit = MyI2C_R_SDA();	//	读取SDA上的数据
	MyI2C_W_SCL(0);	//	拉低SCL

	return AckBit;
}
