/***************************************************************************************
  * 本源文件含有PID运算相关函数
  ***************************************************************************************
  */

#include "PID.h"
#include <math.h>
#include "stm32f10x.h"





/*功能函数*********************/

/**
  * 函    数：PID参数限制
  * 参    数：num      需要进行比较的某个变量的指针
  * 参    数：limit    需要进行比较的某个变量的最大(小)值
  * 返 回 值：无
  * 说    明：将某个变量与其最大(小)值进行比较，如果其大(小)于其最大(小)值，则使其等于其最大(小)值
  */
static void PID_Limit(int32_t* num,uint32_t limit)
{
    if( ( (*num) > 0 ) && ( (*num) > limit ) )  //  如果为正数，且大于最大值
    {
        *num = limit;   //  使其等于最大值
    }
    else if( ( *num < 0 ) && -( *num ) > limit )//  如果为负数，且小于最小值
    {
        *num = -limit;  //  使其等于最小值
    }
}

/**
  * 函    数：PID初始化
  * 参    数：pid      需要初始化的PID结构体
  * 返 回 值：无
  * 说    明：为PID结构体的所有参数初始化
  */
void PID_Init(PID* pid)
{
    pid->error = 0.0f;                  //  初始化本次误差为0
    pid->last_error = 0.0f;             //  初始化上次误差为0
    pid->target = 0.0f;                 //  初始化目标值为0
    pid->current = 0.0f;                //  初始化当前值为0
    pid->kp = 0.0f;                     //  初始化Kp参数为0
    pid->ki = 0.0f;                     //  初始化Ki参数为0
    pid->kd = 0.0f;                     //  初始化Kd参数为0
    pid->p_out = 0;                     //  初始化比例输出为0
    pid->i_out = 0;                     //  初始化积分输出为0
    pid->d_out = 0;                     //  初始化微分输出为0
    pid->total_out = 0;                 //  初始总输出为0
    pid->i_limit = 4294967295;          //  初始化积分限幅参数为4294967295
    pid->i_band = 4294967295;           //  初始化积分分离参数为4294967295
    pid->total_out_limit = 0;           //  初始化总输出最大值为0
}

/**
  * 函    数：PID运算
  * 参    数：pid      需要运算的PID结构体
  * 返 回 值：无
  * 说    明：通过PID的结构体的各个参数得出最终的结果，赋值到 total_out 成员变量上
  */
void PID_Calc(PID* pid)
{
    pid->error = pid->target - pid->current;            //  得出本次误差
    
    pid->p_out = (int32_t)( pid->kp * pid->error );     //  通过本次误差和上次误差得出比例输出
    
    if( fabsf(pid->error) < pid->i_band)                //  判断是否要进行积分分离
    {
        pid->i_out += (int32_t)( pid->ki * pid->error );//  积分累计误差
        
        PID_Limit( &(pid->i_out), pid->i_limit );       //  判断是否要进行积分限幅
    }
    else                                                //  如果要进行积分分离
    {
        pid->i_out = 0;                                 //  清零积分输出
    }
    
    pid->d_out = (int32_t)( pid->kd * ( pid->error - pid->last_error ) );   //  通过本次误差和上次误差得出微分输出
    
    pid->total_out = pid->p_out + pid->i_out + pid->d_out;                  //  通过比例输出、积分输出、微分输出得出总输出
    
    PID_Limit( &(pid->total_out), pid->total_out_limit );                   //  判断总输出是否过大，对其进行限制
    
    pid->last_error = pid->error;                                           //  将本次输出保存到上次输出变量以供下一次运算
}

/*********************功能函数*/
