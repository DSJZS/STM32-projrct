/***************************************************************************************
  * 本菜单为游戏菜单的二级子菜单，用执行俄罗斯方块游戏，并且保存四个最高的得分记录到FLASH
  * FLASH最后一页的第三四五六个半字由高到低存放得分最高的4个记录
  ***************************************************************************************
  */

#include "GameMenu_Tetris.h"
#include "stm32f10x.h"  // Device header

#include "Key.h"
#include "OLED.h"
#include "Menu.h"
#include "Store.h"
#include "Timer.h"
#include "Encoder.h"

#include "MSG.h"

/*图像数据*********************/

/*	俄罗斯方块游戏菜单图标	——	为四个方块呈T形排列	*/
const uint8_t GameMenu_Tetris_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x80,0xC0,0xE0,0xF0,0xE0,0xC0,0xFE,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,
	0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x80,0x00,0xF8,0x1C,
	0x3E,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3E,0x1C,0xF8,0x80,0x00,0xFF,
	0x80,0x40,0x20,0x10,0x08,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xE0,0x70,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,
	0xF8,0x70,0xE0,0x01,0x03,0xFF,0x03,0x01,0x80,0x40,0x20,0x90,0xCF,0xE0,0xF1,0xE1,
	0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7F,0x80,0x00,0x01,0x03,0x07,0x0F,0xFF,0x0F,0x07,0x03,0x01,
	0x80,0x40,0x3F,0x02,0x04,0x07,0x02,0xF9,0x1C,0x3E,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0x7F,0x3E,0x1C,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x1F,0x08,0x04,0x02,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x10,0x20,0x40,0x80,0x01,0x03,0xFF,0x03,
	0x01,0x80,0x40,0x20,0x10,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x02,
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*	俄罗斯方块游戏里的方块	4W - 4H	*/
const uint8_t Cube[] = {
	0x1F,0x1F,0x1F,0x1F,0x1F,
};

/*********************图像数据*/





/*全局变量*********************/

/*	俄罗斯方块游戏菜单结构体	*/
Menu GameMenu_Tetris;	

/*	俄罗斯方块游戏菜单名字	*/
const char* GameMenu_Tetris_Name = "俄罗斯方块";

/*	每个方块图形的每一个旋转图形	*/
const uint8_t Data[7][4][4] = {
	//	@
	// @@@
	0x00,0x02,0x07,0x00,0x00,0x02,0x03,0x02,0x00,0x07,0x02,0x00,0x00,0x02,0x06,0x02,
	//	@@
	//	 @
	//	 @
	0x00,0x06,0x02,0x02,0x00,0x01,0x07,0x00,0x00,0x02,0x02,0x03,0x00,0x07,0x04,0x00,
	//	@@
	//	@
	//	@
	0x00,0x03,0x02,0x02,0x00,0x07,0x01,0x00,0x00,0x01,0x01,0x03,0x00,0x04,0x07,0x00,
	// @@
	//	@@
	0x00,0x06,0x03,0x00,0x00,0x01,0x03,0x02,0x00,0x06,0x03,0x00,0x00,0x01,0x03,0x02,
	// 	@@
	// @@
	0x00,0x03,0x06,0x00,0x00,0x02,0x03,0x01,0x00,0x03,0x06,0x00,0x00,0x02,0x03,0x01,
	// 	@@
	// 	@@
	0x00,0x03,0x03,0x00,0x00,0x03,0x03,0x00,0x00,0x03,0x03,0x00,0x00,0x03,0x03,0x00,
	//  
	// 	@@@@
	0x00,0x00,0x0f,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x0f,0x00,0x01,0x01,0x01,0x01
};

