/*
 * MB_System_Core.c
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#include "MB_System_Core.h"
#include "MB_FunctionCodes_Core.h"
#include "MB_ConnectivitySystem_Core.h"

uint8_t MB_RxBuffer[RxBuffer_Size];
uint8_t MB_TxBuffer[RxBuffer_Size];
uint16_t MB_RxSize = 0;
uint8_t MB_SlaveID = 1;
uint8_t MB_DataReady_Flag ;
UART_HandleTypeDef* MB_huart ;
uint32_t MB_BaudRate ;
uint32_t MB_ParityBit ;
GPIO_TypeDef* MB_RS485_ControlPort;
uint16_t MB_RS485_Control_Pin;
uint8_t MB_RS485_Control_Mode;
uint8_t MB_FactorySlaveID;
uint32_t MB_FactoryBaudRate;
uint32_t MB_FactoryParityBit;
uint32_t MB_FlashStart_Address;


void ModbusRTU_Start_Receive(void)
{
	MB_DataReady_Flag = 0;

#if defined (STM32F4) || defined (STM32F1) || defined (STM32L1) || defined (STM32F2)
		__HAL_UART_CLEAR_IDLEFLAG(MB_huart);
#else
		__HAL_UART_CLEAR_FLAG(MB_huart, UART_CLEAR_IDLEF);
#endif

	__HAL_UART_ENABLE_IT(MB_huart , UART_IT_IDLE);
	HAL_UART_Receive_DMA(MB_huart, MB_RxBuffer, RxBuffer_Size);

}

void ModbusRTU_Start_Transmit(UART_HandleTypeDef* huart,uint8_t* TX_Buffer,uint16_t Buffer_Size,uint8_t RS485_Mode,GPIO_TypeDef* RS485_GPIOx,uint16_t RS485_GPIO_PIN_x )
{
#if defined (STM32F4) || defined (STM32F1) || defined (STM32L1) || defined (STM32F2)
		__HAL_UART_CLEAR_IDLEFLAG(MB_huart);
#else
		__HAL_UART_CLEAR_FLAG(MB_huart, UART_CLEAR_IDLEF);
#endif
	if(Buffer_Size < 4)
	{
		//Hata
		return ;
	}

	if(RS485_Mode == 1)
	{
		HAL_GPIO_WritePin(RS485_GPIOx, RS485_GPIO_PIN_x, GPIO_PIN_SET);
	}

	HAL_UART_Transmit_DMA(huart, TX_Buffer, Buffer_Size);
}


void ModbusRTU_Receive_InWrite_USARTx_IRQHandler(UART_HandleTypeDef* huart)
{

	if(__HAL_UART_GET_FLAG(huart , UART_FLAG_IDLE))
	{
#if defined (STM32F4) || defined (STM32F1) || defined (STM32L1) || defined (STM32F2)
		__HAL_UART_CLEAR_IDLEFLAG(huart);
#else
		__HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_IDLEF);
#endif
		HAL_UART_DMAStop(huart);
		MB_RxSize = RxBuffer_Size - __HAL_DMA_GET_COUNTER(huart -> hdmarx) ;
		MB_DataReady_Flag = 1 ;
	}
}

void ModbusRTU_Transmit_InWrite_USARTx_IRQHandler(UART_HandleTypeDef* huart )
{
	if(__HAL_UART_GET_FLAG(huart , UART_FLAG_TC))
	{
		if(MB_RS485_Control_Mode == 1)
		{
			HAL_GPIO_WritePin(MB_RS485_ControlPort, MB_RS485_Control_Pin, GPIO_PIN_RESET);
		}

		HAL_UART_Receive_DMA(huart, MB_RxBuffer, RxBuffer_Size);

		if(Connectivity_Setting_Flag == 1)
		{
			Connectivity_Setting_Flag = 0 ;

			Connectivity_Update();
		}

	}
}

uint8_t ModbusRTU_Check(void)
{
	uint8_t kontrol = 0 ;
	if(MB_RxBuffer[0] == MB_SlaveID || MB_RxBuffer[0] == 0 )
	{
		if(MB_RxSize >= 4)
		{
			uint16_t hesaplanan_CRC = CRC16_Hesapla(MB_RxBuffer, MB_RxSize-2);
			uint16_t gelen_CRC = ((MB_RxBuffer[MB_RxSize-1]<<8) | (MB_RxBuffer[MB_RxSize-2]));
			if(hesaplanan_CRC == gelen_CRC)
			{
				kontrol = 1;
			}
		}
	}
	return kontrol ;
}


uint16_t CRC16_Hesapla(uint8_t *buffer , uint16_t uzunluk)
{
	uint16_t crc = 0xFFFF;
	for(int i = 0 ; i < uzunluk ; i++)
	{
		crc ^= buffer[i];

		for(int j = 0 ; j < 8 ; j++)
		{
			if((crc & 0x0001) == 1)
			{
				crc>>= 1;
				crc^= 0xA001;
			}

			else
			{
				crc>>= 1;
			}
		}
	}
	return crc ;
}

uint16_t Byte_Birlestir(uint8_t Hi_Byte , uint8_t Lo_Byte)
{
	return (uint16_t)((Hi_Byte<<8) | Lo_Byte) ;
}


