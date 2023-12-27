/***************************************************************************************
  * 本源文件含有GUI相关函数
  * 用于处理外部给予的指令来执行多级菜单模块的相关操作函数
  * 用于显示多级菜单模块中聚焦的菜单(Menu.h中的Menu_Current指针指向的菜单)
  * 用于指向切换菜单的动画
  * ......
  ***************************************************************************************
  */
  
#include "GUI.h"
#include "stm32f10x.h"
#include "OLED.h"
#include "Menu.h"
#include <stdio.h>
#include <string.h>
#include "Delay.h"

#include "ClockMenu.h"			/*	时钟一级菜单							*/
#include "GameMenu.h"			/*	游戏一级菜单							*/
#include "TextMenu.h"			/*记事本一级菜单							*/
#include "SettingMenu.h"		/*	设置一级菜单							*/


/*

Time	-	Game

*/


/*参数宏定义*********************/

/*菜单间转换速度*/
#define MENU_TRANS_SPEED 7
/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*********************参数宏定义*/





/*功能函数*********************/

/**
  * 函    数：配置各菜单之间的关系
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有的菜单在此函数统一配置同级前后关系与父子关系
  */
static void GUI_SetRelationship(void)
{
	Menu_SetNext( &ClockMenu, &GameMenu);
	Menu_SetNext( &GameMenu, &TextMenu);
	Menu_SetNext( &TextMenu, &SettingMenu);

}

/**
  * 函    数：GUI初始化函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有菜单的初始化函数都要在此函数中执行，此外此函数决定了初始菜单界面是哪个菜单
  */
void GUI_Init(void)
{
	OLED_Init();
	
	ClockMenu_Init();
	GameMenu_Init();
	TextMenu_Init();
	SettingMenu_Init();
	

	
	
	GUI_SetRelationship();
	
	Menu_SetCurrent(&ClockMenu);
}

/**
  * 函    数：显示当前的菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：同时会显示当前菜单同级前面的菜单的右半边和同级后面的菜单的左半边
  *           当前的菜单会有一个矩形边框框住
  *			  此外菜单上方会按照 "父菜单名字：当前菜单名字" 的格式显示提示，
  *           如果没有父菜单，父菜单名字的位置显示 "菜单"
  */
void GUI_Display(void)
{
	OLED_Clear();
	
	if(Menu_GetFather() == NULL)
	{
		OLED_ShowChinese(  0, 0, "菜单：");	
		OLED_ShowChinese( 48, 0, Menu_GetName());	
	}
	else
	{
		OLED_ShowChinese(  0, 0, Menu_GetFather()->Name);
		OLED_ShowChinese( 8*(strlen(Menu_GetFather()->Name)/3*2), 0, "：");
		OLED_ShowChinese( 8*(strlen(Menu_GetFather()->Name)/3*2)+16, 0, Menu_GetName());
	}
	
	if(Menu_GetLast() != NULL)
		OLED_ShowImage( -24, 16, 48, 48, Menu_GetLast()->Icon);
		
	OLED_ShowImage( 40, 16, 48, 48, Menu_GetIcon());	
	
	if(Menu_GetNext() != NULL)
		OLED_ShowImage( 104, 16, 48, 48, Menu_GetNext()->Icon);
	
	OLED_DrawRectangle( 40, 16, 48, 48, OLED_UNFILLED);
	
	OLED_Update();
}

/**
  * 函    数：将当前菜单向前移动，使同级前菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有菜单会有向后移动的动画，动画速度可由 MENU_TRANS_SPEED 宏参数决定
  *           如果当前菜单没有同级前菜单，则无事发生
  */