/*	沿纵坐标显示的Score	*/
const uint8_t Tetris_ScoreImage[] = {
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xC0,0xF8,0xF8,0x18,0xF8,0xF8,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0xFC,0x0C,0x0C,0x0C,0x0C,0xFC,0xFC,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7C,0x7C,0x6C,0x6C,0x6C,0x6C,0x7C,0x7C,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x0E,0x1E,0x3E,0x76,0x66,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0x7E,0x06,0x7E,0x7E,0x66,0x7E,0x7E,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
};

/*	沿纵坐标显示的数字	*/
const uint8_t Tetris_NumImage[10][5] = {
	0x07,0x05,0x05,0x05,0x07,	//	0
	0x02,0x02,0x02,0x02,0x02,	//	1
	0x07,0x01,0x07,0x04,0x07,	//	2
	0x07,0x04,0x07,0x04,0x07,	//	3
	0x04,0x04,0x07,0x05,0x05,	//	4
	0x07,0x04,0x07,0x01,0x07,	//	5
	0x07,0x05,0x07,0x01,0x07,	//	6
	0x02,0x02,0x02,0x04,0x07,	//	7
	0x07,0x05,0x07,0x05,0x07,	//	8
	0x04,0x04,0x07,0x05,0x07,	//	9
};

/*	方块容器，存放堆积的方块图形	*/
static uint16_t ball[26] = {0};
//	初始化时如下图，1代表边界
//	1110000000000111	*	0-3		方块图形刷新区域
//	1110000000000111	*
//	1110000000000111	*
//	1110000000000111	*
//	1110000000000111	-	4-23	方块图形显示区域
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1110000000000111	-
//	1111111111111111	+	24-25	容器底部
//	1111111111111111	+	

/*	存放下降的方块图形	*/
static uint16_t fall[4] = {0};
//	初始化时如下图
//	0000000000000000	-	存储正在下降的方块
//	0000000000000000	-
//	0000000000000000	-
//	0000000000000000	-
//				   ^
//				   |
//每个新建方块图形均存放到最右端，靠方块图形左移计数左右移动

/*	俄罗斯方块游戏结构体	*/
static struct {
	int8_t bottom;		//	4-24	方块图形底部计数，表示方块最底部在容器的哪一行
	int8_t LShift;		//	3-		方块图形左移计数，方块最右边相较于容器最右边(bit0)左移多少位
	int8_t shape;		//	0-6		方块图形形状标志，表示当前方块的形状
	int8_t rtStatus;	//	0-3		方块图形旋转标志，当前方块的旋转状态
	uint16_t Score;		//	0-65535 当前游戏得分，，游戏难度会随着得分的提高而提高
}Game;	

/*	菜单状态标志	*/
static uint8_t Status;	

/*********************全局变量*/





/*宏参数定义*********************/

/*	游戏开始后方块图形一开始每多少毫秒降落一格	*/
#define TIME_INTERVAL  1000

/*********************宏参数定义*/





/*枚举*********************/

/*	方块图形类型枚举	*/
enum Shape{	
	//	@
	// @@@
	Shape_T = 0,
	//	@@
	//	 @
	//	 @
	Shape_L,
	//	@@
	//	@
	//	@
	Shape_J,
	// @@
	//	@@
	Shape_Z,
	// 	@@
	// @@
	Shape_S,
	// 	@@
	// 	@@
	Shape_O,
	// 	@@@@
	Shape_I
};

/*	菜单状态枚举	*/
enum {
	MAIN,	//	主菜单
	GAME,	//	游戏进行中
	RECORD,	//	记录菜单
	HELP,	//	帮助菜单
	PAUSE	//	暂停菜单
};

/*********************枚举*/





/*工具函数*********************/

/*工具函数仅供内部部分函数使用*/

/**
  * 函    数：次方函数
  * 参    数：X 底数
  * 参    数：Y 指数
  * 返 回 值：等于X的Y次方
  */
static float Tetris_Pow(float X, float Y)
{
	float Result = 1.0f;	//结果默认为1
	while (Y --)			//累乘Y次
	{
		Result *= X;		//每次把X累乘到结果上
	}
	return Result;
}
/*工具函数*********************/





