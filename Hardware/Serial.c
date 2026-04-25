#include "Serial.h"
/*
uart1    A9  ----> uart1 TX
				 A10 ----> uart1 Rx
				 
uart2    A2  ----> uart2 TX
				 A3  ----> uart2 Rx
*/

//变量声明
uint8_t Serial_RxData_1;				//串口1接收变量
uint8_t Serial_RxData_2;				//串口2接收变量
uint8_t Serial_RxFlag_1;      	//串口1接收完成标志位
uint8_t Serial_RxFlag_2;				//串口2接收完成标志位
uint8_t Serial_TxPacket_1[10];  //串口1发送HEX数据流变量
uint8_t Serial_RxPacket_1[10];	//串口1接收HEX数据流变量
uint8_t Serial_TxPacket_2[10];	//串口2发送HEX数据流变量
uint8_t Serial_RxPacket_2[10];	//串口2接收HEX数据流变量


//初始化
void Serial1_Init(uint32_t bound)      //uart1初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial2_Init(uint32_t bound)        //uart2初始化
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}







//发送与接收函数
void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte)       //发送字节
{
	USART_SendData(USARTx, Byte);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(USART_TypeDef* USARTx, uint8_t *Array, uint16_t Length)  //发送数组
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx, Array[i]);
	}
}

void Serial_SendString(USART_TypeDef* USARTx, char *String)     //发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(USARTx, String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)  //内部调用
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(USART_TypeDef* USARTx, uint32_t Number, uint8_t Length)   //发送无符号数字
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx , Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(USART1, ch);
	return ch;
}

void Serial_Printf(USART_TypeDef* USARTx, char *format, ...) //打印数据到串口函数
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(USARTx, String);
}

void Serial_SendPacket(USART_TypeDef* USARTx, uint16_t Length)           //串口发送HEX格式数据包
	{		//0xFF ----> 帧头     0xFE ----> 帧尾   此处使用连续发送两帧的帧头或帧尾的模式
	if(USARTx == USART1)     //串口1发送HEX数据包
	{
		Serial_SendByte(USART1, 0xFF);     //数据包数据数量需要算上帧头 帧尾
		Serial_SendByte(USART1, 0xFF);
		Serial_SendArray(USART1, Serial_TxPacket_1, Length);//Length ----> 有效数据长度
		Serial_SendByte(USART1, 0xFE);
		Serial_SendByte(USART1, 0xFE);
	}
	else if(USARTx == USART2)   //串口2发送HEX数据包
	{
		Serial_SendByte(USART2, 0xFF);
		Serial_SendByte(USART2, 0xFF);
		Serial_SendArray(USART2, Serial_TxPacket_2, Length);
		Serial_SendByte(USART2, 0xFE);
		Serial_SendByte(USART2, 0xFE);
	}
}


uint8_t Serial_GetRxFlag(USART_TypeDef* USARTx)  //串口接收查询函数
{
	if ((Serial_RxFlag_1 == 1) && (USARTx == USART1)) //查询串口1
	{
		Serial_RxFlag_1 = 0;
		return UART1_Rx_Get;
	}
	else if((Serial_RxFlag_2 == 1) && (USARTx == USART2)) //查询串口2
	{
		Serial_RxFlag_2 = 0;
		return UART2_Rx_Get;
	}
	return 0;
}

uint8_t Serial_GetRxData(USART_TypeDef* USARTx)  //串口接收函数
{
	if (USARTx == USART1)   //串口1数据接收
	{
		return Serial_RxData_1;
	}
	else if(USARTx == USART2) //串口2数据接收
	{
		return Serial_RxData_2;
	}
	return 0;
}

//void USART1_IRQHandler(void)          //串口1中断函数
//{																			//接收数据为单字节时
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		Serial_RxData_1 = USART_ReceiveData(USART1); 
//		Serial_RxFlag_1 = 1;
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}

//void USART1_IRQHandler(void)					//串口1中断函数
//{																			//接收数据为HEX数据包时
//	static uint8_t RxState1 = 0;         //接收状态
//	static uint8_t pRxPacket1 = 0;       //有效数据的“数量”
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData1 = USART_ReceiveData(USART1);
//		
//		if (RxState1 == 0)
//		{
//			if (RxData1 == 0xFF)     //第一次收到帧头
//			{
//				RxState1 = 1;					//RxState = 1 表示处于第一次接收帧头完毕的状态
//			}
//		}
//		else if (RxState1 == 1)    //第二次收到同一个帧头，确定是接收收据，即匹配成功
//		{													//帧头也可以是只有一位的，只不过我这里是用了两位帧头匹配
//			if (RxData1 == 0xFF)
//			{
//				RxState1 = 2;					//RxState = 2 表示处于第二次接收帧头完毕的状态
//				pRxPacket1 = 0;				//也是有效数据接收即将开始的标志
//			}
//		}
//		else if (RxState1 == 2)    //RxState = 2 开始接收有效数据
//		{
//			Serial_RxPacket_1[pRxPacket1] = RxData1;	//串行接收有效数据
//			if((Serial_RxPacket_1[pRxPacket1] == 0xFE) && (pRxPacket1 >0))  //判断帧尾
//			{                                         //注意接收的数据的长度，超过接收的数组长度可能会产生意外的现象
//				if(Serial_RxPacket_1[pRxPacket1-1] == 0xFE)
//				{
//					RxState1 =0;         //接收结束
//					Serial_RxFlag_1 = 1;//接收完成标志置1
//				}
//			}
//			pRxPacket1 ++;                    
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}

void USART2_IRQHandler(void)          //串口2中断函数
{																			//接收数据为单字节时
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		Serial_RxData_2 = USART_ReceiveData(USART2); 
		Serial_RxFlag_2 = 1;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

//void USART2_IRQHandler(void)					//串口2中断函数
//{																			//接收数据为HEX数据包时
//	static uint8_t RxState2 = 0;         //接收状态
//	static uint8_t pRxPacket2 = 0;       //有效数据的“数量”
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData2 = USART_ReceiveData(USART2);
//		
//		if (RxState2 == 0)
//		{
//			if (RxData2 == 0xFF)     //第一次收到帧头
//			{
//				RxState2 = 1;					//RxState = 1 表示处于第一次接收帧头完毕的状态
//			}
//		}
//		else if (RxState2 == 1)    //第二次收到同一个帧头，确定是接收收据，即匹配成功
//		{													//帧头也可以是只有一位的，只不过我这里是用了两位帧头匹配
//			if (RxData2 == 0xFF)
//			{
//				RxState2 = 2;					//RxState = 2 表示处于第二次接收帧头完毕的状态
//				pRxPacket2 = 0;				//也是有效数据接收即将开始的标志
//			}
//		}
//		else if (RxState2 == 2)    //RxState = 2 开始接收数据
//		{
//			Serial_RxPacket_2[pRxPacket2] = RxData2;	//串行接收有效数据
//			if((Serial_RxPacket_2[pRxPacket2] == 0xFE) && (pRxPacket2 >0))  //判断帧尾
//			{
//				if(Serial_RxPacket_2[pRxPacket2-1] == 0xFE)
//				{
//					RxState2 =0;         //接收结束
//					Serial_RxFlag_2 = 1;//接收完成标志置1
//				}
//			}
//			pRxPacket2 ++;                    
//		}
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//	}
//}



