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

#include "string.h"
#include "./key/key.h"
#include "./MSG/MSG.h" 
#include "./OLED/OLED.h"
#include "./Timer/Timer.h"
#include "./i2c/bsp_i2c.h"
#include "./led/bsp_led.h"
#include "./MG310/MG310.h"
#include "./exti/bsp_exti.h" 
#include "./MPU6050/MPU6050.h"
#include "./systick/bsp_SysTick.h"
#include "./BlueTooth/BlueTooth.h"
#include "./BalanceCar/BalanceCar.h"

//  调试模式标识( 此调试并非Keil5的调试功能，而是显示平衡小车当前的直立环与速度环数据 )
uint8_t debug_mode_flag = 0;
//  按键双击标识(当有按键按下使置1，当有按键松开和检测到双击状态时置0，一个按键按下时判断此变量是否为1即可检测是否为双击状态)
uint8_t double_click_flag = 0;
    
int main(void)
{
    OLED_Init();            //  初始化OLED模块
    
    OLED_Printf(0,0,OLED_6X8,"( 1/11");           //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
	SysTick_Init();         //  初始化SysTick(系统定时器)
    SysTick_Cmd(ENABLE);    //  使能SysTick(系统定时器)
    
    OLED_Printf(0,0,OLED_6X8,"( 2/11)");          //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    BLT_Init();             //  初始化蓝牙模块
    
    OLED_Printf(0,0,OLED_6X8,"( 3/11)");          //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    EXTI_Pxy_Config();      //  初始化EXTI外部中断
    
    OLED_Printf(0,0,OLED_6X8,"( 4/11)");          //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
	I2C_Bus_Init();         //  初始化I2C(专用于MPU6050模块的，OLED模块的I2C相关代码在OLED.c中已有)
    
    OLED_Printf(0,0,OLED_6X8,"( 5/11)");           //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    MSG_Init();             //  初始化MSG(消息队列)模块(自己写的一个简易库，用于统一管理各个模块，并非硬件外设)
    
    OLED_Printf(0,0,OLED_6X8,"( 6/11)");           //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    Timer_Init();           //  初始化Timer模块(自己写的一个简易库，用于统一管理各个模块，并非硬件外设)
    
    OLED_Printf(0,0,OLED_6X8,"( 7/11)");         //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    Key_Init();             //  初始化按键
    
    OLED_Printf(0,0,OLED_6X8,"( 8/11)");           //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    MPU6050_DMP_Init();     //  初始化MPU6050(使用DMP库)
    
    OLED_Printf(0,0,OLED_6X8,"( 9/11)");       //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    MG310_Init();           //  初始化MG310电机、编码器
    
    OLED_Printf(0,0,OLED_6X8,"(10/11)");        //  OLED屏幕显示初始化进度(需刷新)
    OLED_UpdateArea(0,0,128,8);
    
    BalanceCar_Init();      //  平衡小车初始化
    
    OLED_Printf(0,0,OLED_6X8,"Init SUCCEED");           //  OLED屏幕显示初始化成功信息(需刷新)
    OLED_Printf(0,16,OLED_6X8,"BlueTooth_Mode:");       //  OLED屏幕显示蓝牙模式标识(需刷新)
    OLED_Printf(0,24,OLED_6X8,"none");                  //  OLED屏幕显示无模式标识(需刷新)
    OLED_Printf(0,32,OLED_6X8,"BlueTooth_Command:");    //  OLED屏幕显示蓝牙指令标识(需刷新)
    OLED_Printf(0,40,OLED_6X8,"none");                  //  OLED屏幕显示无蓝牙指令标识(需刷新)
    OLED_Update();
    
    while(1)
    {   
        MSG msg;        //  消息变量，用于存放一个消息
        
        if(MSG_GetMessage(&msg))    //  从消息队列中获取消息
        {
            switch(msg.message)     //  辨别消息
            {
                case CM_KEYDOWN:{   //  按键按下消息
                    if(double_click_flag != 0)  //  当前是双击状态
                    {
                        debug_mode_flag = !debug_mode_flag; //  开关调试模式
                        if(debug_mode_flag) //  如果打开了调试模式
                        {
                            MG310_Close();  //  关闭电机与编码器
                        }
                        else                //  如果关闭了调试模式
                        {
                            MG310_Init();   //  重新初始化电机与编码器，将它们再次打开
                            OLED_Clear();   //  OLED屏幕清屏
                            OLED_Printf( 0, 0,OLED_6X8,"Debug Completion!");    //  OLED屏幕显示调试完成信息(需刷新)
                            OLED_Printf( 0,32,OLED_6X8,"BlueTooth_Command:");   //  OLED屏幕显示蓝牙指令标识(需刷新)
                            OLED_Update();
                        }
                        double_click_flag = 0;  //  处理了双击，所以将双击状态标识置为0
                    }
                    else    //  当前还不是双击状态
                    {
                        double_click_flag = 1;  //  置1，表示当前已经是双击状态()
                    }
                    break;
                }
                case CM_KEYUP:{   //  按键松开消息
                    double_click_flag = 0;      //  将双击状态置为0
                    break;
                }
                case CM_BLTCMD:{   //  蓝牙指令消息
                    switch(msg.param)
                    {
                        case BLT_MOVE_MODE:{    
                            OLED_ClearArea(0,24,128,8);                     //  OLED屏幕将上一条模式标识清除
                            OLED_Printf(0,24,OLED_6X8, "move_mode");        //  OLED屏幕显示移动模式标识(需刷新)
                            BC.mode = BLT_MOVE_MODE;                        //  变更小车模式为移动模式(只实现了这一个模式)
                            break;
                        }
                        case BLT_QUIT:{
                            OLED_ClearArea(0,24,128,8);                     //  OLED屏幕将上一条模式标识清除
                            OLED_Printf(0,24,OLED_6X8, "none");             //  OLED屏幕显示无模式标识(需刷新)
                            OLED_Printf(0,40,OLED_6X8, "none");             //  OLED屏幕显示无蓝牙指令标识(需刷新)
                            BC.mode = 0;                                    //  变更小车模式为无模式
                            break;
                        }
                        default:{
                            OLED_ClearArea(0,40,128,8);                     //  OLED屏幕将上一条蓝牙指令清除
                            OLED_ClearArea(0,48,128,8);                     //  OLED屏幕将可能存在的错误指令标识清除
                            switch(msg.param)
                            {
                                case BLT_ADVANCE:{
                                    OLED_Printf(0,40,OLED_6X8, "advance");  //  OLED屏幕显示前进指令标识(需刷新)
                                    BalanceCar_SetSpeed(100.0f);            //  设置小车速度，使其前进
                                    break;
                                }
                                case BLT_BACK:{
                                    OLED_Printf(0,40,OLED_6X8, "back");     //  OLED屏幕显示后退指令标识(需刷新)
                                    BalanceCar_SetSpeed(-100.0f);           //  设置小车速度，使其后退
                                    break;
                                }
                                case BLT_BRAKE:{
                                    OLED_Printf(0,40,OLED_6X8, "brake");    //  OLED屏幕显示制动指令标识(需刷新)
                                    BalanceCar_SetSpeed(0.0f);              //  设置小车速度，使其不再前进或后退
                                    break;
                                }
                                case BLT_STOP:{
                                    OLED_Printf(0,40,OLED_6X8, "stop");     //  OLED屏幕显示停止指令标识(需刷新)
                                    BalanceCar_SetSpeed(0.0f);              //  设置小车速度，使其不再前进或后退
                                    BalanceCar_SetTurn(0.0f);               //  设置小车转向，使其不再朝左或朝右旋转
                                    break;
                                }
                                case BLT_LEFT:{
                                    OLED_Printf(0,40,OLED_6X8, "left");     //  OLED屏幕显示朝左指令标识(需刷新)
                                    BalanceCar_SetTurn(50.0f);              //  设置小车转向，使其朝左旋转
                                    break;
                                }
                                case BLT_RIGHT:{
                                    OLED_Printf(0,40,OLED_6X8, "right");     //  OLED屏幕显示朝右指令标识(需刷新)
                                    BalanceCar_SetTurn(-50.0f);              //  设置小车转向，使其朝右旋转
                                    break;
                                }
                                default:{                                    //  OLED屏幕显示错误指令标识(需刷新)
                                    OLED_Printf(0,48,OLED_6X8,"Unknown Command!!!");
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    
                    OLED_UpdateArea(0,24,128,8);    //  OLED屏幕刷新模式标识
                    OLED_UpdateArea(0,40,128,8);    //  OLED屏幕刷新蓝牙指令标识
                    OLED_UpdateArea(0,48,128,8);    //  OLED屏幕刷新可能存在的错误指令标识
                    
                    break;
                }
                default:{           //  默认消息处理
                    MSG_DefaultProc(&msg);      //  用户不处理的消息会交给 MSG_DefaultProc 函数进行默认处理
                    break;
                }
            }
            
            if(debug_mode_flag)     //  每有一条消息发送过来，就检测是否为调试模式，如果是，则显示平衡小车的直立环与速度环数据
            {
                OLED_Clear();       //  OLED屏幕清屏
                OLED_Printf( 0, 0,OLED_6X8,"angle = %.4f", BC.angle.current);                               //  显示平衡小车的倾斜角度(需刷新)
                OLED_Printf( 0, 8,OLED_6X8,"angle - pOut = %5d", BC.angle.p_out);                           //  显示直立环的 pOut(需刷新)
                OLED_Printf( 0,16,OLED_6X8,"angle - dOut = %5d", BC.angle.d_out);                           //  显示直立环的 dOut(需刷新)
                OLED_Printf( 0,24,OLED_6X8,"speed - pOut = %5d", BC.speed.p_out);                           //  显示速度环的 pOut(需刷新)
                OLED_Printf( 0,32,OLED_6X8,"speed - iOut = %5d", BC.speed.i_out);                           //  显示速度环的 iOut(需刷新)
                OLED_Printf( 0,40,OLED_6X8,"angle - out = %5d", BC.angle.total_out);                        //  显示直立环的总输出(需刷新)
                OLED_Printf( 0,48,OLED_6X8,"speed - out = %5d", BC.speed.total_out);                        //  显示速度环的总输出(需刷新)
                OLED_Printf( 0,56,OLED_6X8,"total - out = %5d", BC.angle.total_out + BC.speed.total_out);   //  显示总输出(需刷新)
                OLED_Update();      //  OLED屏幕全屏刷新
            }
            /*
                ps:不用担心调试模式下没有消息发送过来，导致 MSG_GetMessage 返回 0 不进入if语句而显示不了数据
                事实上消息完全不缺，调试模式下OLED能一直刷新数据显示
            */
            
        }
    }	
}
