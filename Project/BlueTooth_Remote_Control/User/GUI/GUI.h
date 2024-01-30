#ifndef __GUI_H__
#define __GUI_H__

#include "stm32f10x.h"                  // Device header

/*指令枚举*********************/

enum{
	GUI_COMMAND_NONE,	//	没指令
	GUI_COMMAND_UP,		//	向上指令
	GUI_COMMAND_DOWN,	//	向下指令
	GUI_COMMAND_LEFT,	//	向左指令
	GUI_COMMAND_RIGHT	//	向右指令
};

/*********************指令枚举*/


/*函数声明*********************/

/*初始化*/
void GUI_Init(void);

/*显示函数*/
void GUI_Display(void);

/*功能函数*/
void GUI_ReceiveCommand(uint8_t Command);

/*********************函数声明*/





#endif
