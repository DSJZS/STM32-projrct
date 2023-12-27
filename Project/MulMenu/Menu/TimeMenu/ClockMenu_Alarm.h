#ifndef __CLOCK_MENU_ALARM_H__
#define __CLOCK_MENU_ALARM_H__

#include "Menu.h"





/*参数宏定义*********************/

/*闹钟菜单图标的默认宽与高*/	/*	未在程序中使用	*/
#define CLOCK_MENU_ALARM_ICON_WIDTH 	Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define CLOCK_MENU_ALARM_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*闹钟个数上限*/
#define ALARM_MAX 	3

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu ClockMenu_Alarm;

/*********************变量声明*/





/*函数声明*********************/

void ClockMenu_Alarm_Init(void);

/*********************函数声明*/





#endif
