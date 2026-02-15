/*
 * MB_FlashMemory_Core.h
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 */

#ifndef INC_MB_FLASHMEMORY_CORE_H_
#define INC_MB_FLASHMEMORY_CORE_H_

#include "main.h"


#define StartAddress 0x08040000
#define Parity_EVEN 0x01
#define Parity_ODD 0x02
#define Parity_NONE 0x03

uint32_t Flash_Write( uint8_t slaveID , uint32_t BaudRate , uint32_t ParityBit);
uint32_t Flash_Read(uint32_t addressR);
uint32_t Flash_Default_Write();
void PAGE_BANK_HESAPLA(uint32_t Address , uint32_t *PAGE , uint32_t *BANK);
void SECTOR_HESAPLA(uint32_t Address , uint32_t *SECTOR , uint32_t *BANK);

#endif /* INC_MB_FLASHMEMORY_CORE_H_ */
