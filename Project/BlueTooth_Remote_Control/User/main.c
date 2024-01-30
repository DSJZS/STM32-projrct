////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//              ."" '<  `.___\_<|>_/___.'  >'"".                  //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//      ========`-.____`-.___\_____/___.-`____.-'========         //
//                           `=---='                              //
//      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^        //
//         佛祖保佑       永无BUG     永不修改                    //
////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "./SysTick/bsp_SysTick.h"
#include "./OLED/OLED.h"
#include "./Timer/Timer.h"
#include "./MSG/MSG.h"
#include "./GUI/GUI.h"
#include "./key/key.h"
#include "./Encoder/Encoder.h"
#include "./BlueTooth/BlueTooth.h"
int main()
{
    SysTick_Init();
    SysTick_Cmd(ENABLE);
    
    BLT_Init();     //  初始化蓝牙模块
    
    OLED_Init();	//	初始化OLED模块
	Timer_Init();	//	初始化Timer模块
    
    MSG_Init();	//	初始化消息模块
	
	Key_Init();	//	初始化按键模块
	Encoder_Init();	//	初始化旋转编码器模块
	
	GUI_Init();	//	初始化GUI模块
	GUI_Display();	//	调用GUI显示函数
    
    while(1)
    {
        MSG msg;
        if( MSG_GetMessage(&msg) )	//	接收消息
		{
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				GUI_ReceiveCommand(GUI_COMMAND_UP);	//	对GUI发送向上指令
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				GUI_ReceiveCommand(GUI_COMMAND_DOWN);	//	对GUI发送向下指令
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				GUI_ReceiveCommand(GUI_COMMAND_RIGHT);	//	对GUI发送向左指令
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				GUI_ReceiveCommand(GUI_COMMAND_LEFT);	//	对GUI发送向右指令
			}
			
			else
			{
				MSG_DefaultProc(&msg);	//	默认处理消息
			}
			GUI_Display();	//	调用GUI显示函数
		}
    }
}
