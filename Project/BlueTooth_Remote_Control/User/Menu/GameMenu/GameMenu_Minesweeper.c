/***************************************************************************************
  * 本菜单为游戏菜单的二级子菜单，用执行扫雷游戏，并且保存游戏三个难度用时最短的记录到FLASH
  * FLASH最后一页的第十一、十二、十三个半字由高到低存放耗时最短的4个记录
  ***************************************************************************************
  */

#include "GameMenu_Minesweeper.h"
#include "stm32f10x.h"                  // Device header

#include "Key.h"
#include "Menu.h"
#include "OLED.h"
#include "Delay.h"
#include "MyRTC.h"
#include "Store.h"
#include "Timer.h"
#include <stdlib.h>
#include "Encoder.h"

#include "MSG.h"

/*图像数据*********************/

/*	扫雷游戏菜单图标	——	一个反光的尖刺地雷		64W - 64H	*/
const uint8_t GameMenu_Minesweeper_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x1C,0x38,0x70,0xE0,0xE0,
	0xF0,0xF0,0xF8,0xF8,0xFC,0xFC,0xFC,0xFF,0xFF,0xFC,0xFC,0xFC,0xF8,0xF8,0xF0,0xF0,
	0xE0,0xE0,0x70,0x38,0x1C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0xE0,0xF8,0xFE,0xFF,0xFF,0xFF,
	0xE1,0xE1,0xE1,0xE1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFE,0xF8,0xE0,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x07,0x1F,0x7F,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x7F,0x1F,0x07,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x1C,0x0E,0x07,0x03,
	0x07,0x0F,0x0F,0x1F,0x1F,0x3F,0x3F,0xFF,0xFF,0x3F,0x3F,0x1F,0x1F,0x0F,0x0F,0x07,
	0x03,0x07,0x0E,0x1C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*	显示周边地雷个数的小数字	3W - 5H	*/
const uint8_t Minesweeper_NumImage[9][3] = {
	0x1F,0x11,0x1F,	//	0
	0x00,0x1F,0x00,	//	1
	0x1D,0x15,0x17,	//	2
	0x15,0x15,0x1F,	//	3
	0x07,0x04,0x1F,	//	4
	0x17,0x15,0x1D,	//	5
	0x1F,0x15,0x1D,	//	6
	0x01,0x1D,0x03,	//	7
	0x1F,0x15,0x1F,	//	8
};

/*	地雷图标	5W - 5H	*/
const uint8_t Minesweeper_MineImage[] = {
0x15,0x0E,0x1F,0x0E,0x15,
};
/*********************图像数据*/





/*宏参数定义*********************/

/*	游戏有几行	-	减一则为纵坐标最大值*/
#define ROW 8
/*	游戏有几列	-	减一则为横坐标最大值*/
#define COL 16

/*********************宏参数定义*/





/*全局变量*********************/

/*	扫雷游戏菜单结构体	*/
Menu GameMenu_Minesweeper;	

/*	扫雷游戏菜单名字	*/
const char* GameMenu_Minesweeper_Name = "扫雷";

/*	扫雷游戏结构体	*/
static struct {
	uint8_t Mine[COL][ROW];	//	地雷地图，存放地雷的位置
	uint8_t Map[COL][ROW];	//	玩家地图，存放玩家看到的地图信息，如每个格子周边8个格子内的地雷数量
	uint8_t Level;			//	游戏难度
	uint16_t StartTime;		//	开始时间，记录每局游戏开始时的时间戳
	uint8_t X;				//	当前指向格子的横坐标
	uint8_t Y;				//	当前指向格子的纵坐标
}Game;	

/*	菜单状态标志	*/
static uint8_t Status;	

/*********************全局变量*/





/*枚举*********************/

/*	菜单状态枚举	*/
enum {
	MAIN,	//	主菜单
	LEVEL,	//	游戏难度选择
	GAME,	//	游戏进行中
	RECORD,	//	记录菜单
	HELP,	//	帮助菜单
	PAUSE	//	暂停菜单
};

