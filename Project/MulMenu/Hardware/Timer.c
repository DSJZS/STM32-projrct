/***************************************************************************************
  * 本源文件含有消息队列相关函数
  * 个人曾经学过一点Windows编程和一点Qt，发现里面的定时器可以直接为函数定时
  * 水平有限的同时，为了学习，所以写一个简易的替代
  ***************************************************************************************
  */
  
#include "Timer.h"
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "MSG.h"





/*参数宏定义*********************/

#define TIM_QUEUE_CAP 16	//	定时器队列最多存多少消息

/*********************参数宏定义*/





/*全局变量*********************/

/*定时列表，为顺序表，用于存放各个需要定时执行的函数(考虑改为链队，但是不知道stm32怎么用链表LOL)	*/
static struct TimerList{
	uint8_t Capacity;			//	线性表容量
	uint8_t Length;				//	线性表长度
	Timer Data[TIM_QUEUE_CAP];	//	定时列表数据，存放者需要定时执行的函数
}TL;

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：初始化定时列表、定时器2并且打开定时器2的中断
  * 参    数：无
  * 返 回 值：无
  * 说    明：使用了定时器2来实现各个功能，定时中断每1ms触发一次，这也是定时的最小单位
  */
void Timer_Init(void)	//	T = 1ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	uint8_t i = 0;
	
	TL.Capacity = TIM_QUEUE_CAP;	//	确定定时列表的长度上限
	TL.Length = 0;	//	初始化定时列表的长度
	for(i=0;i<TL.Capacity;++i)	//	清空定时列表
	{
		TL.Data[i].Function = NULL;
		TL.Data[i].ID = i;
		TL.Data[i].isRun = 0;
		TL.Data[i].Target = 0;
		TL.Data[i].Count = 0;
	}
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);	// 使能TIM2的内部时钟
	TIM_InternalClockConfig( TIM2);	//	选择内部时钟模式驱动时基单元

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	RepetitionCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);
	TIM_ClearFlag( TIM2, TIM_IT_Update);// 由于TIM_TimeBaseInit会生成一个更新事件。
	//				来重新装载预分频器和重复计数器的值
	//				并且更新事件和更新中断是同时产生，所以会产生更新中断并且置更新中断标志位
	//				初始化函数完后便会立刻进入中断，这是不好的，所以要在TIM_TimeBaseInit之后清除标志位

	
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);	//	使能更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//	指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//	配置指定中断通道使能状态
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//	配置指定抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//	配置指定响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd( TIM2, ENABLE);	//	使能计数器
}

/**
  * 函    数：添加函数到定时列表
  * 参    数：Function	要添加的函数
  * 参    数：Target	目标定时值，需要定时的函数要每定时多少毫秒才执行
  * 返 回 值：成功添加返回1，否则返回0
  * 说    明：如果定时列表满了或者函数已经在列表之中，则无事发生
  */
uint8_t Timer_AddFunction( void (*Function)(void), uint16_t Target)
{
	if(TL.Length >= TL.Capacity)
	{
		return 0;
	}
	else
	{
		uint8_t i = 0;
		for(i=0;i<TL.Capacity;++i)
		{
			if(TL.Data[i].Function == Function)
			{
				return 0;
			}
			if(TL.Data[i].Function == NULL)
			{
				TL.Data[i].Target = Target;
				TL.Data[i].Count = 0;
				TL.Data[i].Function = Function;
				TL.Data[i].isRun = 0;
				++TL.Length;
				break;
			}
		}
		
		return 1;
	}
}

/**
  * 函    数：从定时列表删除函数
  * 参    数：Function	要删除的函数
  * 返 回 值：成功删除返回1，否则返回0
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
uint8_t Timer_DeleteFunction( void (*Function)(void))
{
	if(TL.Length == 0)
	{
		return 0;
	}
	else
	{
		uint8_t i = 0;
		for(i=0;i<TL.Length;++i)
		{
			if(TL.Data[i].Function == Function)
			{
				TL.Data[i].isRun = 0;
				TL.Data[i].Function = NULL;
				TL.Data[i].Target = 0;
				TL.Data[i].Count = 0;
				--TL.Length;
				return 1;
			}
		}
		return 0;
	}
}

/**
  * 函    数：通过需要定时的函数在定时列表中的ID找到它并且执行它
  * 参    数：Function	要执行的函数
  * 返 回 值：成功执行返回1，否则返回0
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
uint8_t Timer_ToFunction_ByID(uint8_t ID)
{
	if(TL.Data[ID].Function != NULL)
	{
		TL.Data[ID].Function();
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：设置某个在定时列表中需要定时的函数的目标定时值
  * 参    数：Function	要修改目标定时值的函数
  * 参    数：Target	新的目标定时值
  * 返 回 值：无
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
void Timer_SetTarget( void (*Function)(void), uint16_t Target)
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		if(TL.Data[i].Function == Function)
		{
			TL.Data[i].Target = Target;
			break;
		}
	}
}

/**
  * 函    数：设置某个在定时列表中需要定时的函数的当前定时值
  * 参    数：Function	要修改目标定时值的函数
  * 参    数：Count		新的当前定时值
  * 返 回 值：无
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
void Timer_SetCount( void (*Function)(void), uint16_t Count)
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		if(TL.Data[i].Function == Function)
		{
			TL.Data[i].Count = Count;
			break;
		}
	}
}

/**
  * 函    数：开始为某个在定时列表中需要定时的函数定时
  * 参    数：Function	要开始定时的函数
  * 返 回 值：无
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
void Timer_Start(void (*Function)(void))
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		if(TL.Data[i].Function == Function)
		{
			TL.Data[i].isRun = 1;
			break;
		}
	}
}

/**
  * 函    数：停止为某个在定时列表中需要定时的函数定时
  * 参    数：Function	要停止定时的函数
  * 返 回 值：无
  * 说    明：如果定时列表中没有该函数，则无事发生
  */
void Timer_Stop(void (*Function)(void))
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		if(TL.Data[i].Function == Function)
		{
			TL.Data[i].isRun = 0;
			break;
		}
	}
}

/**
  * 函    数：开始为所有在定时列表中需要定时的函数定时
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果定时列表中没有函数，则无事发生
  */
void Timer_StartAll(void)
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		TL.Data[i].isRun = 1;
	}
}

/**
  * 函    数：停止为所有在定时列表中需要定时的函数定时
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果定时列表中没有函数，则无事发生
  */
void Timer_StopAll(void)
{
	uint8_t i = 0;
	for(i=0;i<TL.Length;++i)
	{
		TL.Data[i].isRun = 0;
	}
}

/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于处理TIM2定时中断
  */
void TIM2_IRQHandler(void)
{
	if(SET == TIM_GetITStatus( TIM2, TIM_IT_Update))
	{
		uint8_t i = 0;
		MSG msg;
		
		TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
		
		msg.message = CM_TIMEUP;	//	设置发送定时器到时了的消息
		for(i=0;i<TL.Length;++i)	//	扫描定时列表
		{
			//	如果列表中的第i个位置配置了函数且配置为定时
			if( (TL.Data[i].isRun==1) && (TL.Data[i].Function != NULL) )	
			{
				++(TL.Data[i].Count);	//	使其当前定时值加一
				if( TL.Data[i].Count >= TL.Data[i].Target)	//	如果当前定时值大于等于目标定时值
				{
					TL.Data[i].Count = 0;	//	清零当前定时值
					msg.param = i;	//	是消息参数为函数在列表中的ID
					MSG_SendMessage(&msg);	//	发送消息
				}
			}
		}
	}
}

/*********************功能函数*/
