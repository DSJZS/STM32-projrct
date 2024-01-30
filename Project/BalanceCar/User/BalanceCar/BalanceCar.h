#ifndef __BALANCE_CAR_H
#define __BALANCE_CAR_H

#include "stm32f10x.h"

#include "../PID/PID.h"

/*结构体定义*********************/

/*  平衡车结构体，用于存放各种与平衡车保持平衡相关的数据  */
struct BalanceCar{
    float angleOffset;  //  机械零点偏移值  
    float angleSet;     //  为设置的平衡车角度，而非实际角度
    float speedSet;     //  为设置的平衡车速度，而非实际速度
    float turnSet;      //  为设置的平衡车转向，而非实际转向
    float leftSpeed;    //  左轮速度
    float rightSpeed;   //  右轮速度
    PID speed;          //  速度环
    PID angle;          //  直立环
    PID turn;           //  转向环
    uint32_t mode;      //  模式
};

/*********************结构体定义*/





/*宏参数定义*********************/


#define ANGLEOFF             ( 1.5f )                                   //  机械零点偏移值

/*如下是一个大一初学者调试PID参数时其大脑的变化*/
/*
 *  一开始的直立环参数调整的无比顺利，Kp调后就平衡了，Kd调好后稳得不行
 *  仅接着为了使其平衡时不前后移动，我前前后后看各种教程调了半天，最后勉强使小车不前后移动了
 *  紧接着就是噩梦的开始，当我造完蓝牙遥控器准备遥控时，我发现小车在前进和后退时非常僵硬
 *  车身会不停的前后摇摆，而且不走直线，查资料加了转向环后依旧不走直线
 *  最后看到网上有人说运动时可以适当调小PID，尝试之后发现效果确实可以
*  总而言之，我个人总结的步骤如下(可能不具有普遍性)：
 8  ( 以下省略参数极性的确定，极性应该很好确定是不是反了 )
 *  1.一开始直立环、速度环、转向环的PID参数均为 0
 *  2.由小到大的调整直立环 Kp ，直到出现大幅度，但是频率较低的摆动
 *  3.接着由小到大的调整直立环 Kd ，直到出现小幅度，但是频率较高的摆动
 *  4.直立环的Ki不用管，之间为 0 即可
 *  5.之后将直立环的Kp与Kd按一定的比例缩小(比如都乘以0.6)，这是为了之后调节速度环的参数，不缩小可能怎么也调不出来
 *    缩小参数之后的小车应该会不倒，但是前后来回的移动，不停留在原地，接下来调节速度环参数来解决这个问题
 *  6.速度环 Kp 和 Ki 之间为200倍关系，即 Ki = Kp / 200。Kd 直接为 0。因此速度环只用调节 Kp 即可
 *  7.由小到大的调整速度环 Kp，使前后来回的移动的距离不断缩小
 *    值得注意的是速度环 Kp 的增大会使小车出现前后摆动的现象
 *    因此在增大速度环 Kp 的同时也要增大直立环 Kd 来抑制摆动
 *    要调节到小车不有明显的前后移动，以及用手推动小车时不出现大幅度的倾斜摆动
 *    ( 小车前后移动是速度环 Kp、Ki 过小。
 *      小车用手推无法回到原位(如果没用积分限幅)、出现大幅度摆动是速度环 Kp、Ki 过大 和直立环 Kd 较小。)
 *  8.适当的等比例缩小直立环和速度环的PID参数，这是为了之后调节转向环的参数
 *  8.此时小车可能会有原地旋转的现象，需要加入转向环来控制，转向环的 Ki、Kd 直接为0，只需调节 Kp
 *    由小到大不断的增加转向环的Kp，直到小车不再原地旋转，同时有小幅度的高频摆动(个人观察到的)
 *    ( 小车原地旋转或者无法直线前进后退是 转向环的Kp 过小
 *      小车前进和后退时的摆动也可能与 转向环的Kp 过小有关(个人调整中发现的问题，存疑) )
 *  9.移动时可能出现运动僵硬的问题，比如摆动幅度较大，被直立环拉扯等现象
 *    此时要在移动时以适当的比例缩小直立环、速度环、转向环的 PID 参数
 * 10.如果速度环没有使用积分限幅，则需要控制移动速度和倾斜角度在一定的范围之内
 *
*/

/*静止时PID参数*/
#define STATIC_ANGLE_KP             ( -800.0f*0.6f*0.9f )               //  直立环静止状态的Kp参数
#define STATIC_ANGLE_KI             ( 0.0f )                            //  直立环静止状态的Ki参数
#define STATIC_ANGLE_KD             ( -4000.0f*0.9f )                   //  直立环静止状态的Kd参数

#define STATIC_SPEED_KP             ( 2300.0f *0.9f )                   //  速度环静止状态的Kp参数
#define STATIC_SPEED_KI             ( STATIC_SPEED_KP / 200.0f )        //  速度环静止状态的Ki参数
#define STATIC_SPEED_KD             ( 0.0f )                            //  速度环静止状态的Kd参数

#define STATIC_TURN_KP              ( 4500.0f )                         //  转向环静止状态的Kp参数
#define STATIC_TURN_KI              ( 0.0f )                            //  转向环静止状态的Ki参数
#define STATIC_TURN_KD              ( 0.0f )                            //  转向环静止状态的Kd参数

/*动态时PID参数*/
#define DYNAMIC_ANGLE_KP             ( STATIC_ANGLE_KP * 0.8f )         //  直立环运动状态的Kp参数
#define DYNAMIC_ANGLE_KI             ( STATIC_ANGLE_KI * 0.8f )         //  直立环运动状态的Ki参数
#define DYNAMIC_ANGLE_KD             ( STATIC_ANGLE_KD * 0.8f )         //  直立环运动状态的Kd参数

#define DYNAMIC_SPEED_KP             ( STATIC_SPEED_KP * 0.5f )         //  速度环运动状态的Kp参数
#define DYNAMIC_SPEED_KI             ( STATIC_SPEED_KI * 0.5f )         //  速度环运动状态的Ki参数
#define DYNAMIC_SPEED_KD             ( STATIC_SPEED_KD * 0.5f )         //  速度环运动状态的Kd参数

#define DYNAMIC_TURN_KP              ( STATIC_TURN_KP * 0.5f )          //  转向环运动状态的Kp参数
#define DYNAMIC_TURN_KI              ( STATIC_TURN_KI * 0.5f )          //  转向环运动状态的Ki参数
#define DYNAMIC_TURN_KD              ( STATIC_TURN_KD * 0.5f )          //  转向环运动状态的Kd参数
/*********************宏参数定义*/





/*变量声明*********************/

extern struct BalanceCar BC;

/*********************变量声明*/





/*函数声明*********************/

void BalanceCar_SetSpeed(float Speed);
void BalanceCar_SetTurn(float Turn);
void BalanceCar_Init(void);

/*********************函数声明*/

#endif