/*	游戏难度枚举	*/
enum {
	HARD,	//	困难
	MEDIUM,	//	中等
	EASY	//	简单
};

/*	游戏地图格子标识	*/
enum {
	//	游戏地图标识不可占用 1-8。1-8 用于表示某个格子周边8个格子内地雷的数量，即地雷数量标识
	UNKNOW=0,	//	为未知
	SPACE=9,	//	已知为空白，同时也表示周边8个格子内没有地雷
	MINE,		//	已知为地雷
};

/*********************枚举*/





/*功能函数*********************/

/**
  * 函    数：在一个随机且合理的地方生成地雷
  * 参    数：无
  * 返 回 值：无
  */
static void Minesweeper_CreateMine(void)
{
	uint8_t X = 0;	//	临时横坐标变量
	uint8_t Y = 0;	//	临时纵坐标变量
	
	do{
		X = rand() % COL;	//	随机生成一个横坐标
		Y = rand() % ROW;	//	随机生成一个纵坐标
		//	倘若随机生成的坐标已经有雷了，则再进行生成，直到得到一个合理的生成位置
	}while(	Game.Mine[X][Y] == MINE );
	Game.Mine[X][Y] = MINE;	//	在地雷地图中对应的格子记录地雷标识
}

/**
  * 函    数：初始化扫雷游戏
  * 参    数：Level	游戏难度
  * 返 回 值：无
  */
static void Minesweeper_Init(uint8_t Level)
{
	uint16_t i = 0;	//	循环计数变量
	
	Game.Level = Level;	//	记录游戏
	
	for(i=0;i<COL;++i)	//	初始化地雷地图和玩家地图
	{
		uint16_t j = 0;
		for(j=0;j<ROW;++j)
		{
			Game.Map[i][j] = UNKNOW;	//	玩家地图的每个格子初始化为未知
			Game.Mine[i][j] = SPACE;	//	地雷地图的每个格子初始化为恐怖
		}
	}
	
	if(Level == HARD)			//	困难难度下
	{
		for(i=0;i<(uint16_t)(ROW*COL*0.35);++i)	//	35%的格子会被埋入地雷
		{
			Minesweeper_CreateMine();	//	创建地雷
		}
	}
	else if(Level == MEDIUM)	//	中等难度下
	{
		for(i=0;i<(uint16_t)(ROW*COL*0.20);++i)	//	20%的格子会被埋入地雷
		{
			Minesweeper_CreateMine();	//	创建地雷
		}
	}
	else if(Level == EASY)		//	简单难度下
	{
		for(i=0;i<(uint16_t)(ROW*COL*0.10);++i)	//	10%的格子会被埋入地雷
		{
			Minesweeper_CreateMine();	//	创建地雷
		}
	}
	
	Game.StartTime = RTC_GetCounter();	//	记录这局游戏开始时的时间戳，方便通关后得出通关时长
	Game.X = 0;		//	当前指向格子的横坐标，使其位于左上角
	Game.Y = 0;		//	当前指向格子的纵坐标，使其位于左上角
}

/**
  * 函    数：保存游戏三个难度用时最短的记录到FLASH
  * 参    数：无
  * 返 回 值：无
  * 说    明：FLASH最后一页的第十一、十二、十三个半字由高到低存放耗时最短的4个记录
  */
