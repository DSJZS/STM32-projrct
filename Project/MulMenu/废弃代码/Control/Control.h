#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "stm32f10x.h" 





/**
  * 数据存储格式：
  * 纵向8点，高位在下，先从左到右，再从上到下
  * 每一个Bit对应一个像素点
  * 
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |   
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  * 
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  * 
  *       0             X轴           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */





/*参数宏定义*********************/

/*控件的默认宽与高(具体宽高看各控件源文件)*/
#define CONTROL_DEFAULT_WIDTH 	48	//	控件的默认宽
#define CONTROL_DEFAULT_HEIGHT 	48	//	控件的默认高

/*控件自动分配ID*/
#define CTRL_AUTO_ID 0

/*********************参数宏定义*/





/*变量声明*********************/

/*********************变量声明*/






/*结构体声明*********************/

/*	控件结构体	*/
typedef struct Control{
	uint8_t X;				/*	控件横坐标													*/
	uint8_t Y;				/*	控件纵坐标													*/
	uint8_t Width;			/*	控件宽度													*/
	uint8_t Height;			/*	控件高度													*/
	uint8_t ID;				/*	控件标识，用于标识控件										*/
	const uint8_t* View;	/*	控件视图(用户看到的控件模样)								*/
	struct Control* Next;	/*	后控件指针，用于查找同级下一个控件							*/
	void (*Funsion)(void);	/*	控件功能，用于处理各种操作									*/
}Control;

/*	控件列表结构体	*/
typedef struct ControlList{
	Control* List;
	Control* Focus;
}ControlList;

/*********************结构体声明*/





/*函数声明*********************/

/*初始化函数*/
void Control_Init(Control* Ctrl);

void ControlList_Init(ControlList* CtrlList);

/*配置函数*/
void Control_SetX( Control* Ctrl, uint8_t X);
void Control_SetY( Control* Ctrl, uint8_t Y);
void Control_SetWidth( Control* Ctrl, uint8_t Width);
void Control_SetHeight( Control* Ctrl, uint8_t Height);
void Control_SetID( Control* Ctrl, uint8_t ID);
void Control_SetView( Control* Ctrl, const uint8_t* View);
void Control_SetNextCtrl( Control* Ctrl, Control* NextCtrl);
void Control_SetFunsion( Control* Ctrl, void (*Funsion)(void));

/*功能函数*/
uint8_t Control_ToFunsion(Control* Ctrl);
void Control_Reverse(Control* Ctrl);
uint8_t Control_Display(Control* Ctrl);
void Control_Close(Control* Ctrl);

void ControlList_AddCtrl( ControlList* CtrlList, Control* Ctrl, uint8_t ID);
Control* ControlList_FindCtrl( ControlList* CtrlList, uint8_t ID);

/*********************函数声明*/





#endif