/*功能函数*********************/

/**
  * 函    数：创建一个方块图形
  * 参    数：无
  * 返 回 值：无
  * 说    明：每个新建方块图形均存放到最右端，靠方块图形左移计数左右移动
  */
static void Tetris_Create(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	Game.LShift = 7;		//	初始化方块图形左移计数
	Game.rtStatus = 0;	//	初始化方块图形旋转计数
	Game.bottom = 3;		//	初始化方块图形底部计数
	
	++Game.shape;			//	使方块形状变为下一个
	if(Game.shape > Shape_I)
		Game.shape = Shape_T;
	
	for(i=0;i<4;++i)	//	记录新的下降方块图形
	{
		fall[i] = Data[Game.shape][Game.rtStatus][i];
	}
}

/**
  * 函    数：初始化俄罗斯方块游戏
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Init(void)
{
	int8_t i = 0;	//	循环计数变量
	
	for(i=0;i<24;++i)	//	初始方块刷新区域和显示区域
	{
		ball[i] = 0xE007;
	}
	
	ball[24] = 0xFFFF;	//	初始化容器底部
	ball[25] = 0xFFFF;
	
	Game.shape = -1;		//	初始化方块图形形状标志
	Game.Score = 0;		//	初始化方块图形得分计数
	Game.LShift = 0;		//	初始化方块图形左移计数
	Game.rtStatus = 0;	//	初始化方块图形旋转标志
	Game.bottom = 0;		//	初始化方块图形底部计数
	
	Tetris_Create();		//	创建一个新的方块图形
}

/**
  * 函    数：使方块图形下降
  * 参    数：无
  * 返 回 值：下降成功为 1 ，否则为 0
  */
static uint8_t Tetris_Fall(void)
{
	int8_t i = 0;	//	循环计数变量
	
	for(i=3;i>=0;--i)
	{
		//	通过与操作，判断下落后是否触底，如果触底则返回 0
		if(( (fall[i]<<Game.LShift) & ball[Game.bottom - (3-i) + 1] ) != 0 )	
		{
			return 0;
		}
	}
	++Game.bottom;	//	方块图形底部计数增加
	return 1;
}

