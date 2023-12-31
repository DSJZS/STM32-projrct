/***************************************************************************************
  * 本菜单为时钟菜单的一级菜单
  * 有世界时钟菜单、闹钟菜单、时钟设置菜单这三个二级子菜单
  ***************************************************************************************
  */

#include "ClockMenu.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Menu.h"


#include "ClockMenu_Time.h"			/*时钟二级菜单	-	世界时钟菜单		*/
#include "ClockMenu_Alarm.h"		/*时钟二级菜单	-	闹钟菜单			*/
#include "ClockMenu_Setting.h"		/*时钟二级菜单	-	设置菜单			*/

/*图像数据*********************/

/*	时钟菜单图标	——	为一个时钟	*/
const uint8_t ClockMenu_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xF8,0xFC,0xFC,0xFE,0xFE,0xDE,
	0x8E,0x04,0x00,0x00,0x00,0x20,0x30,0xF0,0xF0,0x30,0x20,0x00,0x00,0x00,0x04,0x8E,
	0xDE,0xFE,0xFE,0xFC,0xFC,0xF8,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x0F,0x07,0x83,0xE1,0x73,0x37,0x1F,0x1F,
	0x0F,0x0F,0x06,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x06,0x0F,0x0F,
	0x1F,0x1F,0x37,0x73,0xE1,0x83,0x07,0x0F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFE,0x07,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x06,0x00,
	0x00,0x00,0x00,0x00,0x01,0x07,0xFE,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x7F,0xE0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x80,0xE0,0x7F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x8C,0xD8,0xF8,
	0x70,0x30,0x60,0x60,0xE0,0xE0,0xE0,0xEF,0xEF,0xE0,0xE0,0xE0,0x60,0x60,0x30,0x70,
	0xF8,0xD8,0x8C,0x0E,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x0F,0x0F,0x07,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x07,0x0F,0x0F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	时钟菜单结构体	*/
Menu ClockMenu;	

/*	时钟菜单名字	*/
const char* ClockMenu_Name = "时钟";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：时钟菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于时钟菜单的功能函数
  */
void ClockMenu_Funsion(void)
{
	;
}

/**
  * 函    数：初始化时钟菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了时钟菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void ClockMenu_Init(void)
{
	Menu_Init(&ClockMenu);
	ClockMenu.Funsion = ClockMenu_Funsion;
	ClockMenu.Icon = ClockMenu_Icon;
	ClockMenu.Name = ClockMenu_Name;
	
	ClockMenu_Time_Init();
	ClockMenu_Setting_Init();
	ClockMenu_Alarm_Init();
	
	Menu_SetFather( &ClockMenu_Time, &ClockMenu);
	Menu_SetFather( &ClockMenu_Alarm, &ClockMenu);
	Menu_SetFather( &ClockMenu_Setting, &ClockMenu);
}

/*********************功能函数*/
