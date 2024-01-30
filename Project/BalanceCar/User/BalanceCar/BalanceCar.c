/***************************************************************************************
  * 本源文件含有平衡小车运行的相关函数
  * 小车参数等可在对应的地方自定义
  ***************************************************************************************
  */

#include "BalanceCar.h"
#include "stm32f10x.h"
#include "../PID/PID.h"
#include "./MG310/MG310.h"
#include "./MPU6050/MPU6050.h"
#include "./Timer/Timer.h"





/*全局变量*********************/

struct BalanceCar BC;       //  平衡小车变量，存放了平衡小车要用到的所有

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：平衡车直立环输出控制函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于获取平衡车当前的角度信息，对其进行PID运算，得出直立环输出的PWM参数
  */
static void BalanceCar_Angle_Control(void)
{
    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    
    if( MPU6050_DMP_Get_Data( &pitch, &roll, &yaw) == 0 )   //  如果成功获取了角度
    {
        BC.angle.current = pitch + BC.angleOffset;          //  设置当前的角度值为获取的实际角度加上机械零点的偏移值
        BC.angle.target = BC.angleSet;                      //  设置目标角度值为0°
        PID_Calc( &BC.angle );                              //  直立环进行PID运算得出直立环输出的PWM参数
    }  
}   

/**
  * 函    数：用于获取平衡车当前的速度信息
  * 参    数：无
  * 返 回 值：无
  * 说    明：会获取当前左右电机编码器的计数并清零定时器的计数，同时对获取的计数值进行单位化存入速度变量
  */
static void BalanceCar_GetSpeed(void)
{
    int16_t Left_Encoder_Count  = MG310_Encoder1_GetCount();        //  获取左侧电机编码器的计数
    int16_t Right_Encoder_Count = MG310_Encoder2_GetCount();        //  获取右侧电机编码器的计数
    
    BC.leftSpeed =   Left_Encoder_Count/26.0f;                      //  左侧电机编码器的计数以26为单位记到左轮速度变量
    BC.rightSpeed = Right_Encoder_Count/26.0f;                      //  右侧电机编码器的计数以26为单位记到右轮速度变量
}

/**
  * 函    数：平衡车速度环和转向环输出控制函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用平衡车当前的速度信息进行PID运算，得出速度环输出的PWM参数
  */
static void BalanceCar_Speed_Control(void)
{
    

    BC.speed.target = BC.speedSet;                                  //  设置目标速度值
    BC.speed.current = ( BC.leftSpeed + BC.rightSpeed );            //  设置当前速度值为左右轮速度变量之和                                      
    PID_Calc( &BC.speed );                                          //  速度环进行PID运算得出速度环输出的PWM参数
}

/**
  * 函    数：平衡车转向环输出控制函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用平衡车当前左右轮之间的速度差进行PID运算，得出转向环输出的PWM参数
  */
static void BalanceCar_Turn_Control(void)
{
    BC.turn.target = BC.turnSet;                                    //  设置目标转向值
    BC.turn.current = ( BC.leftSpeed - BC.rightSpeed );             //  设置当前转向值为左右轮速度变量之差
    PID_Calc( &BC.turn );                                           //  转向环进行PID运算得出转向环输出的PWM参数
}

/**
  * 函    数：平衡车输出控制函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用直立环和速度环进行PID运算得到的PWM参数。设置PWM的占空比，来控制电机的速度以达到平衡
  */
static void BalanceCar_MotorOutput(void)
{
    //  由直立环、速度环、转向环得出最终的数据
    MG310_Motor_SetLeftVelocity( BC.angle.total_out - BC.speed.total_out - BC.turn.total_out );
    MG310_Motor_SetRightVelocity(BC.angle.total_out - BC.speed.total_out + BC.turn.total_out );
}

/**
  * 函    数：设定平衡小车速度
  * 参    数：Speed        速度，根据我调的参数，其绝对值最好为 -100~100，当然这要看实际情况，别的小车肯定会有所不同
  * 返 回 值：无
  * 说    明：运动状态(速度不为0)和静止状态(速度为0)时的PID参数可能不一样
  */