static void Minesweeper_Save(void)
{
	uint16_t EndTime = RTC_GetCounter();	//	临时变量，用于临时记录游戏通关时的时间戳
	
	//	游戏通关时的时间戳与游戏开始时的时间戳相减得出通过时长
	uint16_t Socre = EndTime - Game.StartTime;	
	
	if(Game.Level == HARD)			//	困难难度下
	{
		if(Socre < Store_Data[10] || Store_Data[10]==0)	//	如果时长更短或者从未记录过数据
		{
			Store_Data[10] = Socre;	//	将困难模式下的数据记录到FLASH最后一页的第十一个半字
		}
	}
	else if(Game.Level == MEDIUM)			//	中等难度下
	{
		if(Socre < Store_Data[11] || Store_Data[11]==0)	//	如果时长更短或者从未记录过数据
		{
			Store_Data[11] = Socre;	//	将中等模式下的数据记录到FLASH最后一页的第十一个半字
		}
	}
	else if(Game.Level == EASY)			//	简单难度下
	{
		if(Socre < Store_Data[12] || Store_Data[12]==0)	//	如果时长更短或者从未记录过数据
		{
			Store_Data[12] = Socre;	//	将简单模式下的数据记录到FLASH最后一页的第十一个半字
		}
	}
	
	Store_Save();	//	保存数据到FLASH
}

/**
  * 函    数：指向 当前指向格子 左边的格子
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果左边没有格子，则指向最右边的格子
  */
static void Minesweeper_Left(void)
{
	int8_t tmp = Game.X;	//	临时变量，用于临时记录当前指向格子的横坐标
	
	if(tmp - 1 < 0)	//	如果左边没有格子
	{
		Game.X = COL-1;	//	指向最右边的格子
	}
	else	//	如果左边仍然有格子
	{
		--(Game.X);		//	指向左边的格子
	}
}

/**
  * 函    数：指向 当前指向格子 右边的格子
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果右边没有格子，则指向最左边的格子
  */
static void Minesweeper_Right(void)
{
	int8_t tmp = Game.X;	//	临时变量，用于临时记录当前指向格子的横坐标
	
	if(tmp + 1 >= COL)	//	如果右边没有格子
	{
		Game.X = 0;	//	指向最左边的格子
	}
	else	//	如果右边仍然有格子
	{
		++(Game.X);	//	指向右边的格子
	}
}

/**
  * 函    数：指向 当前指向格子 上方的格子
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果上方没有格子，则指向最下方的格子
  */
static void Minesweeper_Up(void)
{
	int8_t tmp = Game.Y;	//	临时变量，用于临时记录当前指向格子的纵坐标
	
	if(tmp - 1 < 0)	//	如果上方没有格子
	{
		Game.Y = ROW-1;	//	指向最下方的格子
	}
	else	//	如果上方仍然有格子
	{
		--(Game.Y);	//	指向上方的格子
	}
}

/**
  * 函    数：指向 当前指向格子 下方的格子
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果下方没有格子，则指向最上方的格子
  */
static void Minesweeper_Down(void)
{
	int8_t tmp = Game.Y;	//	临时变量，用于临时记录当前指向格子的纵坐标
	
	if(tmp + 1 >= ROW)	//	如果下方没有格子
	{
		Game.Y = 0;	//	指向最上方的格子
	}
	else	//	如果下方仍然有格子
	{
		++(Game.Y);	//	指向下方的格子
	}
}

/**
  * 函    数：显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
  * 参    数：无
  * 返 回 值：无
  */
