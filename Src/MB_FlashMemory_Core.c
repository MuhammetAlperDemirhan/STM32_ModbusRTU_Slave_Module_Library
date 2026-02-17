/*
 * MB_FlashMemory_Core.c
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

//FLASH_EraseInitTypeDef TANIMLANDIĞI DOSYAYA GİDEREK  WORD YADA DOUBLEWORD BİLGİSİ VE VOLTAGE RANGE VARMI YOKMU BİLGİSİNİ KONTOL ET LİSTELE.

#include "main.h"
#include "MB_FlashMemory_Core.h"
#include "MB_System_Core.h"
#include "MB_STM32_MCUsPortfolio_Core.h"


uint32_t Flash_Write( uint8_t slaveID , uint32_t BaudRate , uint32_t ParityBit) // *******************************************************************************************************************
{
	uint32_t addressW = MB_FlashStart_Address ;
	uint32_t data[3]={slaveID , BaudRate , ParityBit};



	FLASH_EraseInitTypeDef SettingsFlash = {0} ;
	uint32_t ErrorCODE =0 ;
	uint32_t BANK_NO ;
	uint32_t PAGE_NO;
	uint32_t SECTOR_NO;
	uint32_t SECTOR_BANK_NO;

	PAGE_BANK_HESAPLA(MB_FlashStart_Address, &PAGE_NO, &BANK_NO) ;
	SECTOR_HESAPLA(MB_FlashStart_Address, &SECTOR_NO, &SECTOR_BANK_NO);

	HAL_FLASH_Unlock();

#if defined (MB_Sector)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_SECTORS ;
	SettingsFlash.Sector = SECTOR_NO ;
	SettingsFlash.NbSectors = 1 ;
	SettingsFlash.VoltageRange = FLASH_VOLTAGE_RANGE_3 ;
#if defined (FLASH_BANK_1)
	SettingsFlash.Banks = SECTOR_BANK_NO ;
#endif

#elif defined (MB_Page) && defined (Page_ADDRESS)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.PageAddress = MB_FlashStart_Address ;
	SettingsFlash.NbPages = 2 ;
#elif defined (MB_Page) && defined (Page_NO) && defined (BANK_YOK)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.Page = PAGE_NO ;
	SettingsFlash.NbPages = 2 ;
#elif defined (MB_Page) && defined (Page_NO) && defined (BANK_VAR)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.Page = PAGE_NO ;
	SettingsFlash.NbPages = 2 ;
	SettingsFlash.Banks = BANK_NO ;
#endif

	HAL_FLASHEx_Erase(&SettingsFlash, &ErrorCODE);


#if defined (MB_Word)

	for(uint8_t i = 0 ; i<3 ; i++)
	{
		if((HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addressW , data[i])) == HAL_OK)
		{
			addressW += 4;
		}
		else
		{
			return HAL_FLASH_GetError();
		}
	}
#elif defined (MB_DoubleWord)

	for(uint8_t i = 0 ; i<3 ; i++)
	{
		if((HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD , addressW , (uint64_t)data[i])) == HAL_OK)
		{
			addressW += 8;
		}
		else
		{
			return HAL_FLASH_GetError();
		}
	}
#endif

	HAL_FLASH_Lock();

	return 0 ;
}


uint32_t Flash_Read(uint32_t addressR) //*************************************************************************************************************************************************************
{
#if defined (MB_Word)
	return *(__IO uint32_t*)addressR ;
#elif defined (MB_DoubleWord)
	return *(__IO uint64_t*)addressR ;
#endif
}


uint32_t Flash_Default_Write() //*********************************************************************************************************************************************************************
{
	uint32_t addressWDef = MB_FlashStart_Address ;
	uint32_t dataDefault[3]={(uint8_t)MB_FactorySlaveID , MB_FactoryBaudRate , MB_FactoryParityBit};

	uint32_t PAGE_BANK_NO ;
	uint32_t PAGE_NO;
	uint32_t SECTOR_NO;
	uint32_t SECTOR_BANK_NO;

	PAGE_BANK_HESAPLA(MB_FlashStart_Address, &PAGE_NO, &PAGE_BANK_NO) ;
	SECTOR_HESAPLA(MB_FlashStart_Address, &SECTOR_NO, &SECTOR_BANK_NO);

	FLASH_EraseInitTypeDef SettingsFlash = {0} ;
	uint32_t ErrorCODE =0 ;

	HAL_FLASH_Unlock();

#if defined (MB_Sector)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_SECTORS ;
	SettingsFlash.Sector = SECTOR_NO ;
	SettingsFlash.NbSectors = 1 ;
	SettingsFlash.VoltageRange = FLASH_VOLTAGE_RANGE_3 ;
#if defined (FLASH_BANK_1)
	SettingsFlash.Banks = SECTOR_BANK_NO ;
#endif

#elif defined (MB_Page) && defined (Page_ADDRESS)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.PageAddress = MB_FlashStart_Address;
	SettingsFlash.NbPages = 2 ;
#elif defined (MB_Page) && defined (Page_NO) && defined (BANK_YOK)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.Page = PAGE_NO ;
	SettingsFlash.NbPages = 2 ;
#elif defined (MB_Page) && defined (Page_NO) && defined (BANK_VAR)
	SettingsFlash.TypeErase = FLASH_TYPEERASE_PAGES ;
	SettingsFlash.Page = PAGE_NO ;
	SettingsFlash.NbPages = 2 ;
	SettingsFlash.Banks = PAGE_BANK_NO ;
#endif

	if(HAL_FLASHEx_Erase(&SettingsFlash, &ErrorCODE) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return HAL_FLASH_GetError() ;
	}

#if defined (MB_Word)

	for(uint8_t i = 0 ; i<3 ; i++)
	{
		if((HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD , addressWDef , dataDefault[i])) == HAL_OK)
		{
			addressWDef += 4;
		}
		else
		{
			return HAL_FLASH_GetError();
		}
	}
#elif defined (MB_DoubleWord)

	for(uint8_t i = 0 ; i<3 ; i++)
	{
		if((HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD , addressWDef , (uint64_t)dataDefault[i])) == HAL_OK)
		{
			addressWDef += 8;
		}
		else
		{
			return HAL_FLASH_GetError();
		}
	}
#endif

	HAL_FLASH_Lock();

	return 0 ;
}

void PAGE_BANK_HESAPLA(uint32_t Address , uint32_t *PAGE , uint32_t *BANK) //*************************************************************************************************************************
{
#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE 2048
#endif

#if defined (STM32G4) || defined (STM32L5) || defined (STM32L4)
	uint32_t Flash_Size = ((*((uint16_t*)FLASHSIZE_BASE)) * 1024);
	uint32_t Bank2_Baslangic = (FLASH_BASE + (Flash_Size /2));
	if(Address >= Bank2_Baslangic)
	{
		*BANK = FLASH_BANK_2 ;
		*PAGE = ((Address - Bank2_Baslangic) / FLASH_PAGE_SIZE );
	}
	else
	{
		*BANK = FLASH_BANK_1 ;
		*PAGE = ((Address - FLASH_BASE) / FLASH_PAGE_SIZE );
	}
#elif defined (STM32G0) || defined (STM32C0)
	*PAGE = ((Address - FLASH_BASE) / FLASH_PAGE_SIZE) ;
	*BANK = 0 ;
#endif
}

void SECTOR_HESAPLA(uint32_t Address , uint32_t *SECTOR , uint32_t *BANK) //**************************************************************************************************************************
{
	uint32_t Bank_Var = 0 ;
	uint32_t Bank_Yok = 0 ;

#if defined (FLASH_OPTCR_nDBANK)
	if((FLASH->OPTCR & FLASH_OPTCR_nDBANK) == 0)
	{
		Bank_Var = 1 ;
	}
#elif defined (FLASH_OPTCR_DB1M)
	if((FLASH->OPTCR & FLASH_OPTCR_DB1M) == 1)
	{
		Bank_Var = 1 ;
	}
#else
	Bank_Yok = 1 ;
#endif

	if(Bank_Yok)
	{
#if defined (STM32F2) || defined (STM32F4)
		*BANK = FLASH_BANK_1 ;

		if((Address >= 0x08000000) && (Address <= 0x08003FFF )){ *SECTOR = FLASH_SECTOR_0 ; }
		else if ((Address >= 0x08004000) && (Address <= 0x08007FFF )){ *SECTOR = FLASH_SECTOR_1 ; }
		else if ((Address >= 0x08008000) && (Address <= 0x0800BFFF )){ *SECTOR = FLASH_SECTOR_2 ; }
		else if ((Address >= 0x0800C000) && (Address <= 0x0800FFFF )){ *SECTOR = FLASH_SECTOR_3 ; }
		else if ((Address >= 0x08010000) && (Address <= 0x0801FFFF )){ *SECTOR = FLASH_SECTOR_4 ; }
		else if ((Address >= 0x08020000) && (Address <= 0x0803FFFF )){ *SECTOR = FLASH_SECTOR_5 ; }
#if defined (FLASH_SECTOR_6)
		else if ((Address >= 0x08040000) && (Address <= 0x0805FFFF )){ *SECTOR = FLASH_SECTOR_6 ; }
		else if ((Address >= 0x08060000) && (Address <= 0x0807FFFF )){ *SECTOR = FLASH_SECTOR_7 ; }
#endif
#if defined (FLASH_SECTOR_8)
		else if ((Address >= 0x08080000) && (Address <= 0x0809FFFF )){ *SECTOR = FLASH_SECTOR_8 ; }
		else if ((Address >= 0x080A0000) && (Address <= 0x080BFFFF )){ *SECTOR = FLASH_SECTOR_9 ; }
		else if ((Address >= 0x080C0000) && (Address <= 0x080DFFFF )){ *SECTOR = FLASH_SECTOR_10; }
		else if ((Address >= 0x080E0000) && (Address <= 0x080FFFFF )){ *SECTOR = FLASH_SECTOR_11; }
#endif
#endif

#if defined (STM32F7)
		if ((Address >= 0x08000000) && (Address <= 0x08007FFF )){ *SECTOR = FLASH_SECTOR_0 ; }
		else if ((Address >= 0x08008000) && (Address <= 0x0800FFFF )){ *SECTOR = FLASH_SECTOR_1 ; }
		else if ((Address >= 0x08010000) && (Address <= 0x08017FFF )){ *SECTOR = FLASH_SECTOR_2 ; }
		else if ((Address >= 0x08018000) && (Address <= 0x0801FFFF )){ *SECTOR = FLASH_SECTOR_3 ; }
		else if ((Address >= 0x08020000) && (Address <= 0x0803FFFF )){ *SECTOR = FLASH_SECTOR_4 ; }
		else if ((Address >= 0x08040000) && (Address <= 0x0807FFFF )){ *SECTOR = FLASH_SECTOR_5 ; }
#if defined (FLASH_SECTOR_6)
		else if ((Address >= 0x08080000) && (Address <= 0x080BFFFF )){ *SECTOR = FLASH_SECTOR_6 ; }
		else if ((Address >= 0x080C0000) && (Address <= 0x080FFFFF )){ *SECTOR = FLASH_SECTOR_7 ; }
#endif
#if defined (FLASH_SECTOR_8)
		else if ((Address >= 0x08100000) && (Address <= 0x0813FFFF )){ *SECTOR = FLASH_SECTOR_8 ; }
		else if ((Address >= 0x08140000) && (Address <= 0x0817FFFF )){ *SECTOR = FLASH_SECTOR_9 ; }
		else if ((Address >= 0x08180000) && (Address <= 0x081BFFFF )){ *SECTOR = FLASH_SECTOR_10; }
		else if ((Address >= 0x081C0000) && (Address <= 0x081FFFFF )){ *SECTOR = FLASH_SECTOR_11; }
#endif
#endif
	}

	else if(Bank_Var)
	{
#if defined (STM32F7) || defined (STM32F42xxx) || defined (STM32F43xxx)
#if defined (FLASH_BANK_2)
		if((Address >= 0x08100000) && (Address <= 0x081FFFFF)){ *BANK = FLASH_BANK_2 ; }
		else { *BANK = FLASH_BANK_1 ; }
#elif defined (FLASH_BANK_1)
		*BANK = FLASH_BANK_1 ;
#else
		*BANK = 0 ;
#endif

		if ((Address >= 0x08000000) && (Address <= 0x08003FFF)){ *SECTOR = FLASH_SECTOR_0 ; }
		else if ((Address >= 0x08004000) && (Address <= 0x08007FFF)){ *SECTOR = FLASH_SECTOR_1 ; }
		else if ((Address >= 0x08008000) && (Address <= 0x0800BFFF)){ *SECTOR = FLASH_SECTOR_2 ; }
		else if ((Address >= 0x0800C000) && (Address <= 0x0800FFFF)){ *SECTOR = FLASH_SECTOR_3 ; }
		else if ((Address >= 0x08010000) && (Address <= 0x0801FFFF)){ *SECTOR = FLASH_SECTOR_4 ; }
		else if ((Address >= 0x08020000) && (Address <= 0x0803FFFF)){ *SECTOR = FLASH_SECTOR_5 ; }
#if defined (FLASH_SECTOR_6)
		else if ((Address >= 0x08040000) && (Address <= 0x0805FFFF)){ *SECTOR = FLASH_SECTOR_6 ; }
		else if ((Address >= 0x08060000) && (Address <= 0x0807FFFF)){ *SECTOR = FLASH_SECTOR_7 ; }
#endif	
#if defined (FLASH_SECTOR_8)
		else if ((Address >= 0x08080000) && (Address <= 0x0809FFFF)){ *SECTOR = FLASH_SECTOR_8 ; }
		else if ((Address >= 0x080A0000) && (Address <= 0x080BFFFF)){ *SECTOR = FLASH_SECTOR_9 ; }
		else if ((Address >= 0x080C0000) && (Address <= 0x080DFFFF)){ *SECTOR = FLASH_SECTOR_10; }
		else if ((Address >= 0x080E0000) && (Address <= 0x080FFFFF)){ *SECTOR = FLASH_SECTOR_11; }
#endif
#if defined (FLASH_SECTOR_12)
		else if ((Address >= 0x08100000) && (Address <= 0x08103FFF)){ *SECTOR = FLASH_SECTOR_12; }
		else if ((Address >= 0x08104000) && (Address <= 0x08107FFF)){ *SECTOR = FLASH_SECTOR_13; }
		else if ((Address >= 0x08108000) && (Address <= 0x0810BFFF)){ *SECTOR = FLASH_SECTOR_14; }
		else if ((Address >= 0x0810C000) && (Address <= 0x0810FFFF)){ *SECTOR = FLASH_SECTOR_15; }
		else if ((Address >= 0x08110000) && (Address <= 0x0811FFFF)){ *SECTOR = FLASH_SECTOR_16; }
		else if ((Address >= 0x08120000) && (Address <= 0x0813FFFF)){ *SECTOR = FLASH_SECTOR_17; }
		else if ((Address >= 0x08140000) && (Address <= 0x0815FFFF)){ *SECTOR = FLASH_SECTOR_18; }
		else if ((Address >= 0x08160000) && (Address <= 0x0817FFFF)){ *SECTOR = FLASH_SECTOR_19; }
		else if ((Address >= 0x08180000) && (Address <= 0x0819FFFF)){ *SECTOR = FLASH_SECTOR_20; }
		else if ((Address >= 0x081A0000) && (Address <= 0x081BFFFF)){ *SECTOR = FLASH_SECTOR_21; }
		else if ((Address >= 0x081C0000) && (Address <= 0x081DFFFF)){ *SECTOR = FLASH_SECTOR_22; }
		else if ((Address >= 0x081E0000) && (Address <= 0x081FFFFF)){ *SECTOR = FLASH_SECTOR_23; }
#endif
#endif
	}
}