static void GUI_ToLast(void)
{
	uint8_t i = 0;
	if(Menu_ToLast())
	{
		for(i=0;i<64;i+=MENU_TRANS_SPEED)
		{
			OLED_ClearArea( 40+i-MENU_TRANS_SPEED, 16, 48, 48);
			OLED_ClearArea(-24+i-MENU_TRANS_SPEED, 16, 48, 48);
			OLED_ShowImage( 40+i, 16, 48, 48, Menu_GetNext()->Icon);
			OLED_ShowImage(-24+i, 16, 48, 48, Menu_GetIcon());
			
			if(Menu_GetNext()->Next != NULL)
			{
				OLED_ClearArea(104+i-MENU_TRANS_SPEED, 16, 48, 48);
				OLED_ShowImage(104+i, 16, 48, 48, Menu_GetNext()->Next->Icon);
			}
			if(Menu_GetLast() != NULL)
			{
				OLED_ClearArea(-88+i-MENU_TRANS_SPEED, 16, 48, 48);
				OLED_ShowImage(-88+i, 16, 48, 48, Menu_GetLast()->Icon);
			}
			
			OLED_Update();
			Delay_ms(20);
		}
	}
}

/**
  * 函    数：将当前菜单向后移动，使同级后菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：所有菜单会有向前移动的动画，动画速度可由 MENU_TRANS_SPEED 宏参数决定
  *           如果当前菜单没有同级后菜单，则无事发生
  */
static void GUI_ToNext(void)
{
	uint8_t i = 0;
	if(Menu_ToNext())
	{
		for(i=0;i<64;i+=MENU_TRANS_SPEED)
		{
			OLED_ClearArea( 40-i+MENU_TRANS_SPEED, 16, 48, 48);
			OLED_ClearArea(104-i+MENU_TRANS_SPEED, 16, 48, 48);
			OLED_ShowImage( 40-i, 16, 48, 48, Menu_GetLast()->Icon);
			OLED_ShowImage(104-i, 16, 48, 48, Menu_GetIcon());
			
			if(Menu_GetLast()->Last != NULL)
			{
				OLED_ClearArea(-24-i+MENU_TRANS_SPEED, 16, 48, 48);
				OLED_ShowImage(-24-i, 16, 48, 48, Menu_GetLast()->Last->Icon);
			}
			if(Menu_GetNext() != NULL)
			{
				OLED_ClearArea(168-i+MENU_TRANS_SPEED, 16, 48, 48);
				OLED_ShowImage(168-i, 16, 48, 48, Menu_GetNext()->Icon);
			}
			
			OLED_Update();
			Delay_ms(20);
		}
	}
}

/**
  * 函    数：使当前菜单的父菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果当前菜单没有父菜单，则无事发生
  */
static void GUI_ToFather(void)
{
	Menu_ToFather();
}

/**
  * 函    数：使当前菜单的子菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果当前菜单没有子菜单，则无事发生
  */
static void GUI_ToChild(void)
{
	Menu_ToChild();
}

/**
  * 函    数：执行当前菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果当前菜单没有功能函数，则无事发生
  */
static void GUI_ToFunsion(void)
{
	Menu_ToFunsion();
}

/**
  * 函    数：GUI接收指令，并且做出反馈
  * 参    数：	GUI_COMMAND_NONE
				GUI_COMMAND_UP
				GUI_COMMAND_DOWN
				GUI_COMMAND_LEFT
				GUI_COMMAND_RIGHT
  * 返 回 值：无
  * 说    明：如果输入指令参数为 GUI_COMMAND_NONE 或者非以上参数列表中参数，则无事发生
  */
void GUI_ReceiveCommand(uint8_t Command)
{
	switch(Command)
	{
		case GUI_COMMAND_UP:
		{
			GUI_ToFunsion();
			GUI_ToChild();
			break;
		}
		case GUI_COMMAND_DOWN:
		{
			GUI_ToFather();
			break;
		}
		case GUI_COMMAND_LEFT:
		{
			GUI_ToLast();
			break;
		}
		case GUI_COMMAND_RIGHT:
		{
			GUI_ToNext();
			break;
		}
		default:
		{
			break;
		}
	}
}

/*********************功能函数*/
