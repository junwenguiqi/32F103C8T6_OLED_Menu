#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_Init(void);//初始化软件模拟的I2C
void MyI2C_Start(void);//I2C的起始信号
void MyI2C_Stop(void);//I2C的停止信号
void MyI2C_SendByte(uint8_t Byte);//I2C发送一个字节数据
uint8_t MyI2C_ReceiveByte(void);//I2C接收一个字节数据
void MyI2C_SendAck(uint8_t AckBit);//I2C发送应答信号
uint8_t MyI2C_ReceiveAck(void);//I2C接收应答信号

#endif
