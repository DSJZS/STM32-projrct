#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"                  // Device header





/*结构体定义*********************/

/*	定时器结构体，并非用于存放定时器信息，而是用于存放需要定时的函数的信息	*/
typedef struct Timer{
	uint8_t ID;			//	ID，表示需要定时的函数在定时列表中的ID
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

/*********************函数声明*/





#endif
