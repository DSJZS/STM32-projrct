#ifndef __TEXT_MENU_H__
#define __TEXT_MENU_H__

#include "Menu.h"





/*参数宏定义*********************/

/*记事本菜单图标的默认宽与高*/	/*	未在程序中使用	*/
#define TEXT_MENU_ICON_WIDTH 	Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define TEXT_MENU_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu TextMenu;

/*********************变量声明*/





/*函数声明*********************/

/*初始化函数*/
void TextMenu_Init(void);

/*********************函数声明*/





#endif
