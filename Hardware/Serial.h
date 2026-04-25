#ifndef __SERIAL_H
#define __SERIAL_H
//头文件声明
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
//定义
#define UART1_Rx_Get  1
#define UART2_Rx_Get  2

extern uint8_t Serial_TxPacket_1[10];  //串口1发送HEX数据流变量
extern uint8_t Serial_RxPacket_1[10];	//串口1接收HEX数据流变量
extern uint8_t Serial_TxPacket_2[10];	//串口2发送HEX数据流变量
extern uint8_t Serial_RxPacket_2[10];	//串口2接收HEX数据流变量

//函数声明
void Serial1_Init(uint32_t bound);      //uart1初始化
void Serial2_Init(uint32_t bound);        //uart2初始化
void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte);  //发送字节
void Serial_SendArray(USART_TypeDef* USARTx, uint8_t *Array, uint16_t Length);  //发送数组
void Serial_SendString(USART_TypeDef* USARTx, char *String);     //发送字符串
void Serial_SendNumber(USART_TypeDef* USARTx, uint32_t Number, uint8_t Length);   //发送无符号数字
void Serial_Printf(USART_TypeDef* USARTx, char *format, ...); //打印数据到串口函数
uint8_t Serial_GetRxFlag(USART_TypeDef* USARTx);  //串口接收查询函数
uint8_t Serial_GetRxData(USART_TypeDef* USARTx);  //串口接收函数
void Serial_SendPacket(USART_TypeDef* USARTx, uint16_t Length);   //串口发送HEX格式数据包
#endif
