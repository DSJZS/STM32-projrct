/***************************************************************************************
  * 本菜单为记事本菜单的二级子菜单，用于创建新的文本文件，并且保存结果到FLASH
  * FLASH最后一页的第三个字节表示存放的记事本文件个数
  * 记事本文件从FLASH第二十一个半字开始存储，每个记事本文件大小为 Title_LENGTH + Title_LENGTH
  * 记事本文件最多存放 TEXT_MAX 个
  ***************************************************************************************
  */

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

/*	记事本新建菜单图标	——	为一支铅笔	*/
const uint8_t TextMenu_Create_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x80,0x40,
	0x20,0x10,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x04,0x82,0x41,0x23,0x17,0x0F,
	0x1E,0x3C,0x78,0xF0,0xE0,0xC0,0x80,0x01,0x02,0x04,0x88,0x50,0x20,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,
	0x10,0x08,0x04,0x82,0x41,0x20,0x10,0x08,0x04,0x02,0x01,0x80,0x40,0x20,0x10,0x08,
	0x04,0x02,0x01,0x80,0x41,0x23,0x17,0x0F,0x0E,0x05,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xA0,0x10,0x08,0x04,0x82,0x41,0x20,0x10,0x08,
	0x04,0x02,0x01,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x80,0x40,0x20,0x10,0x08,
	0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF0,0xF1,0xE2,0xC5,0x88,0x10,0x20,0x50,0x88,
	0x04,0x02,0x01,0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,
	0x03,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	记事本新建菜单结构体	*/
Menu TextMenu_Create;	

/*	记事本新建菜单名字	*/
const char* TextMenu_Create_Name = "新建";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：记事本新建菜单的状态零视图显示
  * 参    数：Title		文本标题字符串
  * 参    数：c			选中字符
  * 参    数：CharNum	当前字符个数
  * 参    数：Limit		字符个数上限
  * 返 回 值：无
  * 说    明：用于记事本新建菜单状态零视图显示
  */
