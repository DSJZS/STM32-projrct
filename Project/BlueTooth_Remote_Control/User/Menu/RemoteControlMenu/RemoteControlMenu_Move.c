/***************************************************************************************
  * 本菜单为遥控菜单的二级菜单
  * 用于远程控制小车移动
  ***************************************************************************************
  */
  
#include "RemoteControlMenu_Move.h"
#include "stm32f10x.h"                  // Device header
#include "../Menu.h"
#include "../../OLED/OLED.h"
#include "../../MSG/MSG.h"
#include "../../BlueTooth/BlueTooth.h"
#include "../../SysTick/bsp_SysTick.h"
/*图像数据*********************/

/*	遥控移动菜单图标	——	为一个滚动的轮胎	*/
const uint8_t RCMenu_Move_Icon[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xF8,
    0xF0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,
    0xB0,0x30,0x18,0x18,0x18,0x0C,0x0C,0xFC,0xFC,0x0C,0x0C,0x18,0x18,0x18,0x30,0xB1,
    0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x20,0x40,0x00,0x00,0x80,0x00,0x00,0xE0,0xFC,0x9F,0x83,0x80,0x81,
    0x83,0x87,0x8E,0x9C,0xB8,0xF0,0xE0,0xFF,0xFF,0xE0,0xF0,0xB8,0x9C,0x8E,0x87,0x83,
    0x81,0x80,0x83,0x9F,0xFC,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x20,0x82,0x29,0x2A,0x40,0x15,0x48,0x00,0xA0,0x47,0x3F,0xF9,0xC1,0x01,0x81,
    0xC1,0xE1,0x71,0x39,0x1D,0x0F,0x07,0xFF,0xFF,0x07,0x0F,0x1D,0x39,0x71,0xE1,0xC1,
    0x81,0x01,0xC1,0xF9,0x3F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x30,0x2A,0x21,0x20,0x30,0x09,0x05,0x02,0x02,0x02,0x05,0x0A,0x14,0x29,0x2F,0x37,
    0x2D,0x2C,0x38,0x38,0x38,0x30,0x30,0x3F,0x3F,0x30,0x30,0x38,0x38,0x38,0x2C,0x2D,
    0x27,0x13,0x11,0x10,0x10,0x08,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x08,0x10,0x20,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	遥控移动菜单结构体	*/
Menu RCMenu_Move;	

/*	遥控移动菜单名字	*/
const char* RCMenu_Move_Name = "移动";

/*********************全局变量*/





/*功能函数*********************/

/**
  * 函    数：用于显示遥控移动菜单的界面
  * 参    数：无
  * 返 回 值：无
  * 说    明：无
  */
static void RCMenu_Move_View(void)
{
    OLED_ShowChinese(0, 0,"键一前进键二后退");
    OLED_ShowChinese(0,16,"键三停止键四退出");
    OLED_ShowChinese(0,32,"编码器左转则左转");
    OLED_ShowChinese(0,48,"编码器右转则右转");
    OLED_Update();
}

/**
  * 函    数：遥控移动菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于遥控移动菜单的功能函数
  */
void RCMenu_Move_Funsion(void)
{
	MSG msg;
    RCMenu_Move_View();
    BLT_SendCommand( BLT_MOVE_MODE );
    while(1)
    {
        if(MSG_GetMessage(&msg))
        {
            switch(msg.message)
            {
                case CM_KEYDOWN:{
                    switch(msg.param)
                    {
                        case VK_KEY1:{
                            BLT_SendCommand( BLT_ADVANCE );
                            OLED_ReverseArea(0,0,64,16);
                            OLED_UpdateArea(0,0,64,16);
                            break;
                        }
                        case VK_KEY2:{
                            BLT_SendCommand( BLT_BACK );
                            OLED_ReverseArea(64,0,64,16);
                            OLED_UpdateArea(64,0,64,16);
                            break;
                        }
                        case VK_KEY3:{
                            BLT_SendCommand( BLT_STOP );
                            OLED_ReverseArea(0,16,64,16);
                            OLED_UpdateArea(0,16,64,16);
                            break;
                        }
                        case VK_KEY4:{
                            BLT_SendCommand( BLT_STOP);
                            OLED_ReverseArea(64,16,64,16);
                            OLED_UpdateArea(64,16,64,16);
                            break;
                        }
                    }
                    break;
                }
                case CM_KEYUP:{
                    switch(msg.param)
                    {
                        case VK_KEY1:{
                            BLT_SendCommand( BLT_BRAKE );
                            OLED_ReverseArea(0,0,64,16);
                            OLED_UpdateArea(0,0,64,16);
                            break;
                        }
                        case VK_KEY2:{
                            BLT_SendCommand( BLT_BRAKE );
                            OLED_ReverseArea(64,0,64,16);
                            OLED_UpdateArea(64,0,64,16);
                            break;
                        }
                        case VK_KEY3:{
                            BLT_SendCommand( BLT_STOP );
                            OLED_ReverseArea(0,16,64,16);
                            OLED_UpdateArea(0,16,64,16);
                            break;
                        }
                        case VK_KEY4:{
                            BLT_SendCommand( BLT_STOP );
                            OLED_ReverseArea(64,16,64,16);
                            OLED_UpdateArea(64,16,64,16);
                            BLT_SendCommand( BLT_QUIT );
                            return;
                            //break;
                        }
                    }
                    break;
                }
                case CM_ROATATING:{
                    switch(msg.param)
                    {
                        case EK_LEFT:{
                            BLT_SendCommand( BLT_LEFT );
                            OLED_ReverseArea(0,32,128,16);
                            OLED_UpdateArea(0,32,128,16);
                            delay_ms(50);
                            OLED_ReverseArea(0,32,128,16);
                            OLED_UpdateArea(0,32,128,16);
                            break;
                        }
                        case EK_RIGHT:{
                            BLT_SendCommand( BLT_RIGHT );
                            OLED_ReverseArea(0,48,128,16);
                            OLED_UpdateArea(0,48,128,16);
                            delay_ms(50);
                            OLED_ReverseArea(0,48,128,16);
                            OLED_UpdateArea(0,48,128,16);
                            break;
                        }
                    }
                    break;
                }
                default:
                    MSG_DefaultProc(&msg);
            }
        }
    }
}

/**
  * 函    数：初始化遥控移动菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了遥控移动菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void RCMenu_Move_Init(void)
{
	Menu_Init(&RCMenu_Move);
	RCMenu_Move.Funsion = RCMenu_Move_Funsion;
	RCMenu_Move.Icon = RCMenu_Move_Icon;
	RCMenu_Move.Name = RCMenu_Move_Name;
}

/*********************功能函数*/
