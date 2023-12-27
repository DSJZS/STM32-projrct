/***************************************************************************************
  * 本源文件也含有消息队列相关函数
  * 之所以创建了一个Message文件存放消息队列相关函数，又创建了一个MSG文件
  * 是为了避免头文件相互冲突，导致实现 MSG_DefaultProc 特别困难
  * 举个例子，如果在Message文件中直接定义 MSG_DefaultProc
  * 那么就不可能让 Timer 文件包含 Message 文件来发送消息的同时
  * 让 Message 文件里的 MSG_DefaultProc 函数来调用 Timer文件 里的 Timer_ToFunction_ByID 函数来处理消息
  * (这也可能是我水平有限，写的不好导致的问题，如果你有好的更好的办法，可以联系我)
  ***************************************************************************************
  */
  
#include "MSG.h"
#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "OLED.h"
#include "Delay.h"





/*功能函数*********************/

/**
  * 函    数：消息默认处理函数
  * 参    数：msg	存放接收信息的结构体指针
  * 返 回 值：无
  * 说    明：目前仅处理定时器发出的到时函数，执行到时后需要执行的函数
  */
void MSG_DefaultProc(MSG* msg)
{
	switch(msg->message)
	{
		case CM_TIMEUP: 	//	初始化定时器的到时消息
		{
			Timer_ToFunction_ByID(msg->param);	//	执行对应的函数
			break;
		}
		default:
		{
			break;
		}
	}
}


/*功能函数*********************/
