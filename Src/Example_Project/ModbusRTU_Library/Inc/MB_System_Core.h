/*
 * MB_System_Core.h
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef INC_MB_SYSTEM_CORE_H_
#define INC_MB_SYSTEM_CORE_H_

#include "main.h"

#define RxBuffer_Size 256

extern uint8_t  MB_RxBuffer[RxBuffer_Size];
extern uint16_t MB_RxSize;
extern uint8_t  MB_TxBuffer[RxBuffer_Size];
extern uint8_t  MB_SlaveID;
extern uint8_t  MB_DataReady_Flag ;
extern UART_HandleTypeDef* MB_huart;
extern uint32_t MB_BaudRate ;
extern uint32_t MB_ParityBit ;
extern GPIO_TypeDef* MB_RS485_ControlPort;
extern uint16_t MB_RS485_Control_Pin;
extern uint8_t MB_RS485_Control_Mode;
extern uint8_t MB_FactorySlaveID;
extern uint32_t MB_FactoryBaudRate;
extern uint32_t MB_FactoryParityBit;
extern uint32_t MB_FlashStart_Address;


void ModbusRTU_Start_Receive(void);
void ModbusRTU_Receive_InWrite_USARTx_IRQHandler(UART_HandleTypeDef* huart);
uint8_t ModbusRTU_Check(void);
uint16_t CRC16_Hesapla(uint8_t *buffer , uint16_t uzunluk);
uint16_t Byte_Birlestir(uint8_t Hi_Byte , uint8_t Lo_Byte);
uint8_t Byte_Parcala(uint16_t Byte);
void ModbusRTU_Start_Transmit(UART_HandleTypeDef* huart,uint8_t* TX_Buffer,uint16_t Buffer_Size,uint8_t RS485_Mode,GPIO_TypeDef* RS485_GPIOx,uint16_t RS485_GPIO_PIN_x );
void ModbusRTU_Receive_InWrite_USARTx_IRQHandler(UART_HandleTypeDef* huart);
void ModbusRTU_Transmit_InWrite_USARTx_IRQHandler(UART_HandleTypeDef* huart );

#endif /* INC_MB_SYSTEM_CORE_H_ */
