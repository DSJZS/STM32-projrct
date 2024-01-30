/***************************************************************************************
  * 本菜单为记事本菜单的二级子菜单，用于读取已存储的文本文件
  ***************************************************************************************
  */

#include "TextMenu_Read.h"
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

/*	记事本读取菜单图标	——	为一张写有R的A4纸	*/
const uint8_t TextMenu_Read_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
	0xF0,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x20,0x1F,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x06,0x18,0x60,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x10,0x10,0x10,0x10,0x10,
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
	0x10,0x10,0x10,0x10,0x10,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	记事本读取菜单结构体	*/
Menu TextMenu_Read;	

/*	记事本读取菜单名字	*/
const char* TextMenu_Read_Name = "读取";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：记事本读取菜单的状态零视图显示
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本读取菜单状态零视图显示
  */
static void TextMenu_Read_View_Status0(void)
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
  * 函    数：记事本读取菜单的状态一视图显示
  * 参    数：Target	显示存储的第几个文本
  * 返 回 值：无
  * 说    明：用于记事本读取菜单状态一视图显示
  */
static void TextMenu_Read_DisplayText(uint8_t Target)
{
	uint8_t i = 0;	//	循环计数变量
	
	char Title[TITLE_LENGTH+1] = {0};		//	文本标题字符串
	char Content[CONTENT_LENGTH+1] = {0};	//	文本内容字符串
	
	OLED_Clear();	//	清屏
	
	for(i=0;i<TITLE_LENGTH;++i)	//	读取存放的第Target个文本的标题字符串 
	{
		Title[i] = *((uint8_t*)Store_Data+40+(Target-1)*(TITLE_LENGTH+CONTENT_LENGTH)+i);
	}
	for(i=0;i<CONTENT_LENGTH;++i)	//	读取存放的第Target个文本的内容字符串 
	{
		Content[i] = *((uint8_t*)Store_Data+40+(Target-1)*(TITLE_LENGTH+CONTENT_LENGTH)+TITLE_LENGTH+i);
	}
	
	for(i=0;i<TITLE_LENGTH;++i)	//	显示文本标题字符串中的所有字符，每21个为一排，最多三排
	{
		if(Title[i] != 0)
		{
			OLED_ShowChar( i*6%126+1, 0+(i/21)*8, Title[i], OLED_6X8);
		}			
	}
	OLED_DrawLine( 0, 23, 127, 23);
	
	for(i=0;i<CONTENT_LENGTH;++i)	//	显示文本内容字符串中的所有字符，每21个为一排，最多三排
	{
		if(Content[i] != 0)
		{
			OLED_ShowChar( i*6%126+1, 24+(i/21)*8, Content[i], OLED_6X8);
		}			
	}
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本没有存放任何文件而发出的警告
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议配合Delay函数使用
  */
static void TextMenu_Read_View_Warning(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 38, 0, 48, 48, Warning);
	OLED_ShowChinese( 0, 48, "没有存放任何文件");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本读取菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本读取菜单的功能函数
  */
static void TextMenu_Read_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	uint8_t Status = 0;	//	状态标志
	
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	读取已存储的文本个数
	
	if(TextNum == 0)	//	没有存放任何文件，给予警告并退出功能函数
	{
		TextMenu_Read_View_Warning();
		Delay_ms(1000);
		return;
	}
	
	TextMenu_Read_View_Status0();	//	显示状态零视图
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == 0)	//	状态零下
				{
					if(TextNum>=1)	//	判断是否有第一个文本
					{
						Status = 1;	//	转变状态为一
						TextMenu_Read_DisplayText(1);	//	显示存放的第一个文本
					}
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == 0)	//	状态零下
				{
					if(TextNum>=2)	//	判断是否有第二个文本
					{
						Status = 1;	//	转变状态为一
						TextMenu_Read_DisplayText(2);	//	显示存放的第二个文本
					}
				}
				else if(Status == 1)	//	状态一下
				{
					Status = 0;//	转变状态为零
					TextMenu_Read_View_Status0();	//	显示状态零视图
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == 0)	//	状态零下
				{
					if(TextNum>=3)	//	判断是否有第三个文本
					{
						Status = 1;	//	转变状态为一
						TextMenu_Read_DisplayText(3);	//	显示存放的第三个文本
					}
				}
				else if(Status == 1)	//	状态一下
				{
					Status = 0;//	转变状态为零
					TextMenu_Read_View_Status0();	//	显示状态零视图
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == 0)	//	状态零下
				{
					return;
				}
			}
			
			else
			{
				MSG_DefaultProc(&msg);	//	消息默认处理
			}
			
			OLED_Update();	//	刷新
		}
	}
}

/**
  * 函    数：初始化记事本读取菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了记事本读取菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void TextMenu_Read_Init(void)
{
	/*初始化记事本读取菜单*/
	Menu_Init(&TextMenu_Read);	
	TextMenu_Read.Funsion = TextMenu_Read_Funsion;
	TextMenu_Read.Icon = TextMenu_Read_Icon;
	TextMenu_Read.Name = TextMenu_Read_Name;
	/*初始化记事本读取菜单*/
	
	Store_Init();	//	初始化Store模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
}

/*********************功能函数*/