/**
  * 函    数：使方块图形朝左移动
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Left(void)
{
	int8_t i = 0;	//	循环计数变量
	
	for(i=3;i>=0;--i)
	{
		//	通过与操作，判断左移后是否装上其它方块
		if(( ( (fall[i]<<Game.LShift) <<1) & ball[Game.bottom - (3-i)] ) != 0 )
		{
			return;
		}
	}
	++Game.LShift;	//	方块图形左移计数增加
}

/**
  * 函    数：使方块图形朝右移动
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Right(void)
{
	int8_t i = 0;	//	循环计数变量
	
	for(i=3;i>=0;--i)
	{
		//	通过与操作，判断右移后是否装上其它方块
		if(( ( (fall[i]<<Game.LShift) >>1) & ball[Game.bottom - (3-i)] ) != 0 )
		{
			return;
		}
	}
	--Game.LShift;	//	方块图形右移计数增加
}

/**
  * 函    数：将不再下降的方块图形堆叠到容器中
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Stack(void)
{
	int8_t i = 0;	//	循环计数变量
	
	for(i=3;i>=0;--i)
	{
		//	通过或操作将下落的方块存到容器中
		ball[Game.bottom - (3-i)] |= (fall[i]<<Game.LShift) ;
	}
}

/**
  * 函    数：判断并且消除容器中填充满了方块的行
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Full(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	uint8_t Count = 0;	//	用来记录一次连续消除了多少行
	
	for(i=23;i>=4;--i)
	{
		if(ball[i] == 0xFFFF)	//	判断该行是否填充满了方块
		{
			uint8_t j = 0;
			for(j=i;j>=5;--j)	//	用上面几行的方块数据覆盖填充满方块的行
			{
				ball[j] = ball[j-1];
			}
			
			//	使循环计数变量加一，为了再次判断该行是否充满了方块
			//	因为上面几行可能也填充满了方块，它们的数据向下覆盖后
			//	该行可能被覆盖上填充满了方块的数据，如果不再次判断该行
			//	可能会造成该行填充满了方块，但是没有消除的现象
			++i;	
			
			Game.Score += 100 * Tetris_Pow(1.5,Count);//	连续消除的第n行会获得 100 * 1.5^(n-1) 分
			
			++Count;	//	每连续消除一行就加一
		}
	}
}

/**
  * 函    数：使方块图形朝左旋转
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_RtLeft(void)
{
	int8_t i = 0;	//	循环计数变量
	
	int8_t tmp = Game.rtStatus;	//	tmp变量用于表示当前方块图形的上一个方块旋转标志
	--tmp;
	if(tmp < 0)
		tmp = 3;
	
	for(i=3;i>=0;--i)
	{
		uint16_t Rt = Data[Game.shape][tmp][i];
		if(( ( (Rt<<Game.LShift) ) & ball[Game.bottom - (3-i)] ) != 0 )	//	判断左移是否会撞上方块
		{
			return;
		}
	}
	
	Game.rtStatus = tmp;	//	使方块图形旋转标志变为上一个
	for(i=0;i<4;++i)
	{
		fall[i] = Data[Game.shape][Game.rtStatus][i];	//	将新的方块图形保存为下落方块图形
	}
}

/**
  * 函    数：使方块图形朝右旋转
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_RtRight(void)
{
	int8_t i = 0;	//	循环计数变量
	
	int8_t tmp = Game.rtStatus;	//	tmp变量用于表示当前方块图形的下一个方块图形旋转标志
	++tmp;
	if(tmp > 3)
		tmp = 0;
	
	for(i=3;i>=0;--i)
	{
		uint16_t Rt = Data[Game.shape][tmp][i];
		if(( ( (Rt<<Game.LShift) ) & ball[Game.bottom - (3-i)] ) != 0 )
		{
			return;
		}
	}
	Game.rtStatus = tmp;	//	使方块图形旋转标志变为下一个
	for(i=0;i<4;++i)
	{
		fall[i] = Data[Game.shape][Game.rtStatus][i];	//	将新的方块图形保存为下落方块图形
	}
}

/**
  * 函    数：计算方块图形底部距离容器底部还有几格距离
  * 参    数：无
  * 返 回 值：计算方块图形底部到容器底部的距离
  */
static uint8_t Tetris_DistanceToBottom(void)
{
	uint8_t ret = 0;	//	返回值
	
	while(1)
	{
		int8_t i = 0;	//	循环计数变量
		
		for(i=3;i>=0;--i)
		{
			//	通过与操作判断下降 ret+1 行后是否触底
			if(( (fall[i]<<Game.LShift) & ball[Game.bottom + ret - (3-i) + 1] ) != 0 )
			{
				return ret;	//	返回距离
			}
		}
		++ret;	//	判断没触底则使返回值加一
	}
}

/**
  * 函    数：使方块图形快速下落，"击打"容器底部
  * 参    数：无
  * 返 回 值：无
  * 说    明：通过改变图形底部计数的方式实现
  */
static void Tetris_Hitbottom(void)
{
	//	将图形底部计数加上方块底部到容器底部的距离，使得图形底部计数加一后必定会触底
	Game.bottom += Tetris_DistanceToBottom();
}

