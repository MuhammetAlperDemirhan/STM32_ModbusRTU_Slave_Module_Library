/*
 * MB_FunctionCodes_Core.h
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef INC_MB_FUNCTIONCODES_CORE_H_
#define INC_MB_FUNCTIONCODES_CORE_H_

#include "main.h"



#define Buffer 265
extern uint8_t Connectivity_Setting_Flag;

uint16_t Exception_Codes(uint8_t Slave_ID , uint8_t Function_Code , uint8_t Exception_Code , uint8_t* Cevap); //Exception Code masajı hazırlar.
uint16_t Cevap_Hazirla(uint8_t* Rx_Paket , uint8_t* Tx_Paket);   //TX Buffer Hazırlar.
uint16_t Read_Coils(uint8_t* Istek , uint8_t* Cevap);            //0x01
uint16_t Read_Discrete_Inputs(uint8_t* Istek , uint8_t* Cevap ); //0x02
uint16_t Read_Holding_Registers(uint8_t* Istek , uint8_t* Cevap);//0x03
uint16_t Read_Input_Registers(uint8_t* Istek ,uint8_t* Cevap) ;  //0x04
uint16_t Write_Single_Coil(uint8_t* Istek , uint8_t* Cevap);     //0x05
uint16_t Write_Single_Register(uint8_t* Istek , uint8_t* Cevap); //0x06
uint16_t Write_Multiple_Coils(uint8_t* Istek , uint8_t* Cevap);  //0x0F
uint16_t Write_Multiple_Registers(uint8_t* Istek , uint8_t* Cevap); //0x10

#endif /* INC_MB_FUNCTIONCODES_CORE_H_ */
