#ifndef __TEXT_MENU_CREATE_H__
#define __TEXT_MENU_CREATE_H__

#include "Menu.h"





/*参数宏定义*********************/

/*记事本新建菜单图标的默认宽与高*/	/*	未在程序中使用	*/
#define TEXT_MENU_CREATE_ICON_WIDTH 		Menu_ICON_DEFAULT_WIDTH		//	菜单图标为默认宽
#define TEXT_MENU_CREATE_ICON_HEIGHT 	Menu_ICON_DEFAULT_HEIGHT	//	菜单图标为默认高

/*记事本文件的标题长度和文本长度*/
#define TITLE_LENGTH  	63
#define CONTENT_LENGTH 	105

/*记事本文件个数上限*/
#define TEXT_MAX 	3

/*********************参数宏定义*/





/*变量声明*********************/

extern Menu TextMenu_Create;

/*********************变量声明*/





/*函数声明*********************/

/*初始化函数*/
void TextMenu_Create_Init(void);

/*********************函数声明*/





#endif
