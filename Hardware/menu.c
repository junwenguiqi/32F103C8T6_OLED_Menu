#include "menu.h"
#include "OLED.h"
#include "Key.h"
#include "MPU6050.h"
#include "Serial.h"
#include "Servo.h"
//索引数组相关
void (*current_operation_index)();  		// 函数指针，用于执行当前操作函数
uint8_t func_index = 0;									// 当前功能索引

//定义接收按键返回值变量
uint8_t key_value = 0;									// 存储接收到的按键值

//定义PID菜单----PID变量
enum Param_State ParamState = PARAM_Bkp; // 当前PID参数修改状态
float *selectedParam = &Balan_Kp;							// 指向当前修改的参数变量

//定义Servo菜单----Servo变量
float Servo_Angle = 0.0;								// 用于存储舵机角度

/*定义了索引数组，索引数组要配合主程序按键值进行选择*/
 Menu_table  table[26]=
{
    /*****************一级画面**************************/
    //序号,下一项,确定，返回
    {0, 0, 1, 0,(*UI_Start_Screen)}, 
		
		/*****************二级画面**************************/
    {1, 2, 5, 0,(*MPU6050_Show)},				//二级界面   MPU6050实时参数
		{2, 3, 6, 0,(*Balance_PID)},						//二级界面   MPU6050实时参数--需要改参数
		{3, 4, 7, 0,(*Servo_Show)},					//二级界面   PID设置参数--需要改参数
		{4, 1, 4, 0,(*text3)},							//二级界面   MPU6050实时参数
/*
typedef struct
{
    u8 current;//当前状态索引号
    u8 next; //向下一个
    u8 enter; //确定
    u8 back; //返回
    void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;
按键1---向下导航到下一个菜单项 
按键2---//进入当前菜单项
按键3---//返回上一个菜单项
按键4---//PID菜单下 参数的加操作
按键5---//PID菜单下 参数的减操作
按键6---//返回一级菜单
*/    
    /*****************三级画面**************************/	
    //MPU6050实时参数的三级菜单
    {5, 5, 5, 1, (*Get_Show_MPU6050)}, 
		//PID参数的三级菜单 --需要改参数
		{6, 6, 8, 2, (*Get_Show_Balance_PID)},
		//Servo参数的三级菜单--需要改参数
		{7, 7, 11, 3, (*Get_Show_Servo)},
		/*****************四级画面***************************/
		{8, 9, 8, 6, (*Modify_Balance_PID_KP)},
		{9, 10, 9, 6, (*Modify_Balance_PID_KI)},
		{10, 8, 10, 6, (*Modify_Balance_PID_KD)},
		{11, 11, 11, 7, (*Modify_Servo_Angle)},		
};




//一级菜单
void UI_Start_Screen(void)  							//UI初始画面
{
	OLED_ShowString(2, 3, "huakaikanzhe!");
}

/*****************************************************************************
 * @函数名称   : void UI_Second_Screen(void)
 * @函数功能   : 显示二级菜单
******************************************************************************/
void UI_Second_Screen(void)   						//二级菜单
{
	OLED_ShowString(1,3,"  MPU6050");
	OLED_ShowString(2,3,"  Balance_PID");
	OLED_ShowString(3,3,"  Velocity_PID");
	OLED_ShowString(4,3,"  Turn_PID");
}

/*****************************************************************************
 * @函数名称   : void MPU6050_Show(void)
 * @函数功能   : 显示二级菜单MPU6050
******************************************************************************/
void MPU6050_Show(void)										//二级菜单MPU6050实时参数
{
	UI_Second_Screen();  										// 显示二级菜单选项
	ShowMenuArrow(1);												// 在选项前显示箭头
}

/*****************************************************************************
 * @函数名称   : void Get_Show_MPU6050(void)
 * @函数功能   : 显示三级菜单MPU6050实时参数
******************************************************************************/
void Get_Show_MPU6050(void) 							//三级菜单MPU6050实时参数
{
	OLED_ShowString(1,1,"roll: ");
	OLED_ShowString(2,1,"pitch: ");
	OLED_ShowSignedNum(1,8,roll,5);					// 在指定位置显示带符号的整数
	OLED_ShowSignedNum(2,8,pitch,5);				// 在指定位置显示带符号的整数
}

