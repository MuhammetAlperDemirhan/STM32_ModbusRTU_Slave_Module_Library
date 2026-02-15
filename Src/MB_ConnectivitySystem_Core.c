/*
 * MB_ConnectivitySystem_Core.c
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#include "MB_ConnectivitySystem_Core.h"
#include "MB_FlashMemory_Core.h"
#include "MB_USER_REGISTERS_MAP_BOX.h"
#include "MB_FunctionCodes_Core.h"
#include "MB_System_Core.h"
#include "MB_STM32_MCUsPortfolio_Core.h"

void Connectivity_Update(void) // KULLANICI TARAFINDAN YÜKLENEN HABERLEŞME AYARLARINI FLASH HAFIZAYA KAYDEDER.
{
	MB_SlaveID = (uint8_t)MB_HoldingRegisters[0];
	MB_BaudRate = ((uint32_t)(MB_HoldingRegisters[2] << 16) | MB_HoldingRegisters[1]);
	MB_ParityBit = (uint32_t)MB_HoldingRegisters[3];

	Flash_Write(MB_SlaveID, MB_BaudRate, MB_ParityBit);
}


void Connectivity_Default_Update(GPIO_TypeDef* RstBtn_GPIOx,uint16_t RstBtn_GPIO_PIN,GPIO_TypeDef* Led_GPIOx,uint16_t Led_GPIO_PIN) //CIHAZ BAŞALTILIRKEN FACTORY BUTONU ILE FABRİKA AYARLARINI YÜKLER.
{
	if(HAL_GPIO_ReadPin(RstBtn_GPIOx, RstBtn_GPIO_PIN) == GPIO_PIN_SET)
	{
		HAL_Delay(2000);

		if(HAL_GPIO_ReadPin(RstBtn_GPIOx, RstBtn_GPIO_PIN) == GPIO_PIN_SET)
		{
			for(int i = 0; i < 10; i++)
			{
			   HAL_GPIO_TogglePin(Led_GPIOx, Led_GPIO_PIN);
			   HAL_Delay(100);

			   if(HAL_GPIO_ReadPin(RstBtn_GPIOx, RstBtn_GPIO_PIN) != GPIO_PIN_SET)
			   {
			       HAL_GPIO_WritePin(Led_GPIOx, Led_GPIO_PIN, GPIO_PIN_RESET);
			       return;
			   }
			}

			HAL_GPIO_WritePin(Led_GPIOx, Led_GPIO_PIN, GPIO_PIN_SET);

			Flash_Default_Write();

			HAL_GPIO_WritePin(Led_GPIOx, Led_GPIO_PIN, GPIO_PIN_RESET);
			return;
		}
	}
}


void Connectivity_Init_Update(UART_HandleTypeDef* huart , uint8_t* ID , uint32_t* Baud , uint32_t* Parity)//CIHAZ BAŞALTILIRKEN HABERLEŞME AYARLARINI GÜNCELLER.
{
	uint32_t deneme = 0 ;
	deneme = Flash_Read(MB_FlashStart_Address);
	if(deneme == (0xFFFFFFFF))
	{
		Flash_Default_Write();
	}

#if defined (MB_Word)
	*ID = (uint8_t)Flash_Read(MB_FlashStart_Address);
	*Baud = Flash_Read(MB_FlashStart_Address +4);
	*Parity = Flash_Read(MB_FlashStart_Address +8);

#elif defined (MB_DoubleWord)
	*ID = (uint8_t)Flash_Read(MB_FlashStart_Address);
	*Baud = (uint32_t)Flash_Read(MB_FlashStart_Address +8);
	*Parity = (uint32_t)Flash_Read(MB_FlashStart_Address +16);
#endif

	MB_HoldingRegisters[0] = (uint16_t)(*ID);
	MB_HoldingRegisters[1] = (uint16_t)(*Baud & 0xFFFF);
	MB_HoldingRegisters[2] = (uint16_t)(*Baud >> 16);
	MB_HoldingRegisters[3] = (uint16_t)(*Parity);

	if( HAL_UART_DeInit(huart) != HAL_OK)
	{
		Error_Handler();
	}


	huart -> Init.BaudRate = *Baud;
	huart -> Init.StopBits = UART_STOPBITS_1;
	huart -> Init.Mode = UART_MODE_TX_RX;
	huart -> Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart -> Init.OverSampling = UART_OVERSAMPLING_16;

	if(*Parity == Parity_EVEN)
	{
		huart -> Init.Parity = UART_PARITY_EVEN;
		huart -> Init.WordLength = UART_WORDLENGTH_9B;
	}

	else if (*Parity == Parity_ODD)
	{
		huart -> Init.Parity = UART_PARITY_ODD;
		huart -> Init.WordLength = UART_WORDLENGTH_9B;
	}

	else
	{
		huart -> Init.Parity = UART_PARITY_NONE;
		huart -> Init.WordLength = UART_WORDLENGTH_8B;
	}

	if (HAL_UART_Init(huart) != HAL_OK)
	{
	  Error_Handler();
	}

}