void BalanceCar_SetSpeed(float Speed)
{
    if(Speed)   //  运动时
    {
        BC.angle.kp = DYNAMIC_ANGLE_KP;             //  修改直立环的Kp参数为运动状态的Kp参数，根据实际情况修改
        BC.angle.ki = DYNAMIC_ANGLE_KI;             //  修改直立环的Ki参数为运动状态的Ki参数，根据实际情况修改
        BC.angle.kd = DYNAMIC_ANGLE_KD;             //  修改直立环的Kd参数为运动状态的Kd参数，根据实际情况修改
        
        BC.speed.kp = DYNAMIC_SPEED_KP;             //  修改速度环的Kp参数为运动状态的Kp参数，根据实际情况修改
        BC.speed.ki = DYNAMIC_SPEED_KI;             //  修改速度环的Ki参数为运动状态的Ki参数，根据实际情况修改
        BC.speed.kd = DYNAMIC_SPEED_KD;             //  修改速度环的Ki参数为运动状态的Kd参数，根据实际情况修改
        
        BC.turn.kp = DYNAMIC_TURN_KP;               //  修改转向环的Kp参数为运动状态的Kp参数，根据实际情况修改
        BC.turn.ki = DYNAMIC_TURN_KI;               //  修改转向环的Ki参数为运动状态的Ki参数，根据实际情况修改
        BC.turn.kd = DYNAMIC_TURN_KD;               //  修改转向环的Kd参数为运动状态的Kd参数，根据实际情况修改
    }
    else        //  禁止时
    {
        BC.angle.kp = STATIC_ANGLE_KP;              //  修改直立环的Kp参数为静止状态的Kp参数，根据实际情况修改
        BC.angle.ki = STATIC_ANGLE_KI;              //  修改直立环的Ki参数为静止状态的Ki参数，根据实际情况修改
        BC.angle.kd = STATIC_ANGLE_KD;              //  修改直立环的Kd参数为静止状态的Kd参数，根据实际情况修改
        
        BC.speed.kp = STATIC_SPEED_KP;              //  修改速度环的Kp参数为静止状态的Kp参数，根据实际情况修改
        BC.speed.ki = STATIC_SPEED_KI;              //  修改速度环的Ki参数为静止状态的Ki参数，根据实际情况修改
        BC.speed.kd = STATIC_SPEED_KD;              //  修改速度环的Kd参数为静止状态的Kd参数，根据实际情况修改
        
        BC.turn.kp = STATIC_TURN_KP;                //  修改转向环的Kp参数为静止状态的Kp参数，根据实际情况修改
        BC.turn.ki = STATIC_TURN_KI;                //  修改转向环的Ki参数为静止状态的Ki参数，根据实际情况修改
        BC.turn.kd = STATIC_TURN_KD;                //  修改转向环的Kd参数为静止状态的Kd参数，根据实际情况修改
    }
    BC.speedSet = Speed / 50.0f;                    //  对Speed变量进行单位化，避免其过大，根据实际情况修改
}
    
/**
  * 函    数：设定平衡小车转向
  * 参    数：Turn        转向值，为正数时左转，为负数时右转(当然，这要看实际情况)，根据我调的参数，其绝对值最好不要大于100
  * 返 回 值：无
  * 说    明：修改期间会暂时停止速度环和转向环的控制
  */
void BalanceCar_SetTurn(float Turn)   
{
    BC.turnSet += Turn/50.0f;                    //  对Speed变量进行单位化，避免其过大，根据实际情况修改
}

