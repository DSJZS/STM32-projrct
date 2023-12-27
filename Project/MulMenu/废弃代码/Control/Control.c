#include "Control.h"
#include "stm32f10x.h" 
#include "OLED.h"
#include <stdio.h>

/*初始化函数*********************/

/**
  * 函    数：初始化控件
  * 参    数：Ctrl 初始化的控件的指针
  * 返 回 值：无
  * 说    明：会将控件结构体的所有参数赋值为零和空
  */
void Control_Init(Control* Ctrl)
{
	Ctrl->X = 0;
	Ctrl->Y = 0;
	Ctrl->Width = 0;
	Ctrl->Height = 0;
	Ctrl->ID = 0;
	Ctrl->View = NULL;
	Ctrl->Next = NULL;
	Ctrl->Funsion = NULL;
}

/**
  * 函    数：初始化控件列表
  * 参    数：CtrlList 初始化的控件列表
  * 返 回 值：无
  * 说    明：会将控件列表参数赋值为空
  */
void ControlList_Init(ControlList* CtrlList)
{
	CtrlList->List = NULL;
	CtrlList->Focus = NULL;
}

/*********************初始化函数*/





/*配置函数*********************/

/**
  * 函    数：设置控件的横坐标
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：X		横坐标
  * 返 回 值：无
  */
void Control_SetX( Control* Ctrl, uint8_t X)
{
	Ctrl->X = X;
}

/**
  * 函    数：设置控件的纵坐标
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：Y		纵坐标
  * 返 回 值：无
  */
void Control_SetY( Control* Ctrl, uint8_t Y)
{
	Ctrl->Y = Y;
}

/**
  * 函    数：设置控件的宽
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：Width		宽
  * 返 回 值：无
  */
void Control_SetWidth( Control* Ctrl, uint8_t Width)
{
	Ctrl->Width = Width;
}

/**
  * 函    数：设置控件的高
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：Height	高
  * 返 回 值：无
  */
void Control_SetHeight( Control* Ctrl, uint8_t Height)
{
	Ctrl->Height = Height;
}

/**
  * 函    数：设置控件的标识
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：ID		标识
  * 返 回 值：无
  */
void Control_SetID( Control* Ctrl, uint8_t ID)
{
	Ctrl->ID = ID;
}

/**
  * 函    数：设置控件的视图
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：View		视图
  * 返 回 值：无
  */
void Control_SetView( Control* Ctrl, const uint8_t* View)
{
	Ctrl->View = View;
}

/**
  * 函    数：设置控件的同级后菜单
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：NextCtrl 控件的同级后控件的指针
  * 返 回 值：无
  */
void Control_SetNextCtrl( Control* Ctrl, Control* NextCtrl)
{
	Ctrl->Next = NextCtrl;
}

/**
  * 函    数：设置控件的功能函数
  * 参    数：Ctrl 设置的控件的指针
  * 参    数：Funsion 控件的功能函数
  * 返 回 值：无
  */
void Control_SetFunsion( Control* Ctrl, void (*Funsion)(void))
{
	Ctrl->Funsion = Funsion;
}

/*********************配置函数*/





/*功能函数*********************/

/**
  * 函    数：执行控件的功能函数
  * 参    数：无
  * 返 回 值：执行成功会返回 1，执行失败返回 0
  * 说    明：如果控件没有功能函数，则无事发生
  */
uint8_t Control_ToFunsion(Control* Ctrl)
{
	if(Ctrl->Funsion != NULL)
	{
		Ctrl->Funsion();
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：反转(反色)控件的视图显示
  * 参    数：Ctrl	反转显示的控件
  * 返 回 值：无
  * 说    明：控件的视图数组不会改变，只是显示改变了
  */
void Control_Reverse(Control* Ctrl)
{
	OLED_ReverseArea( Ctrl->X, Ctrl->Y, Ctrl->Width, Ctrl->Height);
	OLED_Update();
}

/**
  * 函    数：显示控件视图
  * 参    数：Ctrl	显示的控件
  * 返 回 值：执行成功会返回 1，执行失败返回 0
  * 说    明：如果控件没有视图，则无事发生
  */
uint8_t Control_Display(Control* Ctrl)
{
	if(Ctrl->View != NULL)
	{
		OLED_ShowImage( Ctrl->X, Ctrl->Y, Ctrl->Width, Ctrl->Height, Ctrl->View);
		OLED_Update();
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * 函    数：控件关闭操作
  * 参    数：Ctrl	关闭的控件
  * 返 回 值：无
  * 说    明：关闭一个控件前需要执行该函数
  */
void Control_Close(Control* Ctrl)
{
	OLED_ClearArea( Ctrl->X, Ctrl->Y, Ctrl->Width, Ctrl->Height);
}

/**
  * 函    数：在控件列表中添加控件
  * 参    数：CtrlList	控件列表
  * 参    数：Ctrl		添加的控件的指针
  * 参    数：ID		给添加控件分配的ID，ID取值范围：1~255
  *						若要自动分配ID则输入	CTRL_AUTO_ID
  * 返 回 值：无
  */
void ControlList_AddCtrl( ControlList* CtrlList, Control* Ctrl, uint8_t ID)
{
	if(CtrlList->List == NULL)
	{
		if(ID == CTRL_AUTO_ID)
			Ctrl->ID = 1;
		else
			Ctrl->ID = ID;
		CtrlList->List = Ctrl;
	}
	else
	{
		uint8_t Count = 2;
		Control* tmp = CtrlList->List;
		while(tmp->Next != NULL)
		{
			++Count;
			tmp = tmp->Next;
		}
		if(ID == CTRL_AUTO_ID)
		{
			Ctrl->ID = Count;	
		}
		else
		{
			Ctrl->ID = ID;
		}
		tmp->Next = Ctrl;
	}
}

/**
  * 函    数：在控件列表中查找控件
  * 参    数：CtrlList	控件列表
  * 参    数：ID		目前控件的ID，ID取值范围：1~255
  * 返 回 值：查找成功会返回目标控件的地址，执行失败返回NULL
  */
Control* ControlList_FindCtrl( ControlList* CtrlList, uint8_t ID)
{
	Control* tmp = CtrlList->List;
	while(tmp != NULL)
	{
		if(tmp->ID == ID)
		{
			return tmp;
		}
		tmp = tmp->Next;
	}
	return NULL;
}

/*功能函数*********************/
