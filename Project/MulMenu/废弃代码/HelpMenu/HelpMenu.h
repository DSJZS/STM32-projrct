#ifndef __HELP_MENU_H__
#define __HELP_MENU_H__

#include "Menu.h"





/*参数宏定义*********************/

/*帮助菜单图标的默认宽与高*/
#define HELP_MENU_ICON_WIDTH 	Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define HELP_MENU_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu HelpMenu;

/*********************变量声明*/





/*函数声明*********************/

void HelpMenu_Funsion(void);
void HelpMenu_Init(void);

/*********************函数声明*/





#endif
