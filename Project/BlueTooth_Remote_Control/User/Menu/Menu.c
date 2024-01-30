/***************************************************************************************
  * 本源文件含有多级菜单相关函数
  * 本项目的所有菜单都是依托这个文件里的内容执行
  ***************************************************************************************
  */
  
#include "Menu.h"
#include "stm32f10x.h" 
#include "./OLED/OLED.h"
#include <stdio.h>




/*全局变量*********************/

/* 当前菜单指针，指向当前聚焦的菜单 */
static Menu* Menu_Current;

/*********************全局变量*/





/*初始化函数*********************/

/**
  * 函    数：初始化菜单
  * 参    数：MU 初始化的菜单的指针
  * 返 回 值：无
  * 说    明：会将菜单结构体的所有参数赋值为空
  */
void Menu_Init(Menu* MU)
{
	MU->Name = NULL;
	MU->Icon = NULL;
	MU->Funsion = NULL;
	MU->Child = NULL;
	MU->Father = NULL;
	MU->Last = NULL;
	MU->Next = NULL;
}

/*********************初始化函数*/





/*配置函数*********************/

/**
  * 函    数：设置菜单的同级前菜单
  * 参    数：MU 设置的菜单的指针
  * 参    数：LastMenu 菜单的同级前菜单的指针
  * 返 回 值：无
  * 说    明：会同时将同级前菜单的同级后菜单赋值为MU
  */
void Menu_SetLast( Menu* MU, Menu* LastMenu)
{
	MU->Last = LastMenu;
	LastMenu->Next = MU;
}

/**
  * 函    数：设置菜单的同级后菜单
  * 参    数：MU 设置的菜单的指针
  * 参    数：NextMenu 菜单的同级后菜单的指针
  * 返 回 值：无
  * 说    明：会同时将同级后菜单的同级前菜单赋值为MU
  */
void Menu_SetNext( Menu* MU, Menu* NextMenu)
{
	MU->Next = NextMenu;
	NextMenu->Last = MU;
}

/**
  * 函    数：设置菜单的功能函数
  * 参    数：MU 设置的菜单的指针
  * 参    数：Funsion 菜单的功能函数
  * 返 回 值：无
  */
void Menu_SetFunsion( Menu* MU, void (*Funsion)(void))
{
	MU->Funsion = Funsion;
}

/**
  * 函    数：设置菜单的子菜单
  * 参    数：MU 设置的菜单的指针
  * 参    数：FatherMenu 父菜单的指针
  * 返 回 值：无
  * 说    明：会同时将子菜单MU的父菜单赋值为FatherMenu
  *           如果FatherMenu已经指向了一个子菜单，那么MU会为该子菜单的同级最后菜单
  * 个人理解：父亲只会有一个大儿子，后面的儿子不是父亲的大儿子，而是父亲大儿子的弟弟
  */
void Menu_SetFather( Menu* MU, Menu* FatherMenu)
{
	MU->Father = FatherMenu;
	
	if(FatherMenu->Child == NULL)
	{
		FatherMenu->Child = MU;
	}
	else
	{
		Menu* BrotherMenu = FatherMenu->Child;
		while( BrotherMenu->Next != NULL )
			BrotherMenu = BrotherMenu->Next;
		Menu_SetNext( BrotherMenu, MU);
	}
}

/**
  * 函    数：设置菜单的子菜单
  * 参    数：MU 设置的菜单的指针
  * 参    数：ChildMenu	子菜单的指针
  * 返 回 值：无
  * 说    明：会同时将父菜单MU的子菜单赋值为ChildMenu
  *           如果父菜单已经指向了一个子菜单，那么会判断该子菜单是否是 ChildMenu 的同级前后菜单
  *           如果是 ChildMenu 的同级前后菜单，则父菜单不再指向原子菜单，而是指向 ChildMenu ；
  *           反之，如果不是 ChildMenu 的同级前后菜单，则什么也不会发生
  * 个人理解：父亲只有一个最爱的儿子，只有大儿子时就是大儿子，但是小儿子有了后就不一定了
  *           此外和父亲亲生的儿子们没有血缘关系的"儿子"，不会是父亲最爱的儿子
  */
void Menu_SetChild( Menu* MU, Menu* ChildMenu)
{
	ChildMenu->Father = MU;
	
	if(MU->Child == NULL)
	{
		MU->Child = ChildMenu;
	}
	else
	{
		Menu* tmp = ChildMenu;
		
		while(tmp->Last != NULL)
		{
			tmp = tmp->Last;
			if(MU->Child == tmp)
			{
				MU->Child = ChildMenu;
				return;
			}
		}
		while(tmp->Next != NULL)
		{
			tmp = tmp->Next;
			if(MU->Child == tmp)
			{
				MU->Child = ChildMenu;
				return;
			}
		}
	}
}

