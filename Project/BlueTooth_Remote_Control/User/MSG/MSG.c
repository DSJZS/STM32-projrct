/***************************************************************************************
  * 本源文件含有消息队列相关函数
  * 个人曾经学过一点Windows编程，其消息机制在我特别好用，
  * 水平有限的同时，为了学习，所以写一个简易的替代
  ***************************************************************************************
  */
  

#include "MSG.h"
#include "stm32f10x.h"                  // Device header

/*  用于消息默认处理函数*/
#include "./Timer/Timer.h"



/*宏定义*********************/

#define MSG_QUEUE_CAP 32	//	消息队列最多存多少消息

/*********************宏定义*/





/*全局变量*********************/

/*	消息队列，为顺序队，用于存放各个信息(考虑改为链队，但是不知道stm32怎么用链表LOL)	*/
static struct MessageQueue{
	uint8_t Capacity;	//	队列容量
	uint8_t front;		// 用下标代替头指针
	uint8_t rear;		// 用下标代替尾指针
	MSG Data[MSG_QUEUE_CAP+1];	//	消息队列数据
}MQ;

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：判断消息队列是否为空
  * 参    数：无
  * 返 回 值：消息队列为空返回 1，否则返回 0
  * 说    明：无
  */
static unsigned char MSG_isEmpty()
{
	return (MQ.front == MQ.rear);	//	利用头指针和尾指针的位置判断是否为空
}

/**
  * 函    数：消息溢出处理函数
  * 参    数：msg	-	溢出消息的指针
  * 返 回 值：处理成功返回 1，失败返回 0
  * 说    明：当前不处理
  */
static unsigned char MSG_MQOverflow(MSG* M)
{
	//	当前不处理
	
	return 0;
}

/**
  * 函    数：发出消息到消息队列
  * 参    数：msg	-	要发送消息的指针
  * 返 回 值：发送成功返回 1，失败返回 0
  * 说    明：消息队列存放不下新的消息时会把消息交给消息溢出处理函数处理
  *           收到消息后如果发现消息队列里有相同的消息，则队列里的该消息的重复计数加一，不生成新消息
  *           如果重复计数满了，则生成新消息
  */
uint8_t MSG_SendMessage(MSG* M)
{
	uint8_t i = 0;	//	for循环计数变量
	
	//	从消息队列中的最后一个消息往前找是否有重复的消息
	for(i = (MQ.rear+MQ.Capacity-1)%MQ.Capacity;i != (MQ.front+MQ.Capacity-1)%MQ.Capacity;i = (i+MQ.Capacity-1)%MQ.Capacity)
	{
		//	找到重复的消息
		if((MQ.Data[i].message == M->message) && (MQ.Data[i].param == M->param))
		{
			// 如果该信息的重复计数没有满，则加一并且结束函数
			// 如果满了，就退出循环，创建一个新消息，并且重新计数
			if(MQ.Data[i].count < 255)
			{
				++(MQ.Data[i].count);
				return 1;
			}
			else
			{
				break;
			}
		}
	}
	//	判断消息队列是否已满
	if( (MQ.rear+1) % MQ.Capacity == MQ.front )
	{
		return MSG_MQOverflow(M);	//	如果满了就执行消息队列溢出函数
	}
	else
	{
		MQ.Data[MQ.rear] = *M;	//	将消息放入队列末尾
		MQ.Data[MQ.rear].count = 1;	//	使消息重复计数默认为 1
		MQ.rear = (MQ.rear+1) % MQ.Capacity;	//	尾指针往后移动
		return 1;
	}
}


/**
  * 函    数：接收消息队列对头消息
  * 参    数：msg	-	存放接收信息的结构体指针
  * 返 回 值：有消息接收返回 1，没有则返回 0
  * 说    明：先发过来的先发出去
  */
uint8_t MSG_GetMessage(MSG* M)
{
	if(MSG_isEmpty())	//	判断消息队列是否为空
		return 0;
	*M = MQ.Data[MQ.front];	//	将队首的消息读出
	MQ.front = (MQ.front+1) % MQ.Capacity;	//	使头指针往后移动
	return 1;
}


//	//	告知程序外部有MessageProc()函数
//	extern void MessageProc(unsigned char message,unsigned char param,unsigned char count);
//	/**
//	  *@brief		发布消息给消息处理函数(该函数需要自行编写)
//	  *@param		msg	-	存放要发布信息的结构体指针
//	  *@retval	无
//	  **/
//	void MSG_PostMessage(MSG* M)
//	{
//		//	将数据发送到消息处理函数
//		MessageProc(M->message,M->param,M->count);
//	}


/**
  * 函    数：发送消息队列初始化消息
  * 参    数：无
  * 返 回 值：无
  * 说    明：消息队列初始化时会执行该函数
  */
static void MSG_SendInitMessage(void)
{
	MSG msg = {	CM_Init,	0	,	1};
	MQ.Data[MQ.rear] = msg;								//	将消息放入队列末尾
	MQ.rear = (MQ.rear+1) % MQ.Capacity;	//	尾指针往后移动
}

/**
  * 函    数：初始化消息队列
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果想使用消息队列，则需要在程序执行后一开始执行该函数，且只执行一次
  */
void MSG_Init(void)
{
	MQ.Capacity = MSG_QUEUE_CAP;	//	设置容量大小
	MQ.front = 0;	//	置零头指针	
	MQ.rear = 0;	//	置零尾指针
	MSG_SendInitMessage();	//	发送初始化消息
}

void MSG_DefaultProc(MSG* msg)
{
	switch(msg->message)
	{
#ifdef USE_TIMER
		case CM_TIMEUP: 	//	定时器的到时消息
		{
			//	执行对应的函数
            ((void(*)(void))(msg->param))();
			break;
		}
#endif
        
		default:
		{
			break;
		}
	}
}

/*功能函数*********************/
