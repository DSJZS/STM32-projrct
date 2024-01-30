#ifndef __RC_MENU_H__
#define __RC_MENU_H__

#include "../Menu.h"





/*参数宏定义*********************/

/*遥控菜单图标的默认宽与高*/	/*	未在程序中使用	*/
#define RC_MENU_ICON_WIDTH 	Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define RC_MENU_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu RCMenu;

/*********************变量声明*/





/*函数声明*********************/

void RCMenu_Init(void);

/*********************函数声明*/






#endif