/*****************************************************************************
 * @函数名称   : void PID_Show(void) 
 * @函数功能   : 显示二级菜单PID
******************************************************************************/
void Balance_PID(void)                       //二级菜单PID
{
	UI_Second_Screen();
	ShowMenuArrow(2);
}

/*****************************************************************************
 * @函数名称   : void Get_Show_PID(void) 
 * @函数功能   : 显示三级菜单PID各参数
******************************************************************************/
void Get_Show_Balance_PID(void)                   //三级菜单PID参数
{
	//显示
	OLED_ShowString(1,3,"BKp: ");
	OLED_ShowString(2,3,"BKi: ");
	OLED_ShowString(3,3,"BKd: ");
	OLED_ShowSignedFloat(1,8,Balan_Kp,2);
	OLED_ShowSignedFloat(2,8,Balan_Ki,2);
	OLED_ShowSignedFloat(3,8,Balan_Kd,2);
}	

/*****************************************************************************
 * @函数名称   : void Modify_PID_KP(void)	
 * @函数功能   : 显示PID——KP参数
******************************************************************************/
void Modify_Balance_PID_KP(void)			           //四级菜单修改KP参数     
{
	ParamState = PARAM_Bkp;
	selectedParam = &Balan_Kp;
	ShowMenuArrow(1);
	Get_Show_Balance_PID();
}

/*****************************************************************************
 * @函数名称   : void Modify_PID_KI(void)	
 * @函数功能   : 显示PID——KI参数
******************************************************************************/
void Modify_Balance_PID_KI(void)			           //四级菜单修改KI参数 
{
	ParamState = PARAM_Bki;
	selectedParam = &Balan_Ki;
		ShowMenuArrow(2);
		Get_Show_Balance_PID();
}

/*****************************************************************************
 * @函数名称   : void Modify_PID_KD(void)	
 * @函数功能   : 显示PID——KD参数
******************************************************************************/
void Modify_Balance_PID_KD(void)			           //四级菜单修改KD参数
{
	ParamState = PARAM_Bkd;
	selectedParam = &Balan_Kd;
		ShowMenuArrow(3);
		Get_Show_Balance_PID();
}

/*****************************************************************************
 * @函数名称   : void Modify_Param(enum Param_State state, float *selectedParam, float changeStep, float minVal, float maxVal)
 * @函数功能   : 修改参数通用函数
 * @函数入口参数: enum Param_State state ---> 对应现在的参数状态具体是什么
									float *selectedParam	 ---> 对应参的存储
									float changeStep       ---> 每一步加或减操作的变化步长
									float minVal, float maxVal  --->  最小值和最大值，限制此最小值和最大值，避免参数修改后超过合理范围
******************************************************************************/
void Modify_Param(enum Param_State state, float *selectedParam, float changeStep, float minVal, float maxVal)
{
    if (ParamState == state) 
		{
        if (key_value == 4) 
				{
            *selectedParam += changeStep;
        } 
				else if (key_value == 5) 
				{
            *selectedParam -= changeStep;
        }
        *selectedParam = fmaxf(minVal, fminf(maxVal, *selectedParam));
    }
}

/*****************************************************************************
 * @函数名称   : void ModifyParam(void)
 * @函数功能   : 总修改参数函数
******************************************************************************/
void ModifyParam(void)
{
	if (ParamState == PARAM_Bkp || ParamState == PARAM_Bki || ParamState == PARAM_Bkd)
	{
		Modify_Param(ParamState, selectedParam, 0.01, -100.0, 100.0);// 调用通用修改参数函数
	} 
	else if (ParamState == PARAM_Servo_Angle) 
	{
			Modify_Param(ParamState, &Servo_Angle, 30.0, 0.0, 180.0);// 调用通用修改参数函数
	}
}

