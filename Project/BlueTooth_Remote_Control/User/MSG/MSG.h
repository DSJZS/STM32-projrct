#ifndef __MSG_H__
#define __MSG_H__

#include "stm32f10x.h"                  // Device header

/*宏定义*********************/

#define USE_MSG            // 表示使用了MSG模块

#define message_type    uint8_t
#define param_type      uint32_t    //  建议此宏不要改变，涉及其它模块发出的参数
#define count_type      uint8_t
//	取出1字节数据的高4位与低4位
#define HIGCHAR(c) (((c)>>4)&0x0f)
#define LOWCHAR(c) ((c)&0x0f)

/*********************宏定义*/




/*结构体定义*********************/

//	消息结构体，用于存储每一个消息
typedef struct TagMSG{
	message_type message;			//	消息
	param_type param;				//	消息参数
	count_type count;				//	消息重复计数，每个新消息为1，最大值为255
}MSG;

/*********************结构体定义*/





/*枚举*********************/

//	消息枚举
enum {
	CM_Init=0,			//	消息队列初始化
	CM_KEYDOWN,			//	按键按下
	CM_KEYUP,			//	按键松开
    CM_KEYERROR,        //  按键模块出现错误
	CM_ROATATING,		//	旋转编码器旋转
	CM_TIMEUP,			//	定时器到时了
    CM_BLTCMD,          //  蓝牙指令
    CM_BLTERROR,        //  蓝牙错误
};
//	消息参数列表
enum PARAM
{
	VK_NOKEY=0,	    //	空按键
	VK_KEY1,		//	按键1
	VK_KEY2,		//	按键2
	VK_KEY3,		//	按键3
	VK_KEY4,		//	按键4
	VK_KEY5,		//	按键5
	VK_KEY6,		//	按键6
	VK_KEY7,		//	按键7
	VK_KEY8,		//	按键8
    VK_KEY9,		//	按键9
	VK_KEY10,		//	按键10
	VK_KEY11,		//	按键11
	VK_KEY12,		//	按键12
    VK_KEY13,		//	按键13
	VK_KEY14,		//	按键14
	VK_KEY15,		//	按键15
	VK_KEY16,		//	按键16
    VK_KEY17,		//	按键17
	VK_KEY18,		//	按键18
	VK_KEY19,		//	按键19
	VK_KEY20,		//	按键20
    VK_KEY21,		//	按键21
	VK_KEY22,		//	按键22
	VK_KEY23,		//	按键23
	VK_KEY24,		//	按键24
    VK_KEY25,		//	按键25
	VK_KEY26,		//	按键26
	VK_KEY27,		//	按键27
	VK_KEY28,		//	按键28
    VK_KEY29,		//	按键29
	VK_KEY30,		//	按键30
	VK_KEY31,		//	按键31
	VK_KEY32,		//	按键32
    VK_INIT,        //  按键初始化(用于错误消息辨别)
    VK_SCAN,        //  按键扫描(用于错误消息辨别)
	
	EK_LEFT,		//	旋转编码器向左旋转
	EK_RIGHT,		//	旋转编码器向右旋转
    
    BLT_ADVANCE,
    BLT_BACK,
    BLT_LEFT,
    BLT_RIGHT,
    BLT_BRAKE,
    BLT_STOP,
    BLT_MOVE_MODE,
    BLT_QUIT,
};

/*********************枚举*/




/*函数声明*********************/

void MSG_Init(void);
uint8_t MSG_SendMessage(MSG* M);
uint8_t MSG_GetMessage(MSG* M);
//	void MSG_PostMessage(MSG* M);
void MSG_DefaultProc(MSG* msg);

/*********************函数声明*/





#endif
