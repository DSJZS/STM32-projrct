#ifndef __MG310_H
#define	__MG310_H

#include "stm32f10x.h"





/*宏参数定义*********************/

/*  1路电机编码器的AB相相关宏定义        */
//  接收编码器信号的定时器(用编码器接口)
#define         MG310_ENCODER1_TIM                      ( TIM3 )
#define         MG310_ENCODER1_TIM_APBxClock_FUN        ( RCC_APB1PeriphClockCmd ) 
#define         MG310_ENCODER1_TIM_CLK                  ( RCC_APB1Periph_TIM3 )
#define         MG310_ENCODER1_TIM_Period               ( 65536-1 )
#define         MG310_ENCODER1_TIM_Prescaler            ( 1-1 )
//  连接A相的输入比较通道(这里为IC2)
#define         MG310_ENCODER1_A_TIM_CH_GPIO_APBxClock_FUN     (RCC_APB2PeriphClockCmd)
#define         MG310_ENCODER1_A_TIM_CH_GPIO_CLK               ( RCC_APB2Periph_GPIOA )
#define         MG310_ENCODER1_A_TIM_CH_PORT                   ( GPIOA )
#define         MG310_ENCODER1_A_TIM_CH_PIN                    ( GPIO_Pin_7 )
//  连接B相的输入比较通道(这里为IC1)
#define         MG310_ENCODER1_B_TIM_CH_GPIO_APBxClock_FUN     (RCC_APB2PeriphClockCmd)
#define         MG310_ENCODER1_B_TIM_CH_GPIO_CLK               ( RCC_APB2Periph_GPIOA )
#define         MG310_ENCODER1_B_TIM_CH_PORT                   ( GPIOA )
#define         MG310_ENCODER1_B_TIM_CH_PIN                    ( GPIO_Pin_6 )

/*  2路电机编码器的AB相相关宏定义        */
//  接收编码器信号的定时器(用编码器接口)
#define         MG310_ENCODER2_TIM                      ( TIM2 )
#define         MG310_ENCODER2_TIM_APBxClock_FUN        ( RCC_APB1PeriphClockCmd ) 
#define         MG310_ENCODER2_TIM_CLK                  ( RCC_APB1Periph_TIM2 )
#define         MG310_ENCODER2_TIM_Period               ( 65536-1 )
#define         MG310_ENCODER2_TIM_Prescaler            ( 1-1 )
//  连接A相的输入比较通道
#define         MG310_ENCODER2_A_TIM_CH_GPIO_APBxClock_FUN    (RCC_APB2PeriphClockCmd)
#define         MG310_ENCODER2_A_TIM_CH_GPIO_CLK             ( RCC_APB2Periph_GPIOA )
#define         MG310_ENCODER2_A_TIM_CH_PORT                  ( GPIOA )
#define         MG310_ENCODER2_A_TIM_CH_PIN                   ( GPIO_Pin_1)
//  连接B相的输入比较通道
#define         MG310_ENCODER2_B_TIM_CH_GPIO_APBxClock_FUN    (RCC_APB2PeriphClockCmd)
#define         MG310_ENCODER2_B_TIM_CH_GPIO_CLK              ( RCC_APB2Periph_GPIOA )
#define         MG310_ENCODER2_B_TIM_CH_PORT                  ( GPIOA )
#define         MG310_ENCODER2_B_TIM_CH_PIN                   ( GPIO_Pin_0 )





