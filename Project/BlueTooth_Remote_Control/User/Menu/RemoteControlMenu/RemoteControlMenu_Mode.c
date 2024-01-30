/***************************************************************************************
  * 本菜单为遥控菜单的二级菜单
  * 用于远程控制小车模式
  ***************************************************************************************
  */
  
#include "RemoteControlMenu_Mode.h"
#include "stm32f10x.h"                  // Device header
#include "../Menu.h"
#include "../../OLED/OLED.h"

/*图像数据*********************/

/*	遥控模式菜单图标	——	为一张写满了内容的纸	*/
const uint8_t RCMenu_Mode_Icon[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x02,0x02,0xF2,0x12,0x12,0x12,0x12,
    0x12,0x12,0x12,0x12,0x12,0xF2,0x02,0x02,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62,
    0x62,0x62,0x62,0x62,0x62,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0xFF,0x80,0x80,0x80,0x80,
    0x80,0x80,0x80,0x80,0x80,0xFF,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,
    0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x10,0x20,0x20,
    0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x10,
    0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x42,0x82,0x82,0x82,0x84,
    0x84,0x84,0x44,0x42,0x84,0x84,0x84,0x80,0x82,0x82,0x84,0x84,0x84,0x84,0x44,0x42,
    0x82,0x84,0x84,0x84,0x44,0x80,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x08,0x88,0x08,0x08,0x10,
    0x10,0x10,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x08,0x08,0x08,
    0x08,0x08,0x08,0x10,0x10,0x10,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x40,0x41,0x41,0x41,
    0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x48,0x48,
    0x44,0x48,0x48,0x50,0x50,0x48,0x50,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	遥控模式菜单结构体	*/
Menu RCMenu_Mode;	

/*	遥控模式菜单名字	*/
const char* RCMenu_Mode_Name = "模式";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：遥控模式菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于遥控模式菜单的功能函数
  */
void RCMenu_Mode_Funsion(void)
{
	;//	为空，没实现模式的调节，因为目前就实现了一个模式	
}

/**
  * 函    数：初始化遥控模式菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了遥控模式菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void RCMenu_Mode_Init(void)
{
	Menu_Init(&RCMenu_Mode);
	RCMenu_Mode.Funsion = RCMenu_Mode_Funsion;
	RCMenu_Mode.Icon = RCMenu_Mode_Icon;
	RCMenu_Mode.Name = RCMenu_Mode_Name;
}

/*********************功能函数*/