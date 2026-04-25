#ifndef __MENU_H
#define __MENU_H
#include "stm32f10x.h"

typedef struct
{
    u8 current;                      //当前状态索引号
    u8 next;                         //向下一个
    u8 enter;                        //确定
    u8 back;                         //返回
    void (*current_operation)(void); //当前状态应该执行的操作
		void (*param_modify)(void);      // 参数修改操作函数
} Menu_table;

// 定义PID参数修改状态
enum Param_State {
    PARAM_Bkp = 1,
    PARAM_Bki,
    PARAM_Bkd,
		PARAM_Servo_Angle
};

extern float roll;
extern float pitch;
extern float Balan_Kp ;
extern float Balan_Ki ;
extern float Balan_Kd ;

extern float Ve_Kp ;
extern float Ve_Ki ;
extern float Ve_Kd ;

extern float Turn_Kp ;
extern float Turn_Ki ;
extern float Turn_Kd ;

extern float Servo_Angle ;

void ShowMenuArrow(uint8_t row); //显示箭头
void Menu_Init(void);            //菜单初始化
void UI_Start_Screen(void);      //UI初始画面
void UI_Second_Screen(void);     //二级菜单
void MPU6050_Show(void);         //MPU6050二级菜单
void Get_Show_MPU6050(void);		 //MPU6050三级显示参数菜单

void Balance_PID(void)  ;				   //PID二级菜单
void Get_Show_Balance_PID(void)    ;			 //PID三级显示参数菜单
void Modify_Param(enum Param_State state, float *selectedParam, float changeStep, float minVal, float maxVal);//修改参数函数
void Modify_Balance_PID_KP(void);        //KP菜单
void Modify_Balance_PID_KI(void);				 //KI菜单
void Modify_Balance_PID_KD(void);				 //KD菜单
	
void Servo_Show(void);					 //Servo二级菜单
void Get_Show_Servo(void);			 //Servo三级显示参数菜单
void Modify_Servo_Angle(void);   //Servo_Angle菜单
void text3(void);

uint8_t key_value_Get(void) ;    //获取按键值函数
void  Menu_key_set(void);        //处理按键值函数
#endif