/**
  * 函    数：显示容器内的方块、正在下降的方块图形、下一个方块图形、记分板和方块最终落下的位置
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void Tetris_Display(void)
{
	int8_t i = 0;	//	循环计数变量
	int8_t Distance = 0;	//	存放方块底部到容器底部的距离
	
	OLED_Clear();	//	清屏
	
	/*显示边框*/
	OLED_DrawLine(0,6,99,6);
	OLED_DrawLine(0,6,0,57);
	OLED_DrawLine(0,57,99,57);
	OLED_DrawLine(100,0,100,63);
	OLED_ShowImage(111,0,16,48,Tetris_ScoreImage);
	/*显示边框*/
	
	/*显示得分*/
	for(i=0;i<5;++i)
	{
		uint8_t num = Game.Score / (uint32_t)( Tetris_Pow(10,(5-i-1)) ) % 10;
		OLED_ShowImage(105,i*6,5,3,Tetris_NumImage[num]);
	}
	/*显示得分*/
	
	/*显示下一个方块图形*/
	switch( (Game.shape+1) % (Shape_I+1) )
	{
		case Shape_T:
			OLED_ShowImage(117,49,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			OLED_ShowImage(117,59,5,5,Cube);
			OLED_ShowImage(122,54,5,5,Cube);
			break;
		case Shape_L:
			OLED_ShowImage(122,54,5,5,Cube);
			OLED_ShowImage(122,59,5,5,Cube);
			OLED_ShowImage(117,59,5,5,Cube);
			OLED_ShowImage(112,59,5,5,Cube);
			break;
		case Shape_J:
			OLED_ShowImage(122,54,5,5,Cube);
			OLED_ShowImage(122,59,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			OLED_ShowImage(112,54,5,5,Cube);
			break;
		case Shape_Z:
			OLED_ShowImage(122,49,5,5,Cube);
			OLED_ShowImage(122,54,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			OLED_ShowImage(117,59,5,5,Cube);
			break;
		case Shape_S:
			OLED_ShowImage(122,54,5,5,Cube);
			OLED_ShowImage(122,59,5,5,Cube);
			OLED_ShowImage(117,49,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			break;
		case Shape_O:
			OLED_ShowImage(122,54,5,5,Cube);
			OLED_ShowImage(122,59,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			OLED_ShowImage(117,59,5,5,Cube);
			break;
		case Shape_I:
			OLED_ShowImage(117,44,5,5,Cube);
			OLED_ShowImage(117,49,5,5,Cube);
			OLED_ShowImage(117,54,5,5,Cube);
			OLED_ShowImage(117,59,5,5,Cube);
			break;
	}
	/*显示下一个方块图形*/
	
	/*显示下降的方块图形与容器中堆积的方块*/
	for(i=23;i>=4;--i)
	{
		uint8_t j = 0;
		
		if(i<=Game.bottom && i>=(Game.bottom-3))
		{
			for(j=0;j<=10;++j)
			{
				if( (( ball[i] | (fall[3 - (Game.bottom-i)]<<Game.LShift) ) >> 3) & (0x0001<<(9-j)) )
				{
					OLED_ShowImage(0+5*(23-i),7+5*j,5,5,Cube);
				}
			}
		}
		else
		{
			for(j=0;j<=10;++j)
			{
				if( ( ball[i] >> 3) & (0x0001<<(9-j)) )
				{
					OLED_ShowImage(0+5*(23-i),7+5*j,5,5,Cube);
				}
			}
		}
		
	}
	/*显示下降的方块图形与容器中堆积的方块*/
	
	//	计算方块图形底部距离容器底部还有几格距离
	Distance = Tetris_DistanceToBottom();
	
	/*显示方块最终落下的位置*/
	for(i=3;i>=0;--i)
	{
		uint8_t j = 0;
		for(j=0;j<=10;++j)
		{
			if( ( (fall[i]<<Game.LShift) >> 3) & (0x0001<<(9-j)) )
			{
				if((23-Game.bottom-Distance+(3-i))>=0)
					OLED_DrawRectangle(0+(23-Game.bottom-Distance+(3-i))*5,7+5*j,5,5,OLED_UNFILLED);
			}
		}
	}
	/*显示方块最终落下的位置*/
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：判断游戏是否结束
  * 参    数：无
  * 返 回 值：无
  */
static uint8_t Tetris_GameOver(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	//	通过判断容器的方块图形刷新区域是否被堆积了方块来判断游戏是否结束
	for(i=0;i<4;++i)
	{
		if(ball[i] != 0xE007)
			return 1;
	}
	return 0;
}

/**
  * 函    数：保存游戏最高的四个游戏得分记录到FLASH
  * 参    数：无
  * 返 回 值：FLASH最后一页的第三四五六个半字由高到低存放得分最高的4个记录
  */
static void Tetris_Save(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	for(i=0;i<4;++i)	//	FLASH最后一页的第三四五六个半字由高到低存放得分最高的4个记录
			{
				if(Game.Score >= Store_Data[2+i])
				{
					uint8_t j = 0;
					for(j=3;j>i;--j)
					{
						Store_Data[2+j] = Store_Data[2+j-1];
					}
					Store_Data[2+i] = Game.Score;
					Store_Save();	//	保存到FLASH中
					break;
				}
			}
}

//	声明游戏主菜单视图显示
static void GameMenu_Tetris_View_MAIN(void);
/**
  * 函    数：俄罗斯方块游戏循环
  * 参    数：无
  * 返 回 值：无
  * 说    明：游戏时应通过定时器每1s执行一次
  *           该函数决定了方块图形如何下落，
  *			  判断是否触底，是否有容器行堆满了，游戏是否应该结束了
  *           是否应该增加得分，增加多少得分，以及随得分调整游戏难度等
  */
static void Tetris_Loop(void)
{
	if( !Tetris_Fall() )	//	使方块图形下降，并下降判断方块图形是否成功的下降
	{
		Tetris_Stack();	//	方块图形没下降后堆积到容器中
		if(Tetris_GameOver())	//	判断游戏是否结束了
		{
			
			
			Timer_Stop(Tetris_Loop);	//	定时器不再为俄罗斯方块游戏循环函数定时
			
			Tetris_Save();	//	保存得分记录
			
			Status = MAIN;	//	转变状态为 MAIN
			GameMenu_Tetris_View_MAIN();	//	显示游戏主菜单
			return;
		}
		else
		{
			Tetris_Full();	//	判断是否由填充满了方块的行。如果有，消除该行，并且增加得分
			
			//	随着得分的提高，缩短两次下落之间的时间间隔以提高游戏难度，
			//	10000分时达到最高难度,间隔由一开始的1000ms缩短为200ms
			if(Game.Score <= 10000)
			{
				Timer_Stop(Tetris_Loop);	//	定时器不再为俄罗斯方块游戏循环函数定时，防止打扰后续的目标和当前定时值修改
				Timer_SetCount(Tetris_Loop,0);	//	重置俄罗斯方块游戏循环函数当前定时值
				Timer_SetTarget(Tetris_Loop,TIME_INTERVAL-0.8*TIME_INTERVAL*(Game.Score/10000.0));//重置俄罗斯方块游戏循环函数目标定时值
				Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
			}
			
			Tetris_Create();	//	创建新的方块图形
		}
	}
	Tetris_Display();	//	显示容器内的方块、正在下降的方块图形、下一个方块图形、记分板和方块最终落下的位置
}

/**
  * 函    数：游戏主菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Tetris_View_MAIN(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);
	OLED_ShowImage(70,0,20,48,Button);
	OLED_ShowImage(100,0,20,48,Button);

	OLED_ShowChinese( 3, 48,  "开始");
	OLED_ShowChinese( 34, 48, "退出");
	OLED_ShowChinese( 65, 48, "记录");
	OLED_ShowChinese( 95, 48, "帮助");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏记录菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Tetris_View_RECORD(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	for(i=0;i<4;++i)
	{
		OLED_Printf(0,0+i*16,OLED_8X16,"NO.%d : %5d",i+1,Store_Data[2+i]);	//	显示得分最高的四个记录
	}
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏暂停菜单视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Tetris_View_PAUSE(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);

	OLED_ShowChinese( 5, 48,  "继续");
	OLED_ShowChinese( 35, 48, "退出");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏帮助菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Tetris_View_HELP(void)
{
	OLED_Clear();	//	清屏
	
	
	OLED_ShowChinese(0, 0,"键一左移，二右移");
	OLED_ShowChinese(0,16,"键三击底，四暂停");
	OLED_ShowChinese(0,32,"旋钮逆时旋转左转");
	OLED_ShowChinese(0,48,"旋钮顺时旋转右转");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：俄罗斯方块游戏菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于俄罗斯方块游戏菜单的功能函数
  */
static void GameMenu_Tetris_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	Status = MAIN;	//	初始化状态为 MAIN
	
	GameMenu_Tetris_View_MAIN();	//	游戏主菜单视图显示
	Timer_AddFunction( Tetris_Loop, TIME_INTERVAL);	//	将Tetris_Loop函数添加到定时器定时列表，配置为每1s到时一次
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Tetris_Init();	//	初始化俄罗斯方块游戏
					Timer_SetTarget(Tetris_Loop,TIME_INTERVAL);	//	初始化定时间隔
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，防止打扰左移操作
					Tetris_Left();	//	左移方块图形
					Tetris_Display();	//	显示所有方块等
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
				
				else if(Status == PAUSE)	/*	暂停	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Timer_DeleteFunction(Tetris_Loop);	//	将Tetris_Loop函数从定时器定时列表删除
					return;
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，防止打扰右移操作
					Tetris_Right();	//	右移方块图形
					Tetris_Display();	//	显示所有方块等
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
				else if(Status == RECORD)/*	记录	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Tetris_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == HELP)/*	帮助	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Tetris_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == PAUSE)/*	暂停	*/
				{
					Tetris_Save();	//	保存得分记录
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Tetris_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = RECORD;	//	转换状态为 RECORD
					GameMenu_Tetris_View_RECORD();	//	游戏记录菜单视图显示
				}
				if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，防止打扰击底操作
					Tetris_Hitbottom();	//	方块图形击底
					Tetris_Display();	//	显示所有方块等
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = HELP;	//	转换状态为 HELP
					GameMenu_Tetris_View_HELP();	//	游戏帮助菜单视图显示
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Status = PAUSE;	//	转换状态为 PAUSE
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，暂停
					GameMenu_Tetris_View_PAUSE();	//	游戏暂停菜单视图显示
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，防止打扰右转操作
					Tetris_RtRight();	//	右转方块图形
					Tetris_Display();	//	显示所有方块等
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Tetris_Loop);	//	停止为俄罗斯方块游戏循环函数定时，防止打扰左转操作
					Tetris_RtLeft();	//	左转方块图形
					Tetris_Display();	//	显示所有方块等
					Timer_Start(Tetris_Loop);	//	开始为俄罗斯方块游戏循环函数定时
				}
			}
			else
			{
				MSG_DefaultProc(&msg);	//	消息默认处理
			}
		}
		
	}	
}

/**
  * 函    数：初始化俄罗斯方块游戏菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了俄罗斯方块游戏菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void GameMenu_Tetris_Init(void)
{
	/*初始化俄罗斯方块游戏菜单*/
	Menu_Init(&GameMenu_Tetris);
	GameMenu_Tetris.Funsion = GameMenu_Tetris_Funsion;
	GameMenu_Tetris.Icon = GameMenu_Tetris_Icon;
	GameMenu_Tetris.Name = GameMenu_Tetris_Name;
	/*初始化俄罗斯方块游戏菜单*/
	
	Store_Init();	//	初始化Store模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
}

/*********************功能函数*/