void BalanceCar_Init(void)
{
    BC.angleOffset = ANGLEOFF;                  //  初始化机械零点偏移值，根据实际情况修改
    BC.angleSet = 0.0f;                         //  初始化设定角度值为0，根据实际情况修改
    BC.speedSet = 0.0f;                         //  初始化设定速度值为0，根据实际情况修改
    BC.turnSet  = 0.0f;                         //  初始化设定转向值为0，根据实际情况修改
    BC.leftSpeed = 0.0f;                        //  初始化左轮速度值为0
    BC.rightSpeed = 0.0f;                       //  初始化右轮速度值为0
    BC.mode = 0;                                //  初始化模式为0，(事实上我当前就实现了一个模式QOQ)
    
    PID_Init( &( BC.angle ) );                  //  初始化直立环
    PID_Init( &( BC.speed ) );                  //  初始化速度环
    PID_Init( &( BC.turn  ) );                  //  初始化转向环
    
    BC.angle.kp = STATIC_ANGLE_KP;              //  初始化直立环的Kp参数，根据实际情况修改
    BC.angle.ki = STATIC_ANGLE_KI;              //  初始化直立环的Ki参数，根据实际情况修改
    BC.angle.kd = STATIC_ANGLE_KD;              //  初始化直立环的Kd参数，根据实际情况修改
    BC.angle.total_out_limit = MOTOR_OUT_MAX*2; //  初始化直立环的最大输出参数，根据实际情况修改
    
    BC.speed.kp = STATIC_SPEED_KP;              //  初始化速度环的Kp参数，根据实际情况修改
    BC.speed.ki = STATIC_SPEED_KI;              //  初始化速度环的Ki参数，根据实际情况修改
    BC.speed.kd = STATIC_SPEED_KD;              //  初始化速度环的Kd参数，根据实际情况修改
//    BC.speed.i_band = 4;                      //  不初始化速度环的积分分离参数，根据实际情况修改
    BC.speed.i_limit = MOTOR_OUT_MAX/4;         //  初始化速度环的积分限幅参数，根据实际情况修改
    BC.speed.total_out_limit = MOTOR_OUT_MAX*2; //  初始化速度环的最大输出参数，根据实际情况修改
    
    BC.turn.kp = STATIC_TURN_KP;                //  初始化转向环的Kp参数，根据实际情况修改
    BC.turn.ki = STATIC_TURN_KI;                //  初始化转向环的Ki参数，根据实际情况修改
    BC.turn.kd = STATIC_TURN_KD;                //  初始化转向环的Kd参数，根据实际情况修改
    BC.turn.total_out_limit = MOTOR_OUT_MAX*2;  //  初始化转向环的最大输出参数，根据实际情况修改
    
#ifdef USE_TIMER    //  如果使用了Timer模块
    Timer_AddFunction(BalanceCar_Angle_Control,      1); //  将 BalanceCar_Angle_Control 函数添加到定时列表，执行周期为 1ms
    Timer_AddFunction(BalanceCar_GetSpeed, 25);          //  将 BalanceCar_GetSpeed      函数添加到定时列表，执行周期为25ms
    Timer_AddFunction(BalanceCar_Turn_Control,  25);     //  将 BalanceCar_Turn_Control  函数添加到定时列表，执行周期为25ms
    Timer_AddFunction(BalanceCar_Speed_Control, 25);     //  将 BalanceCar_Speed_Control 函数添加到定时列表，执行周期为25ms
    Timer_AddFunction(BalanceCar_MotorOutput,        5); //  将 BalanceCar_MotorOutput   函数添加到定时列表，执行周期为 5ms
    Timer_Start(BalanceCar_Angle_Control);               //  开始为定时列表中的 BalanceCar_Angle_Control 函数计数定时
    Timer_Start(BalanceCar_GetSpeed);                    //  开始为定时列表中的 BalanceCar_GetSpeed      函数计数定时
    Timer_Start(BalanceCar_Turn_Control);                //  开始为定时列表中的 BalanceCar_Turn_Control  函数计数定时
    Timer_Start(BalanceCar_Speed_Control);               //  开始为定时列表中的 BalanceCar_Speed_Control 函数计数定时
    Timer_Start(BalanceCar_MotorOutput);                 //  开始为定时列表中的 BalanceCar_MotorOutput   函数计数定时
#endif
}

/*********************功能函数*/