/*  PWM信号幅度                         */
#define MOTOR_OUT_MAX       7200
/*  TB6612芯片1路电机相关宏定义           */
//  连接AIN1的GPIO端口
#define         MG310_TB6612_AIN1_GPIO_APBxClock_FUN        (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_AIN1_GPIO_CLK                  ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_AIN1_PORT                      ( GPIOA )
#define         MG310_TB6612_AIN1_PIN                       ( GPIO_Pin_2 )
//  连接AIN2的GPIO端口
#define         MG310_TB6612_AIN2_GPIO_APBxClock_FUN        (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_AIN2_GPIO_CLK                  ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_AIN2_PORT                      ( GPIOA )
#define         MG310_TB6612_AIN2_PIN                       ( GPIO_Pin_3 )
// 向PWMA发出PWM信号的定时器
#define         MG310_TB6612_PWMA_TIM                       ( TIM1 )
#define         MG310_TB6612_PWMA_TIM_APBxClock_FUN         ( RCC_APB2PeriphClockCmd )
#define         MG310_TB6612_PWMA_TIM_CLK                   ( RCC_APB2Periph_TIM1 )
#define         MG310_TB6612_PWMA_TIM_Period                ( MOTOR_OUT_MAX-1 )
#define         MG310_TB6612_PWMA_TIM_Prescaler             ( 1-1 )
//  向PWMA发出PWM信号输出比较通道
#define         MG310_TB6612_PWMA_TIM_CH_GPIO_APBxClock_FUN         (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_PWMA_TIM_CH_GPIO_CLK                   ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_PWMA_TIM_CH_PORT                       ( GPIOA )
#define         MG310_TB6612_PWMA_TIM_CH_PIN                        ( GPIO_Pin_11 )
#define         MG310_TB6612_PWMA_TIM_OCxInit                       ( TIM_OC4Init )
#define         MG310_TB6612_PWMA_TIM_OCxPreloadConfig              ( TIM_OC4PreloadConfig )
#define         MG310_TB6612_PWMA_TIM_SetComparex                   ( TIM_SetCompare4 )

/*  TB6612芯片2路电机相关宏定义           */
//  连接BIN1的GPIO端口
#define         MG310_TB6612_BIN1_GPIO_APBxClock_FUN        (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_BIN1_GPIO_CLK                  ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_BIN1_PORT                      ( GPIOA )
#define         MG310_TB6612_BIN1_PIN                       ( GPIO_Pin_4 )
//  连接BIN2的GPIO端口
#define         MG310_TB6612_BIN2_GPIO_APBxClock_FUN        (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_BIN2_GPIO_CLK                  ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_BIN2_PORT                      ( GPIOA )
#define         MG310_TB6612_BIN2_PIN                       ( GPIO_Pin_5 )
// 向PWMB发出PWM信号的定时器
#define         MG310_TB6612_PWMB_TIM                       ( TIM1 )
#define         MG310_TB6612_PWMB_TIM_APBxClock_FUN         ( RCC_APB2PeriphClockCmd )
#define         MG310_TB6612_PWMB_TIM_CLK                   ( RCC_APB2Periph_TIM1 )
#define         MG310_TB6612_PWMB_TIM_Period                ( MOTOR_OUT_MAX-1 )
#define         MG310_TB6612_PWMB_TIM_Prescaler             ( 1-1 )
//  向PWMB发出PWM信号输出比较通道
#define         MG310_TB6612_PWMB_TIM_CH_GPIO_APBxClock_FUN         (RCC_APB2PeriphClockCmd)
#define         MG310_TB6612_PWMB_TIM_CH_GPIO_CLK                   ( RCC_APB2Periph_GPIOA )
#define         MG310_TB6612_PWMB_TIM_CH_PORT                       ( GPIOA )
#define         MG310_TB6612_PWMB_TIM_CH_PIN                        ( GPIO_Pin_8 )
#define         MG310_TB6612_PWMB_TIM_OCxInit                       ( TIM_OC1Init )
#define         MG310_TB6612_PWMB_TIM_OCxPreloadConfig              ( TIM_OC1PreloadConfig )
#define         MG310_TB6612_PWMB_TIM_SetComparex                   ( TIM_SetCompare1 )

/*********************宏参数定义*/





/*函数声明*********************/

int16_t MG310_Encoder1_GetCount(void);
int16_t MG310_Encoder2_GetCount(void);


void MG310_Motor_SetLeftVelocity(int16_t Speed);
void MG310_Motor_SetRightVelocity(int16_t Speed);
void MG310_Motor_SetVelocity(int16_t Speed);
void MG310_Init(void);
void MG310_Close(void);

/*********************函数声明*/

#endif