/**
  * 函    数：设置菜单的图标
  * 参    数：MU 设置的菜单的指针
  * 参    数：Icon 菜单图标数组
  * 返 回 值：无
  */
void Menu_SetIcon( Menu* MU, const uint8_t* Icon)
{
	MU->Icon = Icon;
}

/**
  * 函    数：设置菜单的名字
  * 参    数：MU 设置的菜单的指针
  * 参    数：Name 菜单名字字符串
  * 返 回 值：无
  * 说    明：名字字符串中的内容要为汉字或者全角字符
  */
void Menu_SetName( Menu* MU, const char* Name)
{
	MU->Name = Name;
}

/**
  * 函    数：设置当前菜单
  * 参    数：MU 设置的菜单的指针
  * 返 回 值：无
  * 说    明：程序执行后必须调用一次该函数
  */
void Menu_SetCurrent(Menu* MU)
{
	Menu_Current = MU;
}

/*********************配置函数*/





/*功能函数*********************/

/**
  * 函    数：将当前菜单向前移动，使同级前菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：移动成功会返回 1，移动失败返回 0
  * 说    明：如果当前菜单没有同级前菜单，则不会移动
  */
uint8_t Menu_ToLast(void)
{
	if(Menu_Current->Last != NULL)
	{
		Menu_Current = Menu_Current->Last;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：将当前菜单向后移动，使同级后菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：移动成功会返回 1，移动失败返回 0
  * 说    明：如果当前菜单没有同级后菜单，则不会移动
  */
uint8_t Menu_ToNext(void)
{
	if(Menu_Current->Next != NULL)
	{
		Menu_Current = Menu_Current->Next;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：使当前菜单的父菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：转换成功会返回 1，转换失败返回 0
  * 说    明：如果当前菜单没有父菜单，则无事发生
  */
uint8_t Menu_ToFather(void)
{
	if(Menu_Current->Father != NULL)
	{
		Menu_SetChild( Menu_Current->Father, Menu_Current);
		Menu_Current = Menu_Current->Father;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：使当前菜单的子菜单变为新的当前菜单
  * 参    数：无
  * 返 回 值：转换成功会返回 1，转换失败返回 0
  * 说    明：如果当前菜单没有子菜单，则无事发生
  */
uint8_t Menu_ToChild(void)
{
	if(Menu_Current->Child != NULL)
	{
		Menu_Current = Menu_Current->Child;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：执行当前菜单的功能函数
  * 参    数：无
  * 返 回 值：执行成功会返回 1，执行失败返回 0
  * 说    明：如果当前菜单没有功能函数，则无事发生
  */
uint8_t Menu_ToFunsion(void)
{
	if(Menu_Current->Funsion != NULL)
	{
		Menu_Current->Funsion();
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：返回当前菜单的指针
  * 参    数：无
  * 返 回 值：当前菜单的指针
  */
Menu* Menu_GetCurrent(void)
{
	return Menu_Current;
}

/**
  * 函    数：返回当前菜单的同级前菜单的指针
  * 参    数：无
  * 返 回 值：当前菜单的同级前菜单的指针
  */
Menu* Menu_GetLast(void)
{
	return Menu_Current->Last;
}

/**
  * 函    数：返回当前菜单的同级后菜单的指针
  * 参    数：无
  * 返 回 值：当前菜单的同级后菜单的指针
  */
Menu* Menu_GetNext(void)
{
	return Menu_Current->Next;
}

/**
  * 函    数：返回当前菜单的父菜单的指针
  * 参    数：无
  * 返 回 值：当前菜单的父菜单的指针
  */
Menu* Menu_GetFather(void)
{
	return Menu_Current->Father;
}

/**
  * 函    数：返回当前菜单的子菜单的指针
  * 参    数：无
  * 返 回 值：当前菜单的子菜单的指针
  */
Menu* Menu_GetChild(void)
{
	return Menu_Current->Child;
}

/**
  * 函    数：返回当前菜单的菜单名
  * 参    数：无
  * 返 回 值：当前菜单的菜单名字符串
  */
const char* Menu_GetName(void)
{
	return Menu_Current->Name;
}

/**
  * 函    数：返回当前菜单的菜单图标
  * 参    数：无
  * 返 回 值：当前菜单的菜单图标数组
  */
const uint8_t* Menu_GetIcon(void)
{
	return Menu_Current->Icon;
}

/*功能函数*********************/
