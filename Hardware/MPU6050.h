#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "math.h"
#include "stdbool.h"



#define M_PI 									(3.14159265358979323846)
//#define MPU6050_ACC_SAMPLE         (0x10)
//#define MPU6050_GYR_SAMPLE         (0x18)

typedef struct {
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
} MPU6050_Data;  //加速度和陀螺仪数据的结构体



void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);//向 MPU6050 寄存器写入数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress);//从 MPU6050 寄存器读取数据
void MPU6050_Init(void);                      //MPU6050初始化
uint8_t MPU6050_GetID(void);               //获取MPU6050的设备标识
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,  //获取MPU6050的陀螺仪数据
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
//void MPU6050_GetDataStruct(MPU6050_Data *data);//

void CalculateRollPitch(float *roll, float *pitch);//计算滚动和俯仰角
//bool AnalyzeStep(int16_t accX, int16_t accY, int16_t accZ);//
//float MPU6050_acc_transition(int16_t acc_value);//将MPU6050加速度计数据转换为实际物理数据
//float MPU6050_gyro_transition(int16_t gyro_value);//将MPU6050陀螺仪数据转换为实际物理数据

#endif

