/***************************************************************************************
  * 本源文件含有按键相关函数
  * GPIO口，发送的消息均可在对应的地方自定义
  ***************************************************************************************
  */
#include "key.h"
#include "stm32f10x.h"                  // Device header
#include "./MSG/MSG.h"
#include "./Timer/Timer.h"

#if defined USE_MSG             //  如果使用了MSG模块
#define USE_MSG_SEND_ERROR  1   //  非0表示使用MSG发送按键错误信息
#define USE_MSG_SEND_STATE  1   //  非0表示使用MSG发送按键状态信息
#endif

/**
  * 函    数：获取当前按键状态
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：可自定义GPIO口
  */
static key_state_type Key_GetState(void)
{
	key_state_type Key_State = NO_KEY_FLAG;   //  初始化按键状态变量为空
    
	if( 0 == GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN))	{	Key_State |= KEY1_FLAG;	}   //  检测按键1状态，在按键状态变量上对应位进行状态标记
	if( 0 == GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN))	{	Key_State |= KEY2_FLAG;	}   //  检测按键2状态，在按键状态变量上对应位进行状态标记
	if( 0 == GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN))	{	Key_State |= KEY3_FLAG;	}   //  检测按键3状态，在按键状态变量上对应位进行状态标记
	if( 0 == GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN))	{	Key_State |= KEY4_FLAG;	}   //  检测按键4状态，在按键状态变量上对应位进行状态标记
	
	return Key_State;   //  返回按键状态变量
}

/**
  * 函    数：按键循环函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议每20ms执行一次扫描各个按键状态，据此发送消息
  *           可根据多次按下来识别连按，或者接连判断按下来识别双击等
  *           发生的消息也可以在Message.h里自定义
  */
void Key_Loop(void)
{
	static key_state_type NowState,LastState;	//	存放上次扫描的按键状态和本次扫描的按键状态
    uint8_t i = 0;
    
#if defined (USE_MSG) && ( defined (USE_MSG_SEND_ERROR) || defined (USE_MSG_SEND_STATE) )   //  如果使用了MSG模块并且要用它发送错误或者状态信息
	MSG msg;                    //  消息变量，用于存放一个消息
#endif
    
	
	LastState = NowState;	    //	记录上次扫描的按键状态
	
	NowState = Key_GetState();	//	获取本次扫描的按键状态
	
    for(i=0;i<KEY_NOW_NUM;++i)
    {
        if( ( LastState & (0x00000001 << i) ) && ( !( NowState & (0x00000001 << i) ) ) )    //  对状态变量的每一位进行检测，判断是否有按键上次按下，本次没按下
        {
#if defined (USE_MSG) && defined (USE_MSG_SEND_STATE)   //  如果使用了MSG模块并且要用它发送状态信息
            msg.message = CM_KEYUP;                     //  配置消息类型为松开消息
            msg.param = VK_KEY1 + i;                    //  配置消息参数为当前检测到松开的按键的按键号
            msg.count = 1;                              //  配置消息参数的重复计数(不配置也行，MSG_SendMessage会处理)
            MSG_SendMessage(&msg);                      //  发送消息
#else
        //  如果你没有使用我写的MSG模块或者你使用了MSG模板，但是不想用MSG发送按键状态信息而定义USE_MSG_SEND_SATE宏为0
        //  你可以在此处处理按键松开 
#endif
        }
        else if( ( !( LastState & (0x00000001 << i) ) ) && ( NowState & (0x00000001 << i) ) )//  对状态变量的每一位进行检测，判断是否有按键上次没按下，本次按下
        {
#if defined (USE_MSG) && defined (USE_MSG_SEND_STATE)   //  如果使用了MSG模块并且要用它发送状态信息
            msg.message = CM_KEYDOWN;                   //  配置消息类型为按下消息
            msg.param = VK_KEY1 + i;                    //  配置消息参数为当前检测到按下的按键的按键号
            msg.count = 1;                              //  配置消息参数的重复计数(不配置也行，MSG_SendMessage会处理)
            MSG_SendMessage(&msg);                      //  发送消息
#else
        //  如果你没有使用我写的MSG模块或者你使用了MSG模板，但是不想用MSG发送按键状态信息而定义USE_MSG_SEND_SATE宏为0
        //  你可以在此处处理按键按下 
#endif
        }
    }
    
    if( ( NowState >= (1 << KEY_NOW_NUM) ) && (KEY_NOW_NUM < KEY_MAX_NUM) ) //  扫描出了不该出现的按键
    {
#if defined (USE_MSG) && defined (USE_MSG_SEND_ERROR)   //  如果使用了MSG模块并且要用它发送错误信息
        msg.message = CM_KEYERROR;                      //  配置消息类型为错误消息
        msg.param = VK_SCAN;                            //  配置消息参数为按键扫描
        msg.count = 1;                                  //  配置消息参数的重复计数(不配置也行，MSG_SendMessage会处理)           
        MSG_SendMessage(&msg);                          //  发送消息
#else
        //  如果你没有使用我写的MSG模块或者你使用了MSG模板，但是不想用MSG发送按键错误信息而定义USE_MSG_SEND_ERROR宏为0
        //  你可以在此处处理扫描错误
#endif
        return;
    }
}

/**
  * 函    数：按键初始化
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：打开对应的GPIOA口，可自定义
  */
void Key_Init(void)
{
    if( (!KEY_NOW_NUM) || (KEY_NOW_NUM > KEY_MAX_NUM) ) //  如果当前按键个数为0，或者当前按键个数大于按键最大个数
    {
#if defined (USE_MSG) && defined (USE_MSG_SEND_ERROR)   //  如果使用了MSG模块并且要用它发送状态信息
        MSG msg;                                        //  消息变量，用于存放一个消息
        msg.message = CM_KEYERROR;                      //  配置消息类型为错误消息
        msg.param = VK_INIT;                            //  配置消息参数为按键初始化        
        msg.count = 1;                                  //  配置消息参数的重复计数(不配置也行，MSG_SendMessage会处理)
        MSG_SendMessage(&msg);                          //  发送消息
#else
        //  如果你没有使用我写的MSG模块或者你使用了MSG模板，但是不想用MSG发送错误信息而没定义USE_MSG宏(MSG.h默认定义了)
        //  你可以在此处处理扫描错误
#endif
        return;
    }
	GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd( KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK | KEY4_GPIO_CLK , ENABLE);
	
	/*初始化按键使用的GPIO口为上拉输入模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( KEY1_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( KEY2_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( KEY3_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( KEY4_GPIO_PORT, &GPIO_InitStructure);
	/*初始化按键使用的GPIO口为上拉输入模式*/
	
#ifdef USE_TIMER    //  如果使用了Timer模块
	/*将Key_Loop函数添加到定时列表每20ms执行一次*/
	Timer_AddFunction( Key_Loop, 20);   //  将 Key_Loop 函数添加到定时列表，执行周期为 20ms
	Timer_Start(Key_Loop);              //  开始为定时列表中的 Key_Loop 函数计数定时
	/*将Key_Loop函数添加到定时列表每20ms执行一次*/
#endif
}
