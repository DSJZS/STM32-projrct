#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "stm32f10x.h"                  // Device header





/*结构体定义*********************/

//	消息结构体，用于存储每一个消息
typedef struct TagMSG{
	uint8_t message;			//	消息
	uint8_t param;				//	消息参数
	uint8_t count;				//	消息重复计数，每个新消息为1，最大值为255
}MSG;

/*********************结构体定义*/





/*枚举*********************/

//	消息枚举
enum {
	CM_Init=0,			//	消息队列初始化
	CM_KEYDOWN,			//	按键按下
	CM_KEYUP,			//	按键松开
	CM_BOTTONDOWN,		//	旋转编码器按钮按下
	CM_BOTTONUP,		//	旋转编码器按钮松开
	CM_ROATATING,		//	旋转编码器旋转
	CM_TIMEUP,			//	定时器到时了
};
//	消息参数列表
enum PARAM
{
	VK_UNPRESS=0,	//	没有按键按下
	VK_KEY1,		//	按键1
	VK_KEY2,		//	按键2
	VK_KEY3,		//	按键3
	VK_KEY4,		//	按键4
	VK_BUTTON,		//	旋转编码器按钮
	
	EK_LEFT,		//	旋转编码器向左旋转
	EK_RIGHT,		//	旋转编码器向右旋转
};

/*********************枚举*/





/*宏定义*********************/

//	取出1字节数据的高4位与低4位
#define HIGCHAR(c) (((c)>>4)&0x0f)
#define LOWCHAR(c) ((c)&0x0f)

/*********************宏定义*/





/*函数声明*********************/

void MSG_Init(void);
uint8_t MSG_SendMessage(MSG* M);
uint8_t MSG_GetMessage(MSG* M);
//	void MSG_PostMessage(MSG* M);

/*********************函数声明*/





#endif
