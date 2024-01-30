#ifndef __MPU6050_H
#define __MPU6050_H

#include <stdio.h>

/*宏函数定义*********************/

#define MPU6050_ERROR(fmt,arg...)          printf("MPU6050_ERROR"fmt"\n",##arg) //  打印错误报告
#define MPU6050_INFO(fmt,arg...)          printf("MPU6050_INFO"fmt"\n",##arg)   //  打印信息报告

/*********************宏函数定义*/

/*函数声明*********************/

void MPU6050_DMP_Init(void);
int MPU6050_DMP_Get_Data( float* pitch, float* roll, float* yaw);

/*********************函数声明*/

#endif
