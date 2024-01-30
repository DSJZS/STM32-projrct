#ifndef __PID_H
#define	__PID_H

#include "stm32f10x.h"





/*结构体定义*********************/

/*  PID结构体，用于存放进行PID运算所要用到的所有参数 */
typedef struct PID{
    float error;            //  本次误差
    float last_error;       //  上次误差
    float target;           //  目标值
    float current;          //  当前值
    float kp;               //  Kp参数
    float ki;               //  Ki参数
    float kd;               //  Kd参数
    int32_t p_out;          //  比例输出
    int32_t i_out;          //  积分输出
    int32_t d_out;          //  微分输出
    int32_t total_out;      //  总输出
    uint32_t i_limit;       //  积分限幅
    uint32_t i_band;        //  积分分离
    int32_t total_out_limit;//  总输出最大值
}PID;

/*********************结构体定义*/





/*函数声明*********************/

void PID_Init(PID* pid);
void PID_Calc(PID* pid);

/*********************函数声明*/





#endif
