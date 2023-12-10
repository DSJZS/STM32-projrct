#include "MPU6050.h"
#include "stm32f10x.h"                  // Device header
//#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADRESS 0xD0

/**
  *@brief		具有超时退出机制的I2C_CheckEvent
  *@param		I2C_CheckEvent的参数
  *@retval		无
  **/
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;	//	自定义计数次数
	while(SUCCESS != I2C_CheckEvent( I2Cx, I2C_EVENT))
	{
		--Timeout;
		if(Timeout == 0)	//	循环对应次数后不再等待，直接退出
		{
			/*	此处可添加错误处理	*/
			break;
		}
	}
}


/**
  *@brief		指定地址写寄存器
  *@param		RedAdress	寄存器地址
  *@param		Data	要写入的数据
  *@retval		
  **/
void MPU6050_WriteReg( uint8_t RedAdress, uint8_t Data)
{
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RedAdress);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(Data);	//*	可循环这两句代码发送多个字节的数据	
//	MyI2C_ReceiveAck();		//*
//	MyI2C_Stop();
	
	I2C_GenerateSTART( I2C2, ENABLE);	//	生成起始条件
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT);	//	等待起始条件生成完	EV5
	
	I2C_Send7bitAddress( I2C2, MPU6050_ADRESS, I2C_Direction_Transmitter);	//	发送从机地址(发送)
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//	等待从机地址发送完成	EV6
	
	I2C_SendData( I2C2, RedAdress);
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//	等待寄存器地址发送完成	EV8
	
	I2C_SendData( I2C2, Data);
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//	等待字节发送完成	EV8_2
	I2C_GenerateSTOP( I2C2, ENABLE);	//	生成终止条件
}

/**
  *@brief		指定地址读寄存器
  *@param		RedAdress	寄存器地址
  *@retval		读出的数据
  **/
uint8_t MPU6050_ReadReg( uint8_t RedAdress)
{
	uint8_t Data;
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RedAdress);
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADRESS|0x01);
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();	//*	可循环这两句代码，接收多个字节的数据	
//	MyI2C_SendAck(1);			//* 如果要接收多个字节的数据，此处需应答0
//	MyI2C_Stop();
	
	I2C_GenerateSTART( I2C2, ENABLE);	//	生成起始条件
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT);	//	等待起始条件生成完	EV5
	
	I2C_Send7bitAddress( I2C2, MPU6050_ADRESS, I2C_Direction_Transmitter);	//	发送从机地址(发送)
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//	等待从机地址发送完成	EV6
	
	I2C_SendData( I2C2, RedAdress);
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//	等待寄存器地址发送完成	EV8_2
	
	I2C_GenerateSTART( I2C2, ENABLE);	//	生成起始条件
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT);	//	等待起始条件生成完	EV5
	
	I2C_Send7bitAddress( I2C2, MPU6050_ADRESS, I2C_Direction_Receiver);	//	发送从机地址(接收)
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//	等待从机地址发送完成	EV6
	
	I2C_AcknowledgeConfig( I2C2, DISABLE);	//	在接收最后一个字节前设置不应答
	I2C_GenerateSTOP( I2C2, ENABLE);	//	在接收最后一个字节前配置停止位
	
	MPU6050_WaitEvent( I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//	等待字节接收完成	EV7
	Data = I2C_ReceiveData( I2C2);	//	接收字节数据
	
	I2C_AcknowledgeConfig( I2C2, ENABLE);	//	恢复默认的应答设置
	return Data;
}

/**
  *@brief		MPU6050初始化
  *@param		无
  *@retval		无
  **/
void MPU6050_Init(void)
{
//	MyI2C_Init();
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE);	//	使能I2C2时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);	//	使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	//	复用开头输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	//	选择I2C模式
	I2C_InitStructure.I2C_ClockSpeed = 50000;	//	配置SCL时钟频率为50k
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//	配置时钟占空比为2:1，由于此时为标准速度，所以无用
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	//	配置是否应答，此处为应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//	配置从机模式下响应地址位数，这里设置为7位
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;	//	配置从机模式下地址，这里为0x00
	I2C_Init( I2C2, &I2C_InitStructure);
	
	I2C_Cmd( I2C2, ENABLE);	//	使能I2C2
	
	MPU6050_WriteReg( MPU6050_PWR_MGMT_1, 0x01);	//	电源配置
	MPU6050_WriteReg( MPU6050_PWR_MGMT_2, 0x00);	//	
	MPU6050_WriteReg( MPU6050_SMPLRT_DIV, 0x09);	//	采样分配配置
	MPU6050_WriteReg( MPU6050_CONFIG, 0x06);		//	配置寄存器
	MPU6050_WriteReg( MPU6050_GYRO_CONFIG, 0x18);	//	配置陀螺仪
	MPU6050_WriteReg( MPU6050_ACCEL_CONFIG, 0x18);	//	配置加速度计
	
}

/**
  *@brief		获取MPU6050的ID号
  *@param		无
  *@retval		MPU6050的ID号
  **/
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
  *@brief		获取MPU6050六轴的数据
  *@param		AccX、AccY、AccZ、GyroX、GyroY、GyroZ		六周的指针
  *@retval		无
  **/
void MPU6050_GetData( int16_t *AccX, int16_t *AccY, int16_t *AccZ,
					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH,DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH<<8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH<<8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH<<8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH<<8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH<<8)|DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH<<8)|DataL;
}
