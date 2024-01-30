#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"





/*宏定义*********************/

#define mdelay SysTick_Delay_Ms         //  MPU6050模块用到

#define delay_s     SysTick_Delay_S     //  秒延时
#define delay_ms    SysTick_Delay_Ms    //  毫秒延时
#define delay_us    SysTick_Delay_Us    //  微秒延时

/*********************宏定义*/





/*函数声明*********************/

int get_tick_count(unsigned long *count);
//void mdelay(unsigned long nTime);
void SysTick_Init(void);
void SysTick_Delay_S( __IO uint32_t s);
void SysTick_Delay_Ms( __IO uint32_t ms);
void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Cmd(FunctionalState NewState);

/*********************函数声明*/

#endif /* __SYSTICK_H */
