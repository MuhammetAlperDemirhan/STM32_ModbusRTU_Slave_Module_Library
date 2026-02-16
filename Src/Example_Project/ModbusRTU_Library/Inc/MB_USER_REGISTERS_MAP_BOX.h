/*
 * MB_USER_REGISTERS_MAP_BOX.h
 *
 *  Created on: Feb 13, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef MODBUSRTU_LIBRARY_INC_MB_USER_REGISTERS_MAP_BOX_H_
#define MODBUSRTU_LIBRARY_INC_MB_USER_REGISTERS_MAP_BOX_H_

#include "main.h"


extern const uint16_t CoilStatus_Size ;
extern const uint16_t InputStatus_Size ;
extern const uint16_t InputRegisters_Size ;
extern const uint16_t HoldingRegisters_Size;

extern uint8_t MB_Coils[]; // 0xxxx , R/W , 1-bit
extern uint8_t MB_DiscrateInput[]; // 1xxxx , R , 1-bit
extern uint16_t MB_InputRegisters[]; //3xxxx , R , 16-bit/word/2-byte
extern uint16_t MB_HoldingRegisters[]; //4xxxx , R/W , 16-bit/word/2-byte

typedef struct
{
	GPIO_TypeDef* Port ;
	uint16_t Pin ;
}Coil_Degisken;

typedef struct
{
	GPIO_TypeDef* Port ;
	uint16_t Pin ;
}Discrete_Input_Degisken;

extern const Coil_Degisken Coil_Map[];
extern const Discrete_Input_Degisken Discrete_Input_Map[];
extern const uint16_t Coil_Map_Size;
extern const uint16_t Discrete_Input_Size;


#endif /* MODBUSRTU_LIBRARY_INC_MB_USER_REGISTERS_MAP_BOX_H_ */