/*****************************************************************************
 * @函数名称   : void Servo_Show(void)
 * @函数功能   : 显示二级菜单Servo
******************************************************************************/
void Servo_Show(void)							  //二级菜单Servo
{
	UI_Second_Screen();								// 显示二级菜单选项
	ShowMenuArrow(3);   							// 在选项前显示箭头
}

/*****************************************************************************
 * @函数名称   : void Get_Show_Servo(void)
 * @函数功能   : 显示三级菜单Servo
******************************************************************************/
void  Get_Show_Servo(void) 				             //三级菜单Servo
{
	OLED_ShowString(1, 3, "Angle:");
	OLED_ShowSignedNum(1,8,Servo_Angle,5);       // 在指定位置显示带符号的整数
}

/*****************************************************************************
 * @函数名称   : void Modify_Servo_Angle(void)	
 * @函数功能   : 显示Servo_Angle参数
******************************************************************************/
void Modify_Servo_Angle(void)					         //四级菜单Servo
{
	ParamState = PARAM_Servo_Angle;
	selectedParam = &Servo_Angle;
	ShowMenuArrow(1);
	Get_Show_Servo();
	Servo_SetAngle(Servo_Angle);
}

//text
void text3(void)
{
	UI_Second_Screen();
	ShowMenuArrow(4);
}
//显示箭头
void ShowMenuArrow(uint8_t row)
{
    OLED_ShowString(row, 1, "->");
}

void Menu_Init(void) 													//菜单初始化
{
	Key_Init();// 初始化按键
	OLED_Init();// 初始化OLED屏幕
	MPU6050_Init();// 初始化OLED屏幕
	Serial2_Init(115200);// 初始化串口2
	OLED_Clear(); // 清空OLED屏幕
	//Servo_Init();// 初始化舵机
}

/*****************************************************************************
 * @函数名称   : uint8_t key_value_Get(void) 
 * @函数功能   : 获取按键值
******************************************************************************/
uint8_t key_value_Get(void) 									//通过串口2获取key_value
{
	uint8_t key_num = 0;
	uint8_t RxData = 0;
	if (Serial_GetRxFlag(USART2) == UART2_Rx_Get)
	{
		RxData =Serial_GetRxData(USART2);
		switch(RxData)
		{
			case 0x01 :
				key_num = 1;
				break;
			case 0x02 :
				key_num = 2;
				break;
		  case 0x03 :
				key_num = 3;
				break;
		  case 0x04 :
				key_num = 4;
				break;
			case 0x05 :
				key_num = 5;
				break;
			case 0x06 :
				key_num = 6;
				break;
			default:
				break;			
		}
		Serial_SendByte(USART2, key_num);	
		Serial_SendByte(USART2, func_index);	
	}

	return  key_num;
}
//按键
/*****************************************************************************
 * @函数名称   : void Menu_key_set(void)
 * @函数功能   : 处理按键操作
******************************************************************************/
void Menu_key_set(void)
{
   // key_value = key_value_Get();								//通过串口形式接收
		key_value = Key_GetNum();
    switch (key_value)
    {
        case 1:
            func_index = table[func_index].next;//按键next按下后,向下导航到下一个菜单项 
				    OLED_Clear();  											//OLED清屏
            break;
        case 2:
            func_index = table[func_index].enter;//进入当前菜单项
				    OLED_Clear(); 
            break;
        case 3:
            func_index = table[func_index].back;//返回上一个菜单项
				    OLED_Clear();  
            break;
        case 4:																	// 按键4加操作
        case 5:																	// 按键5减操作
					  ModifyParam();
				    OLED_Clear();  
            break;
        case 6:
            func_index = 0;											// 返回到一级菜单
				    OLED_Clear();  
            break;
        default:
            break;
    }
    current_operation_index = table[func_index].current_operation;//执行当前索引号所对应的功能函数。
    (*current_operation_index)();								//执行当前操作函数
}


