/***************************************************************************************
  * 本菜单为游戏菜单的二级子菜单，用执行贪吃蛇游戏，并且保存四个最高的得分记录到FLASH
  * FLASH最后一页的第七八九十个半字由高到低存放得分最高的4个记录
  ***************************************************************************************
  */

#include "GameMenu_Snake.h"
#include "stm32f10x.h"                  // Device header

#include "Key.h"
#include "Menu.h"
#include "OLED.h"
#include "MyRTC.h"
#include <stdio.h>
#include "Store.h"
#include "Timer.h"
#include <stdlib.h>
#include "Encoder.h"

#include "MSG.h"

/*图像数据*********************/

/*	贪吃蛇游戏菜单图标	——	盯着食物的S形蛇	*/
const uint8_t GameMenu_Snake_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,
	0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,
	0xC7,0xC7,0xC7,0xC7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x1F,0x1F,0x1F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,
	0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFF,0xFF,0x71,0xBF,0xBF,0xE3,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*	蛇身的图像数据	-	为一个5*5的方块*/
const uint8_t BodyImage[] = {
	0x1F,0x1F,0x1F,0x1F,0x1F,
};

/*	蛇头的图像数据，分别对应四个方向*/
const uint8_t HeadImage[4][5] = {
	0x1F,0x19,0x1F,0x19,0x1F,
	0x1F,0x15,0x15,0x1F,0x1F,
	0x1F,0x13,0x1F,0x13,0x1F,
	0x1F,0x1F,0x15,0x15,0x1F,
};

/*********************图像数据*/





/*宏参数定义*********************/

/*	游戏有几行	-	减一则为纵坐标最大值*/
#define ROW 11
/*	游戏有几列	-	减一则为横坐标最大值*/
#define COL 25
/*	游戏开始后蛇一开始每多少毫秒走一格	*/
#define TIME_INTERVAL  1000

/*********************宏参数定义*/





/*全局变量*********************/

/*	贪吃蛇游戏菜单结构体	*/
Menu GameMenu_Snake;	

/*	贪吃蛇游戏菜单名字	*/
const char* GameMenu_Snake_Name = "贪吃蛇";

/*	贪吃蛇游戏结构体		*/
typedef struct SnakeBody{
	struct SnakeBody* Last;	//	指向上一个蛇身
	struct SnakeBody* Next;	//	指向下一个蛇身
	int8_t X;	//	蛇身的横坐标
	int8_t Y;	//	蛇身的纵坐标
}SnakeBody;
static struct{
	SnakeBody* Head;	//	蛇头指针，指向蛇头
	SnakeBody* Tail;	//	蛇尾指针，指向蛇尾
	SnakeBody Body[COL][ROW];	//	格子(蛇身)数组，存放所有格子(蛇身和非蛇身)的数据
	int8_t Direction;	//	蛇前进的方向
	uint16_t Score;		//	当前游戏得分，游戏难度会随着得分的提高而提高
	struct{	int8_t X;	int8_t Y;}Food;	//	食物的坐标位置
}Game;



/*	菜单状态标志	*/
static uint8_t Status;	

/*********************全局变量*/





/*枚举*********************/

/*	菜单状态枚举	*/
enum {
	MAIN,	//	主菜单
	GAME,	//	游戏进行中
	RECORD,	//	记录菜单
	HELP,	//	帮助菜单
	PAUSE	//	暂停菜单
};

/*	蛇移动方向枚举	*/
enum {
	UP,		//	向上
	LEFT,	//	向左
	DOWN,	//	向下
	RIGHT	//	向右
};

/*********************枚举*/





/*功能函数*********************/

/**
  * 函    数：在一个随机且合理的地方生成食物
  * 参    数：无
  * 返 回 值：无
  */
static void Snake_CreateFood(void)
{
	int8_t X = -1;	//	临时横坐标变量
	int8_t Y = -1;	//	临时纵坐标变量
	
	do{
		X = rand() % COL;	//	随机生成一个横坐标
		Y = rand() % ROW;	//	随机生成一个纵坐标
		//	倘若随机生成的坐标已经被蛇身占用，则再进行生成，直到得到一个合理的生成位置
	}while( (Game.Body[X][Y].Last != NULL) ||  (Game.Body[X][Y].Next != NULL) || (Game.Head == &Game.Body[X][Y]) );
	Game.Food.X = X;	//	记录新的食物横坐标位置
	Game.Food.Y =Y;	//	记录新的食物总坐标位置
}

