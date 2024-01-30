#ifndef __GAME_MENU_MINESWEEPER_H__
#define __GAME_MENU_MINESWEEPER_H__

#include "Menu.h"





/*参数宏定义*********************/

/*扫雷游戏菜单图标的默认宽与高*/	/*	未在程序中使用	*/
#define GAME_MENU_MINESWEEPER_ICON_WIDTH 	Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define GAME_MENU_MINESWEEPER_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu GameMenu_Minesweeper;

/*********************变量声明*/





/*函数声明*********************/

void GameMenu_Minesweeper_Init(void);

/*********************函数声明*/






#endif
