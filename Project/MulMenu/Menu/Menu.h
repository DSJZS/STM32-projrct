#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h" 




/*参数宏定义*********************/

/*菜单图标的默认宽与高(具体宽高看各菜单源文件)*/	/*	未在程序中使用	*/
#define Menu_ICON_DEFAULT_WIDTH 	48	//	菜单图标的默认宽
#define Menu_ICON_DEFAULT_HEIGHT 	48	//	菜单图标的默认高

/*********************参数宏定义*/





/*结构体声明*********************/

/*	窗口结构体	*/
typedef struct Menu{
	const char* Name;			/*	菜单名														*/
	const uint8_t* Icon;		/*	菜单图标，上级菜单中显示的图标，宽高由宏定义				*/
	void (*Funsion)(void);		/*	菜单功能，用于处理各种操作									*/
	struct Menu* Father;		/*	父菜单指针，用于查找上一级菜单，为NULL表示该窗口无父菜单	*/
	struct Menu* Child;			/*	子菜单指针，用于查找下一级菜单，为NULL表示该窗口无子菜单	*/
	struct Menu* Last;			/*	前菜单指针，用于查找同级上一个菜单							*/
	struct Menu* Next;			/*	后菜单指针，用于查找同级下一个菜单							*/
}Menu;

/*********************结构体声明*/





/*函数声明*********************/

/*初始化函数*/
void Menu_Init(Menu* MU);

/*配置函数*/
void Menu_SetLast( Menu* MU, Menu* LastMenu);
void Menu_SetNext( Menu* MU, Menu* NextMenu);
void Menu_SetFunsion( Menu* MU, void (*Funsion)(void));
void Menu_SetFather( Menu* MU, Menu* FatherMenu);
void Menu_SetChild( Menu* MU, Menu* ChildMenu);
void Menu_SetIcon( Menu* MU, const uint8_t* Icon);
void Menu_SetName( Menu* MU, const char* Name);
void Menu_SetCurrent(Menu* MU);

/*功能函数*/
uint8_t Menu_ToLast(void);
uint8_t Menu_ToNext(void);
uint8_t Menu_ToFather(void);
uint8_t Menu_ToChild(void);
uint8_t Menu_ToFunsion(void);
Menu* Menu_GetCurrent(void);
Menu* Menu_GetLast(void);
Menu* Menu_GetNext(void);
Menu* Menu_GetFather(void);
Menu* Menu_GetChild(void);
const char* Menu_GetName(void);
const uint8_t* Menu_GetIcon(void);

/*********************函数声明*/





#endif