/**
  * 函    数：初始化贪吃蛇游戏
  * 参    数：无
  * 返 回 值：无
  */
static void Snake_Init(void)
{
	int8_t i = 0;	//	循环计数变量
	
	SnakeBody body;		//	临时蛇身结构体变量，用于临时记录蛇头数据
	body.Next = NULL;	//	初始化蛇头前指针为空
	body.Last = NULL;	//	初始化蛇头后指针为空
	body.X = COL/2;		//	初始化蛇头的横坐标，使其位于正中间的格子
	body.Y = ROW/2;		//	初始化蛇头的纵坐标，使其位于正中间的格子
	
	for(i=0;i<COL;++i)	//	初始化所有的格子
	{
		uint8_t j = 0;
		for(j=0;j<ROW;++j)
		{
			Game.Body[i][j].Last = NULL;		//	初始化格子前指针
			Game.Body[i][j].Next = NULL;		//	初始化格子后指针
			Game.Body[i][j].X = -1;			//	初始化格子横坐标
			Game.Body[i][j].Y = -1;			//	初始化格子纵坐标
		}
	}
	
	Game.Body[COL/2][ROW/2] = body;	//	将蛇头放入正中间的格子
	Game.Head = &( Game.Body[COL/2][ROW/2] );	//	使蛇头指针指向蛇头
	Game.Tail = &( Game.Body[COL/2][ROW/2] );	//	使蛇尾指针指向蛇尾
	Game.Direction = LEFT;	//	使蛇一开始向左移动
	Game.Score = 0;	//	初始化游戏得分为0
	
	Game.Food.X = -1;	//	初始化食物的横坐标
	Game.Food.Y = -1;	//	初始化食物的纵坐标
	Snake_CreateFood();	//	生成第一个食物
}

/**
  * 函    数：显示蛇的身体，食物，游戏边框和当前得分
  * 参    数：无
  * 返 回 值：无
  */
