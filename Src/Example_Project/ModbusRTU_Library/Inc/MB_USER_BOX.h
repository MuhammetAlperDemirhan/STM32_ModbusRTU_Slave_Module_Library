/*
 * MB_USER_BOX.h
 *
 *  Created on: Feb 3, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef INC_MB_USER_BOX_H_
#define INC_MB_USER_BOX_H_

#include "main.h"
#include "MB_USER_REGISTERS_MAP_BOX.h"


void MODBUSRTU_InWrite_USARTx_IRQHandler(UART_HandleTypeDef *huart);
void MODBUSRTU_Write_While(void);
void MODBUSRTU_Start(UART_HandleTypeDef *huart , uint8_t* SLAVE_ID , uint32_t* BAUD_RATE , uint32_t* PARİTY,
		             uint8_t RS485_MODE_1_OR_0, GPIO_TypeDef* RS485_PORT_GPIOx,uint16_t RS485_GPIO_PIN_X,
					 GPIO_TypeDef* RstBtn_GPIOx,uint16_t RstBtn_GPIO_PIN_x,GPIO_TypeDef* Led_GPIOx,uint16_t Led_GPIO_PIN_x,
					 uint8_t Factory_SlaveID , uint32_t Factory_Baud , uint32_t Factory_Parity_1E_2O_3N , uint32_t Flash_Address);

#endif /* INC_MB_USER_BOX_H_ */
