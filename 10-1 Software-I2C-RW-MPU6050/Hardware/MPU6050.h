#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "stm32f10x.h"                  // Device header

void MPU6050_WriteReg( uint8_t RedAdress, uint8_t Data);
uint8_t MPU6050_ReadReg( uint8_t RedAdress);
void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData( int16_t *AccX, int16_t *AccY, int16_t *AccZ,
					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
	
#endif
