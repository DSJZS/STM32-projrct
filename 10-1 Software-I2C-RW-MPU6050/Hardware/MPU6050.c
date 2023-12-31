#include "MPU6050.h"
#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADRESS 0xD0

/**
  *@brief		指定地址写寄存器
  *@param		RedAdress	寄存器地址
  *@param		Data	要写入的数据
  *@retval		
  **/
void MPU6050_WriteReg( uint8_t RedAdress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RedAdress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);	//*	可循环这两句代码发送多个字节的数据	
	MyI2C_ReceiveAck();		//*
	MyI2C_Stop();
}

/**
  *@brief		指定地址读寄存器
  *@param		RedAdress	寄存器地址
  *@retval		读出的数据
  **/
uint8_t MPU6050_ReadReg( uint8_t RedAdress)
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RedAdress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADRESS|0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();	//*	可循环这两句代码，接收多个字节的数据	
	MyI2C_SendAck(1);			//* 如果要接收多个字节的数据，此处需应答0
	MyI2C_Stop();
	return Data;
}

/**
  *@brief		MPU6050初始化
  *@param		无
  *@retval		无
  **/
void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg( MPU6050_PWR_MGMT_1, 0x00);	//	电源配置
	MPU6050_WriteReg( MPU6050_PWR_MGMT_2, 0x00);	//	
	MPU6050_WriteReg( MPU6050_SMPLRT_DIV, 0x07);	//	采样分配配置
	MPU6050_WriteReg( MPU6050_CONFIG, 0x06);		//	配置寄存器
	MPU6050_WriteReg( MPU6050_GYRO_CONFIG, 0x18);	//	配置陀螺仪
	MPU6050_WriteReg( MPU6050_ACCEL_CONFIG, 0x01);	//	配置加速度计
	
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
