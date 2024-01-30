#ifndef __KEY_H__
#define __KEY_H__

/*参数宏定义*********************/

/*用到的GPIO口引脚宏定义*/
#define KEY1_GPIO_PORT      ( GPIOB )
#define KEY1_GPIO_CLK 	    ( RCC_APB2Periph_GPIOB )
#define KEY1_GPIO_PIN		( GPIO_Pin_0 )

#define KEY2_GPIO_PORT      ( GPIOB )
#define KEY2_GPIO_CLK 	    ( RCC_APB2Periph_GPIOB )
#define KEY2_GPIO_PIN		( GPIO_Pin_1 )

#define KEY3_GPIO_PORT      ( GPIOB )
#define KEY3_GPIO_CLK 	    ( RCC_APB2Periph_GPIOB )
#define KEY3_GPIO_PIN		( GPIO_Pin_10 )

//#define KEY4_GPIO_PORT      ( GPIOB )
//#define KEY4_GPIO_CLK 	    ( RCC_APB2Periph_GPIOB )
//#define KEY4_GPIO_PIN		( GPIO_Pin_11 )
/*用到的GPIO口引脚宏定义*/

#define KEY_NOW_NUM         ( 3 )       //  按键个数(可自定义)
#define KEY_MAX_NUM         ( 32 )      //  按键最多个数(也可自定义，不过要响应的增加按键标识与状态变量的数据类型大小)

#define key_state_type      uint32_t    //  按键状态变量的类型，涉及到按键最多个数(KEY_MAX_NUM)的大小
                                            //  按键状态变量，配合按键标识与位操作，可以记录多个按键的按下松开状态，
                                            //  目前为32位，支持32个按键，可将状态变量改为64位变量，支持64按键，
                                            //  或者修改代码，利用数组等方式表示状态以支持更多按键
/*按键标识*/
#define NO_KEY_FLAG         (  0   )    //  无按键标识
#define KEY1_FLAG           (1 << 0)    //  按键1标识
#define KEY2_FLAG           (1 << 1)    //  按键2标识
#define KEY3_FLAG           (1 << 2)    //  按键3标识
#define KEY4_FLAG           (1 << 3)    //  按键4标识
#define KEY5_FLAG           (1 << 4)    //  按键5标识
#define KEY6_FLAG           (1 << 5)    //  按键6标识
#define KEY7_FLAG           (1 << 6)    //  按键7标识
#define KEY8_FLAG           (1 << 7)    //  按键8标识
#define KEY9_FLAG           (1 << 8)    //  按键9标识
#define KEY10_FLAG          (1 << 9)    //  按键10标识
#define KEY11_FLAG          (1 << 10)   //  按键11标识
#define KEY12_FLAG          (1 << 11)   //  按键12标识
#define KEY13_FLAG          (1 << 12)   //  按键13标识
#define KEY14_FLAG          (1 << 13)   //  按键14标识
#define KEY15_FLAG          (1 << 14)   //  按键15标识
#define KEY16_FLAG          (1 << 15)   //  按键16标识
#define KEY17_FLAG          (1 << 16)   //  按键17标识
#define KEY18_FLAG          (1 << 17)   //  按键18标识
#define KEY19_FLAG          (1 << 18)   //  按键19标识
#define KEY20_FLAG          (1 << 19)   //  按键20标识
#define KEY21_FLAG          (1 << 20)   //  按键21标识
#define KEY22_FLAG          (1 << 21)   //  按键22标识
#define KEY23_FLAG          (1 << 22)   //  按键23标识
#define KEY24_FLAG          (1 << 23)   //  按键24标识
#define KEY25_FLAG          (1 << 24)   //  按键25标识
#define KEY26_FLAG          (1 << 25)   //  按键26标识
#define KEY27_FLAG          (1 << 26)   //  按键27标识
#define KEY28_FLAG          (1 << 27)   //  按键28标识
#define KEY29_FLAG          (1 << 28)   //  按键29标识
#define KEY30_FLAG          (1 << 29)   //  按键30标识
#define KEY31_FLAG          (1 << 30)   //  按键31标识
#define KEY32_FLAG          (1 << 31)   //  按键32标识
/*按键标识*/

/*********************参数宏定义*/





/*函数声明*********************/

void Key_Init(void);
void Key_Loop(void);

/*********************函数声明*/

#endif
