#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "menu.h"
#include "Motor.h"
#include "Encoder.h"
#include "control.h"
int main(void)
{
	
	Menu_Init();
	MiniBalance_PWM_Init(7199,0);
	Encoder_Init_TIM2();            //编码器接口
	Encoder_Init_TIM3();            //初始化编码器2
	while (1)
	{
		//Call_back_handler();
		int Encoder_Left,Encoder_Right;             					//左右编码器的脉冲计数
   int Balance_Pwm,Velocity_Pwm,Turn_Pwm;		  					//平衡环PWM变量，速度环PWM变量，转向环PWM变 
		Get_Angle(Way_Angle);                     					//更新姿态，5ms一次，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
		Encoder_Left=-Read_Encoder(2);            					//读取左轮编码器的值，前进为正，后退为负
		Encoder_Right=-Read_Encoder(3);           					//读取右轮编码器的值，前进为正，后退为负
																												//左轮A相接TIM2_CH1,右轮A相接TIM4_CH2,故这里两个编码器的极性相同
		Get_Velocity_Form_Encoder(Encoder_Left,Encoder_Right);//编码器读数转速度（mm/s）

		Balance_Pwm=Balance(Angle_Balance,Gyro_Balance);    //平衡PID控制 Gyro_Balance平衡角速度极性：前倾为正，后倾为负
		Velocity_Pwm=Velocity(Encoder_Left,Encoder_Right);  //速度环PID控制	记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
		Turn_Pwm=Turn(Gyro_Turn);														//转向环PID控制     
		
		Motor_Left=Balance_Pwm+Velocity_Pwm+Turn_Pwm;       //计算左轮电机最终PWM
		Motor_Right=Balance_Pwm+Velocity_Pwm-Turn_Pwm;      //计算右轮电机最终PWM
																												//PWM值正数使小车前进，负数使小车后退
		Motor_Left=PWM_Limit(Motor_Left,6900,-6900);
		Motor_Right=PWM_Limit(Motor_Right,6900,-6900);			//PWM限幅
		if(Turn_Off(Angle_Balance)==0)     					//如果不存在异常
			Set_Pwm(Motor_Left,Motor_Right);         					//赋值给PWM寄存器     
		Menu_key_set();
	}
}