static void Snake_Display(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	/*显示得分*/
	OLED_Printf( 0, 0, OLED_6X8, "Score:%5d", Game.Score);
	/*显示得分*/
	
	/*显示边框*/
	OLED_DrawLine(0,7,127,7);
	OLED_DrawLine(0,63,127,63);
	OLED_DrawLine(0,0,0,63);
	OLED_DrawLine(1,0,1,63);
	OLED_DrawLine(127,0,127,63);
	/*显示边框*/
	
	/*显示蛇身*/
	for(i=0;i<COL;++i)
	{
		uint8_t j = 0;
		for(j=0;j<ROW;++j)
		{
			if( (&Game.Body[i][j]) == Game.Head )
			{
				OLED_ShowImage(2+i*5,8+j*5,5,5,HeadImage[Game.Direction]);
			}
			else if( (Game.Body[i][j].Last != NULL) || (Game.Body[i][j].Next != NULL) )
			{
				OLED_ShowImage(2+i*5,8+j*5,5,5,BodyImage);
			}
		}
	}
	/*显示蛇身*/
	
	/*显示食物位置*/
	OLED_DrawRectangle( 2+Game.Food.X*5, 8+Game.Food.Y*5, 5, 5, OLED_UNFILLED);
	/*显示食物位置*/
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：使蛇向左转头
  * 参    数：无
  * 返 回 值：无
  */
static void Snake_Left(void)
{
	int8_t tmp = Game.Direction;	//	临时变量，用于临时存放方向数据
	++(tmp);	//	加一，使方向变为下一个
	
	if(tmp>RIGHT)	//	防止越界
		tmp = UP;
	
	//	判断转变方向后蛇是否会"拧断脖子"，即两次即以上的短时间内连续转向可能会使蛇头转向180°，甚至更多
	//	这是需要避免的情况，蛇头只能转向0°或者90°
	switch(tmp)	
	{
		case UP:
			if(Game.Head->Y == (Game.Head->Next->Y+1))
				return;
			break;
		case DOWN:
			if(Game.Head->Y == (Game.Head->Next->Y-1))
				return;
			break;
		case LEFT:
			if(Game.Head->X == (Game.Head->Next->X+1))
				return;
			break;
		case RIGHT:
			if(Game.Head->X == (Game.Head->Next->X-1))
				return;
			break;
	}
	
	Game.Direction = tmp;	//	将处理好的临时方向数据保存下来
}

/**
  * 函    数：使蛇向右转头
  * 参    数：无
  * 返 回 值：无
  */
static void Snake_Right(void)
{
	int8_t tmp = Game.Direction;	//	临时变量，用于临时存放方向数据
	
	--(tmp);	//	减一，使方向变为上一个
	
	if(tmp<UP)	//	防止越界
		tmp = RIGHT;
	
	//	判断转变方向后蛇是否会"拧断脖子"，即两次即以上的短时间内连续转向可能会使蛇头转向180°，甚至更多
	//	这是需要避免的情况，蛇头只能转向0°或者90°
	switch(tmp)
	{
		case UP:
			if(Game.Head->Y == (Game.Head->Next->Y+1))
				return;
			break;
		case DOWN:
			if(Game.Head->Y == (Game.Head->Next->Y-1))
				return;
			break;
		case LEFT:
			if(Game.Head->X == (Game.Head->Next->X+1))
				return;
			break;
		case RIGHT:
			if(Game.Head->X == (Game.Head->Next->X-1))
				return;
			break;
	}
	
	Game.Direction = tmp;	//	将处理好的临时方向数据保存下来
}

/**
  * 函    数：保存游戏最高的四个游戏得分记录到FLASH
  * 参    数：无
  * 返 回 值：FLASH最后一页的第七八九十个半字由高到低存放得分最高的4个记录
  */
static void Snake_Save(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	for(i=0;i<4;++i)	//	FLASH最后一页的第三四五六个半字由高到低存放得分最高的4个记录
			{
				if(Game.Score >= Store_Data[6+i])
				{
					uint8_t j = 0;
					for(j=3;j>i;--j)
					{
						Store_Data[6+j] = Store_Data[6+j-1];
					}
					Store_Data[6+i] = Game.Score;
					Store_Save();	//	保存到FLASH中
					break;
				}
			}
}

//	声明游戏主菜单视图显示
static void GameMenu_Snake_View_MAIN(void);
/**
  * 函    数：贪吃蛇游戏循环
  * 参    数：无
  * 返 回 值：无
  * 说    明：游戏时应通过定时器每1s执行一次
  *           该函数决定了蛇如何移动(在老蛇头的前方生成一个新蛇头替代它)，
  *           判断是否吃到了食物，游戏是否应该结束了
  *           是否应该增加得分，增加多少得分，以及随得分调整游戏难度
  */
void Snake_Loop(void)
{
	uint8_t flag = 0;	//	标志，用于表示游戏是否应该结束了
	
	int8_t HeadX = Game.Head->X;	//	临时变量，记录蛇头一开始的横坐标，之后由判断得出新蛇头的横坐标
	int8_t HeadY = Game.Head->Y;	//	临时变量，记录蛇头一开始的纵坐标，之后由判断得出新蛇头的纵坐标
	
	//	由蛇的前进方向和蛇身的坐标数据进行判断，
	//	如果蛇按照这个方向前进一个之后碰到了自己或者边界
	//	那么就会置 flag 变量为一，表示游戏结束了
	//	如果不会碰到自己或者边界
	//	那么会对 HeadX 或 HeadY 进行加减得出新的蛇头坐标
	switch(Game.Direction)
	{
		case UP:
		{
			if( HeadY-1 < 0)
				flag = 1;
			else if( (Game.Body[ HeadX ][ HeadY-1 ].Last != NULL) || (Game.Body[ HeadX ][ HeadY-1 ].Next != NULL) )
				flag = 1;
			else
				--HeadY;
			break;
		}
		case DOWN:
		{
			if( HeadY+1 >= ROW )
				flag = 1;
			else if( (Game.Body[ HeadX ][ HeadY+1 ].Last != NULL) || (Game.Body[ HeadX ][ HeadY+1 ].Next != NULL) )
				flag = 1;
			else
				++HeadY;
			break;
		}
		case LEFT:
		{
			if( HeadX-1 < 0 )
				flag = 1;
			else if( (Game.Body[ HeadX-1 ][ HeadY ].Last != NULL) || (Game.Body[ HeadX-1 ][ HeadY ].Next != NULL) )
				flag = 1;
			else
				--HeadX;
			break;
		}
		case RIGHT:
		{
			if( HeadX+1 >= COL )
				flag = 1;
			else if( (Game.Body[ HeadX+1 ][ HeadY ].Last != NULL) || (Game.Body[ HeadX+1 ][ HeadY ].Next != NULL) )
				flag = 1;
			else
				++HeadX;
		}
	}
	
	if(flag == 1)	//	如果游戏应该结束了
	{
		Timer_Stop(Snake_Loop);	//	定时器不再为贪吃蛇游戏循环函数定时
		
		Snake_Save();	//	保存得分记录
		
		Status = MAIN;	//	转变状态为 MAIN
		GameMenu_Snake_View_MAIN();	//	显示游戏主菜单
		return;
	}
	else
	{
		SnakeBody body;	//	临时蛇身结构体变量，用于临时记录新蛇头数据
		body.X = HeadX;	//	初始化新蛇头的横坐标
		body.Y = HeadY;	//	初始化新蛇头的纵坐标
		body.Last = NULL;	//	初始化新蛇头的前指针为空
		body.Next = &( Game.Body[ Game.Head->X ][ Game.Head->Y ] );//初始化新蛇头的后指针，指向老蛇头
		Game.Body[ HeadX ][ HeadY ] = body;	//	将新蛇头放入对应的格子中
		Game.Head->Last = &(Game.Body[ HeadX ][ HeadY ]);	//	使老蛇头的前指针指向新蛇头
		Game.Head = &( Game.Body[ HeadX ][ HeadY ] );	//	使蛇头指针指向新蛇头
		
		if(Game.Food.X == HeadX && Game.Food.Y == HeadY)	//	如果蛇头遇到食物
		{
			Game.Score += 100 * (1+Game.Score/1000);	//	吃到食物得100分，每满一千分，得分额外加100
			
			//	随着得分的提高，缩短两次下落之间的时间间隔以提高游戏难度，
			//	2000分时达到最高难度,间隔由一开始的1000ms缩短为200ms
			if(Game.Score <= 2000)	
			{
				Timer_Stop(Snake_Loop);	//	定时器不再为贪吃蛇游戏循环函数定时，防止打扰后续的目标和当前定时值修改
				Timer_SetCount(Snake_Loop,0);	//	重置贪吃蛇游戏循环函数当前定时值
				Timer_SetTarget(Snake_Loop,TIME_INTERVAL-0.8*TIME_INTERVAL*(Game.Score/2000.0));//重置贪吃蛇游戏循环函数目标定时值
				Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
			}
			Snake_CreateFood();	//	新生成一个食物
		}
		else	//	如果蛇头没遇到食物
			{	//	对应新蛇头得生成，为了保持蛇身长度不变，要删除老蛇尾
			Game.Tail = Game.Tail->Last;	//	使蛇尾指针指向新蛇尾
			Game.Tail->Next->Last = NULL;	//	删除老蛇尾前指针数据
			Game.Tail->Next->Next = NULL;	//	删除老蛇尾后指针数据
			Game.Tail->Next->X = -1;	//	删除老蛇尾横坐标数据
			Game.Tail->Next->Y = -1;	//	删除老蛇尾纵坐标数据
		}
	}
	
	Snake_Display();	//	显示蛇的身体，食物，游戏边框和当前得分
}


/**
  * 函    数：游戏主菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Snake_View_MAIN(void)
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
  * 函    数：游戏暂停菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Snake_View_PAUSE(void)
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
static void GameMenu_Snake_View_HELP(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowChinese(0, 0,"键一左转键二右转");
	OLED_ShowChinese(0,16,"键四暂停");
	OLED_ShowChinese(0,32,"旋钮逆时旋转左转");
	OLED_ShowChinese(0,48,"旋钮顺时旋转右转");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：游戏记录菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void GameMenu_Snake_View_RECORD(void)
{
	uint8_t i = 0;	//	循环计数变量
	
	OLED_Clear();	//	清屏
	
	for(i=0;i<4;++i)
	{
		OLED_Printf(0,0+i*16,OLED_8X16,"NO.%d : %5d",i+1,Store_Data[6+i]);	//	显示得分最高的四个记录
	}
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：贪吃蛇游戏菜单的功能函数
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于贪吃蛇游戏菜单的功能函数
  */
static void GameMenu_Snake_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	Status = MAIN;	//	初始化状态为 MAIN
	
	GameMenu_Snake_View_MAIN();	//	游戏主菜单视图显示
	
	//	将Snake_Loop函数添加到定时器定时列表，配置为每1s到时一次
	Timer_AddFunction( Snake_Loop, TIME_INTERVAL);
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Snake_Init();	//	初始化贪吃蛇游戏
					Timer_SetTarget(Snake_Loop,TIME_INTERVAL);
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Snake_Loop);	//	停止为贪吃蛇游戏循环函数定时，防止打扰蛇的转头操作
					Snake_Left();	//	蛇向左转头
					Snake_Display();	//	显示蛇的身体，食物，游戏边框和当前得分
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
				}
				
				else if(Status == PAUSE)	/*	暂停	*/
				{
					Status = GAME;	//	转换状态为 GAME
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Timer_DeleteFunction(Snake_Loop);	//	将Snake_Loop函数从定时器定时列表删除
					return;
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Timer_Stop(Snake_Loop);	//	停止为贪吃蛇游戏循环函数定时，防止打扰蛇的转头操作
					Snake_Right();	//	蛇向右转头
					Snake_Display();	//	显示蛇的身体，食物，游戏边框和当前得分
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
				}
				else if(Status == RECORD)/*	记录	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Snake_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == HELP)/*	帮助	*/
				{
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Snake_View_MAIN();	//	游戏主菜单视图显示
				}
				else if(Status == PAUSE)/*	暂停	*/
				{
					Snake_Save();	//	保存得分记录
					Status = MAIN;	//	转换状态为 MAIN
					GameMenu_Snake_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = RECORD;	//	转换状态为 RECORD
					GameMenu_Snake_View_RECORD();	//	游戏记录菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = HELP;	//	转换状态为 HELP
					GameMenu_Snake_View_HELP();	//	游戏帮助菜单视图显示
				}
				else if(Status == GAME)	/*	游戏中	*/
				{
					Status = PAUSE;	//	转换状态为 PAUSE
					Timer_Stop(Snake_Loop);	//	停止为贪吃蛇游戏循环函数定时，暂停
					GameMenu_Snake_View_PAUSE();	//	游戏暂停菜单视图显示
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Snake_Loop);	//	停止为贪吃蛇游戏循环函数定时，防止打扰蛇的转头操作
					Snake_Right();	//	蛇向右转头
					Snake_Display();	//	显示蛇的身体，食物，游戏边框和当前得分
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(Status == GAME)	/*	初始界面	*/
				{
					Timer_Stop(Snake_Loop);	//	停止为贪吃蛇游戏循环函数定时，防止打扰蛇的转头操作
					Snake_Left();		//	蛇向左转头
					Snake_Display();	//	显示蛇的身体，食物，游戏边框和当前得分
					Timer_Start(Snake_Loop);	//	开始为贪吃蛇游戏循环函数定时
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
  * 函    数：初始化贪吃蛇游戏菜单
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了贪吃蛇游戏菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void GameMenu_Snake_Init(void)
{
	/*初始化贪吃蛇游戏菜单*/
	Menu_Init(&GameMenu_Snake);
	GameMenu_Snake.Funsion = GameMenu_Snake_Funsion;
	GameMenu_Snake.Icon = GameMenu_Snake_Icon;
	GameMenu_Snake.Name = GameMenu_Snake_Name;
	/*初始化贪吃蛇游戏菜单*/
	
	Store_Init();	//	初始化Store模块
	MyRTC_Init();	//	初始化MyRTC模块
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
	
	srand((unsigned int)RTC_GetCounter());	//	初始化随机数生成
}

/*********************功能函数*/