static void Minesweeper_Display(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	/*显示边框*/
	for(i=0;i<COL;++i)
	{
		OLED_DrawLine( 0+8*i, 0, 0+8*i, 63);
	}
	OLED_DrawLine(127,0, 127, 63);
	for(i=0;i<ROW;++i)
	{
		OLED_DrawLine( 0, 0+8*i, 127, 0+8*i);
	}
	OLED_DrawLine( 0, 63, 127, 63);
	/*显示边框*/

	/*显示玩家地图信息*/
	for(i=0;i<COL;++i)
	{
		uint8_t j = 0;	//	循环计数变量
		
		for(j=0;j<ROW;++j)
		{
			if(Game.Map[i][j] == UNKNOW)	//	如果该格子标识为未知
			{
				OLED_DrawRectangle( 2+8*i, 2+8*j,5,5,OLED_FILLED);	//	显示实心方块
			}
			else if(Game.Map[i][j] == SPACE)	//	如果该格子标识为已知为空白(周边8个格子没有地雷)
			{
				;	//	什么也不显示
			}
			else if(Game.Map[i][j] == MINE)	//	如果该格子标识为已知为地雷
			{
				OLED_ShowImage( 2+8*i, 2+8*j,5,5,Minesweeper_MineImage);	//	显示地雷图像
			}
			else	//	如果该格标识为周围8个格子中地雷的数量
			{
				//	显示对应的数字图像
				OLED_ShowImage( 3+8*i, 2+8*j,3,5,Minesweeper_NumImage[ Game.Map[i][j] ]);
			}
		}
	}
	/*显示玩家地图信息*/
	
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：计算某个指定的格子周边8个格子内有几个地雷
  * 参    数：X		指定格子的横坐标
  * 参    数：Y		指定格子的纵坐标
  * 返 回 值：指定的格子周边8个格子内有几个地雷
  */
static uint8_t Minesweeper_NearbyMineNum(uint8_t X,uint8_t Y)
{
	int8_t i = 0;	//	循环计数变量
	
	uint8_t Count = 0;	//	临时变量，用于记录指定的格子周边8个格子内有几个地雷
	
	for(i=X-1;i<=X+1;++i)
	{
		int8_t j = 0;	//	循环计数变量
		
		if(i<0 || i>=COL)	//	如果超出了地图的范围就跳过
			continue;
		
		for(j=Y-1;j<=Y+1;++j)
		{
			if(j<0 || j>=ROW)	//	如果超出了地图的范围就跳过
				continue;
			
			if(Game.Mine[ i ][ j ] == MINE)	//	判断周边的该格是否为地雷
				++Count;	//	是地雷则使计数加一
		}
	}
	return Count;	//	返回计数
}

/**
* 函    数：从某个指定的格子开始清空地图中一片未知且探查后应该为空白(即周边8个格子没有地雷)的区域，
  *         并且在旁边有地雷的格子显示周边8格内地雷的数量
  * 参    数：X		指定格子的横坐标
  * 参    数：Y		指定格子的纵坐标
  * 返 回 值：无
  * 说    明：该函数为递归函数
  */
static void Minesweeper_ClearArea(int8_t X,int8_t Y)
{
	uint8_t Count = 0;	//	临时变量，用于记录指定的格子周边8个格子内有几个地雷
	
	if(X < 0 || X >= COL)	//	如果超出地图范围则不再递归
		return;
	if(Y < 0 || Y >= ROW)	//	如果超出地图范围则不再递归
		return;
	if(Game.Map[ X ][ Y ] != UNKNOW)	//	如果指定的格子不为未知的格子则不再递归
		return;
	
	Count = Minesweeper_NearbyMineNum(X,Y);	//	计算指定的格子周边8个格子内有几个地雷
	
	if(Count == 0)	//	如果指定的格子周边没有地雷(即指定的格子为空白的格子)，则递归
	{
		Game.Map[ X ][ Y ] = SPACE;	//	在玩家地图中对应的格子记录空白标识
		Minesweeper_ClearArea( (int8_t)X-1, (int8_t)Y );	//	从指定的格子左边的格子开始清除
		Minesweeper_ClearArea( (int8_t)X+1, (int8_t)Y );	//	从指定的格子右边的格子开始清除
		Minesweeper_ClearArea( (int8_t)X, (int8_t)Y-1 );	//	从指定的格子上方的格子开始清除
		Minesweeper_ClearArea( (int8_t)X, (int8_t)Y+1 );	//	从指定的格子下方的格子开始清除
	}
	else	//	如果有地雷，则不递归
	{
		Game.Map[ X ][ Y ] =  Count;	//	在玩家地图中对应的格子记录地雷数量标识
	}
}

/**
  * 函    数：在玩家地图中标识所有地雷的位置
  * 参    数：无
  * 返 回 值：无
  * 说    明：应该在查明所有地雷位置，踩到地雷或者作弊的时候调用
  */
static void Minesweeper_ShowAllMine(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	for(i=0;i<COL;++i)
	{
		uint8_t j = 0;	//	循环计数变量
		
		for(j=0;j<ROW;++j)
		{
			if(Game.Mine[i][j] == MINE)	//	如果地雷地图当前位置有地雷
				Game.Map[i][j] = MINE;	//	在玩家地图中对应的格子记录地雷标识
		}
	}
}

/**
  * 函    数：扫雷游戏循环
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：游戏时应通过定时器每1s执行一次
  *           该函数会使当前指定的格子闪烁
  */
void Minesweeper_Loop(void)
{
	OLED_ReverseArea( 2+8*Game.X, 2+8*Game.Y,5,5);	//	使当前当前指定的格子区域反色
	OLED_UpdateArea( 2+8*Game.X, 2+8*Game.Y,5,5);	//	刷新前指定的格子区域
}

/**
  * 函    数：判断这局扫雷游戏是否以及获胜(玩家地图探明了所有非地雷格子)
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：如果探明了玩家地图所有非地雷格子则获胜了并返回1，否则返回0
  */
static uint8_t Minesweeper_isWin(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	for(i=0;i<COL;++i)
	{
		uint8_t j = 0;
		for(j=0;j<ROW;++j)
		{
			//	如果玩家地图有未知的格子，而这个格子在地雷地图里不是地雷，说明没有探完
			if(Game.Mine[i][j] != MINE && Game.Map[i][j] == UNKNOW)	
				return 0;	//	没获胜返回0
		}
	}
	return 1;	//	获胜返回1
}

//	声明游戏主菜单视图显示
static void GameMenu_Minesweeper_View_MAIN(void);
/**
  * 函    数：在当前指向格子进行扫雷
  * 参    数：无
  * 返 回 值：无
  * 说    明：
  */
static void Minesweeper_Sweep(void)
{
	if(Game.Mine[ Game.X ][ Game.Y ] == MINE)	//	如果踩到雷了
	{
		Minesweeper_ShowAllMine();	//	在玩家地雷标明所有地图的位置
		Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
		
		Delay_ms(2000);	//	延时2s
		Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
		
		Status = MAIN;	//	初始化状态为 MAIN

		GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
	}
	else	//	如果没踩到雷
	{
		//	从某个指定的格子开始清空地图中一片未知且探查后应该为空白(即周边8个格子没有地雷)的区域
		Minesweeper_ClearArea(Game.X,Game.Y);
		
		if(Minesweeper_isWin())	//	如果获胜了(玩家地图探明了所有非地雷格子)
		{
			Minesweeper_Save();	//	保存游戏耗时成绩
			Minesweeper_ShowAllMine();		//	在玩家地雷标明所有地图的位置
			Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
			
			Delay_ms(2000);	//	延时2s
			Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
			
			Status = MAIN;	//	初始化状态为 MAIN

			GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
		}
	}
}



/**
  * 函    数：游戏主菜单视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Minesweeper_View_MAIN(void)
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
  * 函    数：游戏难度选择菜单视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Minesweeper_View_LEVEL(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);
	OLED_ShowImage(70,0,20,48,Button);
	OLED_ShowImage(100,0,20,48,Button);

	OLED_ShowChinese( 3, 48,  "简单");
	OLED_ShowChinese( 34, 48, "中等");
	OLED_ShowChinese( 65, 48, "困难");
	OLED_ShowChinese( 95, 48, "退出");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏暂停菜单视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Minesweeper_View_PAUSE(void)
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
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Minesweeper_View_HELP(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowChinese(0, 0,"键一上移键二下移");
	OLED_ShowChinese(0,16,"键三扫雷键四暂停");
	OLED_ShowChinese(0,32,"旋钮逆时旋转左移");
	OLED_ShowChinese(0,48,"旋钮顺时旋转右移");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏记录菜单视图显示
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Minesweeper_View_RECORD(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	for(i=0;i<4;++i)
	{
		//	分别显示三个难度用时最短的记录
		OLED_ShowChinese(0, 0,"各难度最短用时");
		OLED_ShowChinese(0,16,"困难：");
		OLED_ShowChinese(0,32,"中等：");
		OLED_ShowChinese(0,48,"简单：");
		OLED_Printf(48,16,OLED_8X16,"%5d s",Store_Data[10]);
		OLED_Printf(48,32,OLED_8X16,"%5d s",Store_Data[11]);
		OLED_Printf(48,48,OLED_8X16,"%5d s",Store_Data[12]);
	}
	
	OLED_Update();	//	刷新
}



/**
  * 函    数：扫雷游戏菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于扫雷游戏菜单的功能函数
  */
void GameMenu_Minesweeper_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	Status = MAIN;	//	初始化状态为 MAIN
	
	GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
	
	Timer_AddFunction( Minesweeper_Loop, 500);	//	将Minesweeper_Loop函数添加到定时器定时列表，配置为每0.5s到时一次
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = LEVEL;	//	转换状态为 LEVEL
					GameMenu_Minesweeper_View_LEVEL();
				}
				else if(Status == LEVEL)	/*	游戏难度选择	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Minesweeper_Init(EASY);	//	初始化扫雷游戏
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					Minesweeper_Up();	//	指向 当前指向格子 上方的格子
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
				
				else if(Status == PAUSE)	/*	暂停	*/
				{
					Status = GAME;	//	转换状态为 GAME
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Timer_DeleteFunction(Minesweeper_Loop);	//	将Minesweeper_Loop函数从定时器定时列表删除
					return;
				}
				else if(Status == LEVEL)	/*	游戏难度选择	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Minesweeper_Init(MEDIUM);	//	初始化扫雷游戏
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					Minesweeper_Down();	//	指向 当前指向格子 下方的格子
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
				else if(Status == RECORD)/*	记录	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == HELP)/*	帮助	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == PAUSE)/*	暂停	*/
				{
					Minesweeper_Save();	//	保存得分记录
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = RECORD;	//	转换状态为 RECORD
					GameMenu_Minesweeper_View_RECORD();	//	游戏记录菜单视图显示
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					Minesweeper_Sweep();	//	在当前指向格子进行扫雷
					if(Status == GAME)	//	如果游戏还没结束
					{
						Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
						Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
					}
				}
				else if(Status == LEVEL)	/*	游戏难度选择	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Minesweeper_Init(HARD);	//	初始化扫雷游戏
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = HELP;	//	转换状态为 HELP
					GameMenu_Minesweeper_View_HELP();	//	游戏帮助菜单视图显示
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Status = PAUSE;	//	转换状态为 PAUSE
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					GameMenu_Minesweeper_View_PAUSE();	//	游戏暂停菜单视图显示
				}
				else if(Status == LEVEL)	/*	游戏难度选择	*/
				{
					Status = MAIN;	//	转换状态为 GAME
					GameMenu_Minesweeper_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					Minesweeper_Right();	//	指向 当前指向格子 右边的格子
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Minesweeper_Loop);	//	定时器不再为扫雷游戏循环函数定时
					Minesweeper_Left();		//	指向 当前指向格子 左边的格子
					Minesweeper_Display();	//	显示边框和玩家地图信息，如每个格子周边8个格子内的地雷数量等
					Timer_Start(Minesweeper_Loop);	//	开始为扫雷游戏循环函数定时
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
  * 函    数：初始化扫雷游戏菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了扫雷游戏菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void GameMenu_Minesweeper_Init(void)
{
	/*初始化扫雷游戏菜单*/
	Menu_Init(&GameMenu_Minesweeper);
	GameMenu_Minesweeper.Funsion = GameMenu_Minesweeper_Funsion;
	GameMenu_Minesweeper.Icon = GameMenu_Minesweeper_Icon;
	GameMenu_Minesweeper.Name = GameMenu_Minesweeper_Name;
	/*初始化扫雷游戏菜单*/
	
	Store_Init();	//	初始化Store模块
	MyRTC_Init();	//	初始化MyRTC模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
	
	srand((unsigned int)RTC_GetCounter());	//	初始化随机数生成
}

/*********************功能函数*/

