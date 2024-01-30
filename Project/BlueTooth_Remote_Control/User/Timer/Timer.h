#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

/*      此Timer模块并非用来管理所有的定时器，是用来使用某一个定时器来管理并执行某些需要定时执行的函数     */
#define USE_TIMER

#define Timer_TIM                   ( TIM2 )
#define Timer_TIM_APBxClock_FUN     (RCC_APB1PeriphClockCmd)  
#define Timer_TIM_CLK               ( RCC_APB1Periph_TIM2 )
#define Timer_TIM_Period            ( 7200-1 )
#define Timer_TIM_Prescaler         ( 10-1 )
#define Timer_TIM_IRQ               TIM2_IRQn
#define Timer_TIM_IRQHandler        TIM2_IRQHandler     //  此函数实现放在 stm32f10x_it.c  文件中


/*结构体定义*********************/

/*	定时器结构体，并非用于存放定时器信息，而是用于存放需要定时的函数的信息	*/
typedef struct Timer{
	uint8_t isRun;		//	定时标准位，需要定时的函数当前是否要继续定时
	uint16_t Target;	//	目标定时值，需要定时的函数要每定时多少毫秒才执行
	uint16_t Count;		//	当前定时值，需要定时的函数已经定时了多少毫秒
	void (*Function)(void);	//	函数指针，指向需要定时的函数的指针
}Timer;

/*********************结构体定义*/





/*函数声明*********************/

void Timer_Init(void);
uint8_t Timer_AddFunction( void (*Function)(void), uint16_t Target);
uint8_t Timer_DeleteFunction( void (*Function)(void));
uint8_t Timer_ToFunction_ByID(uint8_t ID);
void Timer_SetTarget( void (*Function)(void), uint16_t Target);
void Timer_SetCount( void (*Function)(void), uint16_t Count);
void Timer_Start(void (*Function)(void));
void Timer_Stop(void (*Function)(void));
void Timer_StartAll(void);
void Timer_StopAll(void);
Timer* Timer_GetStatus(uint8_t index);
uint8_t Timer_GetLength(void);

/*********************函数声明*/





#endif
