#include "MPU6050.h"

#define MPU6050_ADDRESS		0xD0   //定义 MPU6050 设备的 I2C 地址

/*
函数作用：向 MPU6050 寄存器写入数据
函数入口参数：
		参数 RegAddress：要写入数据的寄存器地址
		参数 Data：要写入的数据
*/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

/*
函数作用：从 MPU6050 寄存器读取数据
函数入口参数：参数 RegAddress：要读取数据的寄存器地址
函数返回值：从寄存器读取到的数据
*/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

/*
函数作用：初始化 MPU6050
*/
void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

/*
函数作用：获取 MPU6050的标识
函数返回值：MPU6050的设备标识
*/
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/*
函数作用：读取 MPU6050 的加速度和陀螺仪数据
函数入口参数：
	参数 AccX、AccY、AccZ：指向存储加速度数据的变量的指针
	参数 GyroX、GyroY、GyroZ：指向存储陀螺仪数据的变量的指针
	形参类型为指针
*/
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

//使用结构体作为入口参数,传入结构体的地址
//void MPU6050_GetDataStruct(MPU6050_Data *data)
//{
//    uint8_t DataH, DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
//    data->AccX = (DataH << 8) | DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
//    data->AccY = (DataH << 8) | DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
//    data->AccZ = (DataH << 8) | DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
//    data->GyroX = (DataH << 8) | DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
//    data->GyroY = (DataH << 8) | DataL;
//    
//    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
//    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
//    data->GyroZ = (DataH << 8) | DataL;
//}

/*
函数作用：计算滚动和俯仰角
函数入口参数： 
函数结果（返回值）：°（度）
*/
void CalculateRollPitch( float *roll, float *pitch) 
{
    // 1. 读取传感器数据
    int16_t accX, accY, accZ;
    int16_t gyroX, gyroY, gyroZ;
    MPU6050_GetData(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    
    // 2. 计算滚动和俯仰角
    *roll = atan2(accY, accZ) * (180.0 / M_PI);
    *pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * (180.0 / M_PI);
    
}

/*
函数作用：分析步态
*/
//bool AnalyzeStep(int16_t accX, int16_t accY, int16_t accZ) 
//{
//    // 计算加速度向量的大小
//    float magnitude = sqrt(accX * accX + accY * accY + accZ * accZ);

//    // 设置步态检测的阈值
//    const float stepThreshold = 1.2;

//    if (magnitude > stepThreshold) {
//        return true;  // 检测到一步
//    } else {
//        return false; // 未检测到步态
//    }
//}
/*
关于float MPU6050_acc_transition(int16_t acc_value)和
		float MPU6050_gyro_transition(int16_t gyro_value)
的参数MPU6050_ACC_SAMPLE和MPU6050_GYR_SAMPLE的说明：
这两个数是用来设置加速度计量程的，它决定了加速度计所能测量的最大加速度范围
这两个的参数的选择跟你的需求有关
选择的加速度变化范围越大，首先，量程越大（防止数据失真），
但精度也会越低
*/
// 函数简介：将MPU6050加速度计数据转换为实际物理数据
// 参数说明：acc_value - 任意轴的加速度计数据
// 返回参数：返回经过转换的加速度物理值（单位为g，即m/s2）
//float MPU6050_acc_transition(int16_t acc_value)
//{
//    float acc_data = 0;
//    switch (MPU6050_ACC_SAMPLE) 
//		{
//        case 0x00:
//            acc_data = (float)acc_value / 16384;  // 加速度计量程为±2g，数据除以16384，转化为带单位的数据
//            break;
//        case 0x08:
//            acc_data = (float)acc_value / 8192;   // 加速度计量程为±4g，数据除以8192，转化为带单位的数据
//            break;
//        case 0x10:
//            acc_data = (float)acc_value / 4096;   // 加速度计量程为±8g，数据除以4096，转化为带单位的数据
//            break;
//        case 0x18:
//            acc_data = (float)acc_value / 2048;   // 加速度计量程为±16g，数据除以2048，转化为带单位的数据
//            break;
//        default:
//            break;
//    }
//    return acc_data;
//}

// 函数简介：将MPU6050陀螺仪数据转换为实际物理数据
// 参数说明：gyro_value - 任意轴的陀螺仪数据
// 返回参数：返回经过转换的陀螺仪物理值（单位为°/s）
//float MPU6050_gyro_transition(int16_t gyro_value) 
//{
//    float gyro_data = 0;
//    switch (MPU6050_GYR_SAMPLE) 
//		{
//        case 0x00:
//            gyro_data = (float)gyro_value / 131.0f;  // 陀螺仪量程为±250 dps，数据除以131，转化为带单位的数据
//            break;
//        case 0x08:
//            gyro_data = (float)gyro_value / 65.5f;   // 陀螺仪量程为±500 dps，数据除以65.5，转化为带单位的数据
//            break;
//        case 0x10:
//            gyro_data = (float)gyro_value / 32.8f;   // 陀螺仪量程为±1000 dps，数据除以32.8，转化为带单位的数据
//            break;
//        case 0x18:
//            gyro_data = (float)gyro_value / 16.4f;   // 陀螺仪量程为±2000 dps，数据除以16.4，转化为带单位的数据
//            break;
//        default:
//            break;
//    }
//    return gyro_data;
//}



