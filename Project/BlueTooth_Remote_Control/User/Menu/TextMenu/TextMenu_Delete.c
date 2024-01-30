/***************************************************************************************
  * 本菜单为记事本菜单的二级子菜单，用于删除已存储的文本文件，并且保存结果到FLASH
  * 删除的方式为被删除的文本文件后面的文件往前移动，覆盖掉被删除的文件
  ***************************************************************************************
  */

#include "TextMenu_Delete.h"
#include "TextMenu_Create.h"
#include "stm32f10x.h"                  // Device header

#include "Key.h"
#include "OLED.h"
#include "Menu.h"
#include "Delay.h"
#include "Store.h"
#include "Encoder.h"

#include "MSG.h"

/*图像数据*********************/

/*	记事本删除菜单图标	——	为一个垃圾桶	*/
const uint8_t TextMenu_Delete_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0xE0,0xF8,0xFC,0xBC,0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,0xBC,0xFC,0xF8,
	0xE0,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0x3F,0xBF,0xBF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
	0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
	0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0xBF,0xBF,0x3F,0x1F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,
	0xF8,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0xF8,
	0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x30,0x30,0x30,0x30,0x30,0x31,0x31,0x31,
	0x31,0x30,0x30,0x30,0x30,0x30,0x31,0x31,0x31,0x31,0x30,0x30,0x30,0x30,0x30,0x31,
	0x31,0x31,0x31,0x30,0x30,0x30,0x30,0x30,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	记事本删除菜单结构体	*/
Menu TextMenu_Delete;	

/*	记事本删除菜单名字	*/
const char* TextMenu_Delete_Name = "删除";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：记事本删除菜单的状态零视图显示
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本删除菜单状态零视图显示
  */
static void TextMenu_Delete_View_Status0(void)
{
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	读取已存储的文本个数
	
	OLED_Clear();	//	清屏
	
	if(TextNum >= 1)	//	判断是否有第一个文本
	{
		OLED_ShowImage( 10,0,20,48,Button);
		OLED_ShowChinese( 3, 48,  "文一");
	}
	if(TextNum >= 2)	//	判断是否有第二个文本
	{
		OLED_ShowImage(40,0,20,48,Button);
		OLED_ShowChinese( 34, 48, "文二");
	}
	if(TextNum >= 3)	//	判断是否有第三个文本
	{
		OLED_ShowImage(70,0,20,48,Button);
		OLED_ShowChinese( 65, 48, "文三");
	}
	
	OLED_ShowImage(100,0,20,48,Button);
	OLED_ShowChinese( 95, 48, "退出");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本删除菜单删除文本
  * 参    数：Target	删除的文本是第几个文本
  * 返 回 值：无
  * 说    明：第 Target 个文本之后的文本会往前移动一个单位存放，覆盖掉第 Target 个文本
  *           同时存储的文本个数会减一
  */
static void TextMenu_Delete_DeleteText(uint8_t Target)
{
	uint8_t i = 0;	//	循环计数变量
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	读取已存储的文本个数

	for(i=0;i<TEXT_MAX-Target;++i)	//	有几个文本文件需要移动
	{
		uint8_t j = 0;	//	循环计数变量
		for(j = 0;j < TITLE_LENGTH; ++j)	//	后面文件的文本标题字符串覆盖前面文件的文本标题字符串
			*((uint8_t*)Store_Data+20+(Target-1+i)*(TITLE_LENGTH+CONTENT_LENGTH)+j) = *((uint8_t*)Store_Data+20+(Target+i)*(TITLE_LENGTH+CONTENT_LENGTH)+j);
	}
	for(i=0;i<TEXT_MAX-Target;++i)	//	有几个文本文件需要移动
	{
		uint8_t j = 0;	//	循环计数变量
		for(j = 0;j < CONTENT_LENGTH; ++j)	//	后面文件的文本内容字符串覆盖前面文件的文本内容字符串
			*((uint8_t*)Store_Data+20+(Target-1+i)*(TITLE_LENGTH+CONTENT_LENGTH)+TITLE_LENGTH+j) = *((uint8_t*)Store_Data+20+(Target+i)*(TITLE_LENGTH+CONTENT_LENGTH)+TITLE_LENGTH+j);
	}
	
	*((uint8_t*)Store_Data+2) = TextNum-1;	//	存储的文本个数会减一
	
	Store_Save();	//	保存到FLASH
	
	OLED_Clear();	//	清屏
	
	//	提示成功
	OLED_ShowImage( 38, 0, 48, 48, Pass);
	OLED_ShowChinese( 46, 48, "完成");
	
	OLED_Update();	//	刷新
	Delay_ms(1000);	//	延时1s
}

/**
  * 函    数：记事本没有存放任何文件而发出的警告
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议配合Delay函数使用
  */
static void TextMenu_Delete_View_Warning(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 38, 0, 48, 48, Warning);
	OLED_ShowChinese( 0, 48, "没有存放任何文件");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本删除菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本删除菜单的功能函数
  */
static void TextMenu_Delete_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	读取已存储的文本个数
	
	if(TextNum == 0)	//	没有存放任何文件，给予警告并退出功能函数
	{
		TextMenu_Delete_View_Warning();
		Delay_ms(1000);
		return;
	}
	
	TextMenu_Delete_View_Status0();	//	显示状态零视图
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(TextNum>=1)	//	判断是否有第一个文本
				{
					TextMenu_Delete_DeleteText(1);	//	删除第一个文本
					return;	//	退出
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(TextNum>=2)	//	判断是否有第二个文本
				{
					TextMenu_Delete_DeleteText(2);	//	删除第二个文本
					return;	//	退出
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(TextNum>=3)	//	判断是否有第二个文本
				{
					TextMenu_Delete_DeleteText(3);	//	删除第二个文本
					return;	//	退出
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				return;
			}
			
			else
			{
				MSG_DefaultProc(&msg);	//	消息默认处理
			}
		}
	}
}

/**
  * 函    数：初始化记事本删除菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了记事本删除菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void TextMenu_Delete_Init(void)
{
	/*初始化记事本删除菜单*/
	Menu_Init(&TextMenu_Delete);
	TextMenu_Delete.Funsion = TextMenu_Delete_Funsion;
	TextMenu_Delete.Icon = TextMenu_Delete_Icon;
	TextMenu_Delete.Name = TextMenu_Delete_Name;
	/*初始化记事本删除菜单*/
	
	Store_Init();	//	初始化Store模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
}

/*********************功能函数*/