static void TextMenu_Create_View_Status0( char* Title, char c, uint8_t CharNum, uint8_t Limit)
{
	uint8_t i = 0;	//	循环计数变量

	OLED_Clear();	//	清屏
	
	OLED_ShowChinese( 0, 0, "当前选中字符：");
	OLED_ShowChar( 112, 0, c, OLED_8X16);		//	显示被选中的字符
	OLED_DrawLine( 0, 15, 127, 15);
	OLED_ShowChinese( 0, 16, "输入新文本名字：");
	OLED_Printf( 91, 32, OLED_6X8 , "%2d/%2d", CharNum, Limit);	//	显示当前字符个数与字符个数上限
	OLED_DrawLine( 0, 39, 127, 39);
	
	OLED_ReverseArea( CharNum*6%126+1, 40+(CharNum/21)*8, 6, 8);
	
	for(i=0;i<CharNum;++i)	//	显示文本标题字符串中的所有字符，每21个为一排，最多三排
	{
		OLED_ShowChar( i*6%126+1, 40+(i/21)*8, Title[i], OLED_6X8);
		OLED_Printf( 91, 32, OLED_6X8 , "%2d/%2d", CharNum, Limit);	//	显示当前字符个数与字符个数上限
		OLED_DrawLine( 0, 39, 127, 39);
	}
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本新建菜单的状态一视图显示
  * 参    数：Content	文本内容字符串
  * 参    数：c			选中字符
  * 参    数：CharNum	当前字符个数
  * 参    数：Limit		字符个数上限
  * 返 回 值：无
  * 说    明：用于记事本新建菜单状态一视图显示
  */
static void TextMenu_Create_View_Status1( char* Content, char c, uint8_t CharNum, uint8_t Limit)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	OLED_ShowChinese( 0, 0, "当前选中字符：");
	OLED_ShowChar( 112, 0, c, OLED_8X16);		//	显示被选中的字符
	OLED_DrawLine( 0, 15, 127, 15);
	OLED_Printf( 75, 16, OLED_6X8 , "%3d/%3d", CharNum, Limit);	//	显示当前字符个数与字符个数上限
	OLED_DrawLine( 0, 23, 127, 23);
	
	OLED_ReverseArea( CharNum*6%126+1, 24+(CharNum/21)*8, 6, 8);
	
	for(i=0;i<CharNum;++i)	//	显示文本内容字符串中的所有字符，每21个为一排，最多三排
	{
		OLED_ShowChar( i*6%126+1, 24+(i/21)*8, Content[i], OLED_6X8);			
		OLED_Printf( 75, 16, OLED_6X8 , "%3d/%3d", CharNum, Limit);	//	显示当前字符个数与字符个数上限
		OLED_DrawLine( 0, 23, 127, 23);
	}
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本新建菜单的状态二视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本新建菜单状态二视图显示，提示是否保存
  */
static void TextMenu_Create_View_Status2(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);
	OLED_ShowImage(70,0,20,48,Button);

	OLED_ShowChinese( 3, 48,  "保存");
	OLED_ShowChinese( 34, 48, "退出");
	OLED_ShowChinese( 65, 48, "取消");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：将记事本文件保存到Flash的最后一页之中
  * 参    数：Title		文本标题字符串
  * 参    数：Content	文本内容字符串
  * 返 回 值：无
  * 说    明：FLASH最后一页的第三个字节表示存放的记事本文件个数
  *           记事本文件从第二十一个半字开始存储，每个记事本文件大小为 Title_LENGTH + Title_LENGTH
  *           记事本文件最多存放 TEXT_MAX 个
  */
static void TextMenu_Save( char* Title, char* Content)
{
	uint8_t i = 0;	//	循环计数变量
	
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	获取已存放的记事本文件个数
	
	if(  TextNum < TEXT_MAX )
	{
		for(i=0;i<TITLE_LENGTH;++i)	//	存放标题
		{
			*((uint8_t*)Store_Data+40+TextNum*(TITLE_LENGTH+CONTENT_LENGTH)+i) = Title[i];
		}
		for(i=0;i<CONTENT_LENGTH;++i)	//	存放内容
		{
			*((uint8_t*)Store_Data+40+TextNum*(TITLE_LENGTH+CONTENT_LENGTH)+TITLE_LENGTH+i) = Content[i];
		}
		*((uint8_t*)Store_Data+2) = TextNum+1;	//	记事本文件个数加一
		Store_Save();	//	保存到FLASH
	}
}

/**
  * 函    数：记事本文本数量达到上限而发出的警告
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议配合Delay函数使用
  */
static void TextMenu_Create_View_Warning(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 38, 0, 48, 48, Warning);
	OLED_ShowChinese( 7, 48, "文本数已达上限");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：记事本新建菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于记事本新建菜单的功能函数
  */
static void TextMenu_Create_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	char c = ' ';	//	当前选中字符
	
	char Title[TITLE_LENGTH+1] = {0};		//	文本标题字符串
	char Content[CONTENT_LENGTH+1] = {0};	//	文本内容字符串
	
	uint8_t Status = 0;		//	状态标志
	uint8_t CharNum = 0;	//	当前字符个数
	uint8_t Limit = 63;		//	当前字符上限
	uint8_t TextNum = *((uint8_t*)Store_Data+2);	//	读取已存储的文本个数
	
	if(TextNum >= TEXT_MAX)	//	文本个数达到上限，给予警告并退出功能函数
	{
		TextMenu_Create_View_Warning();
		Delay_ms(1000);
		return;
	}
	
	TextMenu_Create_View_Status0(Title,c,CharNum,Limit);	//	显示状态零视图
	
	
	while(1)
	{

		if(MSG_GetMessage(&msg))	//	获取消息
		{
			if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(c < ' '+94)	//	限制c超出可见字符
				{
					++c;	//	c变为下一个可见字符
					OLED_ShowChar( 112, 0, c, OLED_8X16);
					OLED_DrawLine( 112, 15, 127, 15);
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(c > ' ')		//	限制c脱离可见字符
				{
					--c;	//	c变为上一个可见字符
					OLED_ShowChar( 112, 0, c, OLED_8X16);
					OLED_DrawLine( 112, 15, 127, 15);
				}
			}
						
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == 0)	//	状态零下
				{
					if(CharNum<TITLE_LENGTH)	//	检测文本标题字符串是否已满
					{
						Title[CharNum] = c;	//	添加新字符到文本标题字符串
						++CharNum;	//	当前字符个数加一
						TextMenu_Create_View_Status0( Title, c, CharNum, Limit);	//	显示视图
					}
				}
				if(Status == 1)	//	状态一下
				{
					if(CharNum<CONTENT_LENGTH)	//	检测文本标题字符串是否已满
					{
						Content[CharNum] = c;	//	添加新字符到文本内容字符串
						++CharNum;	//	当前字符个数加一
						TextMenu_Create_View_Status1( Content, c, CharNum, Limit);	//	显示视图
					}
				}
				if(Status == 2)	//	状态二下
				{
					TextMenu_Save( Title, Content);	//	保存
					break;
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == 0)	//	状态零下
				{
					if(CharNum>0)	//	检测文本标题字符串是否有字符
					{
						Title[CharNum] = 0;	//	从文本标题字符串删除字符
						--CharNum;	//	当前字符个数减一
						TextMenu_Create_View_Status0(Title,c,CharNum,Limit);	//	显示视图
					}
				}
				if(Status == 1)	//	状态一下
				{
					if(CharNum>0)	//	检测文本内容字符串是否有字符
					{
						Content[CharNum] = 0;	//	从文本内容字符串删除字符
						--CharNum;	//	当前字符个数减一
						TextMenu_Create_View_Status1( Content, c, CharNum, Limit);	//	显示视图
					}
				}
				if(Status == 2)	//	状态二下
				{
					return;	//	退出
				}
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == 0)	//	状态零下
				{
					Status = 1;		//	转变状态为一
					CharNum = 0;	//	清零当前字符个数，重新计算文本字符串个数
					Limit = 105;	//	设置文本字符串的上限
					TextMenu_Create_View_Status1( Content, c, CharNum, Limit);	//	显示视图
				}
				else if(Status == 1)	//	状态一下
				{
					Status = 2;	//	转变状态为二
					TextMenu_Create_View_Status2();	//	显示视图
				}
				else if(Status == 2)	//	状态二下
				{
					Status = 1;	//	转变状态为一
					TextMenu_Create_View_Status1( Content, c, CharNum, Limit);	//	显示视图
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
  * 函    数：初始化记事本新建菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了记事本新建菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void TextMenu_Create_Init(void)
{
	/*初始化记事本新建菜单*/
	Menu_Init(&TextMenu_Create);
	TextMenu_Create.Funsion = TextMenu_Create_Funsion;
	TextMenu_Create.Icon = TextMenu_Create_Icon;
	TextMenu_Create.Name = TextMenu_Create_Name;
	/*初始化记事本新建菜单*/
	
	Store_Init();	//	初始化Store模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
}

/*********************功能函数*/
