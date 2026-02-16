/*
 * MB_ConnectivitySystem_Core.h
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef INC_MB_CONNECTIVITYSYSTEM_CORE_H_
#define INC_MB_CONNECTIVITYSYSTEM_CORE_H_

#include "main.h"


void Connectivity_Update(void);
void Connectivity_Default_Update(GPIO_TypeDef* RstBtn_GPIOx,uint16_t RstBtn_GPIO_PIN,GPIO_TypeDef* Led_GPIOx,uint16_t Led_GPIO_PIN);
void Connectivity_Init_Update(UART_HandleTypeDef* huart , uint8_t* ID , uint32_t* Baud , uint32_t* Parity);

#endif /* INC_MB_CONNECTIVITYSYSTEM_CORE_H_ */
