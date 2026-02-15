/*
 * MB_STM32_MCUsPortfolio_Core.h
 *
 *  Created on: Feb 13, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#ifndef MODBUSRTU_LIBRARY_INC_MB_STM32_MCUSPORTFOLIO_CORE_H_
#define MODBUSRTU_LIBRARY_INC_MB_STM32_MCUSPORTFOLIO_CORE_H_

/*=============================== HAFIZA DURUMU =============================== */
#if defined (STM32F1) || defined (STM32F0) || defined (STM32F3) || defined (STM32G0) || defined (STM32G4) || defined (STM32L0) ||\
    defined (STM32L4) || defined (STM32L5) || defined (STM32C0) || defined (STM32L1)
#define MB_Page 1

#elif defined (STM32F4) || defined (STM32F7) || defined (STM32F2) //VOLTAGE RANGE AYARI VAR. FLASH_VOLTAGE_RANGE_3 YAPILIRSA WORD YAZMA YAPILIR.
#define MB_Sector 1
#endif
/*=============================== YAZMA BİT DURUMU =============================== */

#if defined (STM32G0) || defined (STM32G4) || defined (STM32L4) || defined (STM32L5) || defined (STM32C0)
#define MB_DoubleWord 1

#elif defined (STM32F4) || defined (STM32F7) || defined (STM32F2) || defined (STM32F0) || defined (STM32F1) || defined (STM32F3) || defined (STM32L0) || defined (STM32L1)
#define MB_Word 1
#endif
/*=============================== PAGE DURUMU =============================== */

#if defined (STM32L1) || defined (STM32L0) || defined (STM32F1) || defined (STM32F0) || defined (STM32F3)
#define Page_ADDRESS 1

#elif defined (STM32G0) || defined (STM32G4) || defined (STM32L4) || defined (STM32L5) || defined (STM32C0)
#define Page_NO 1
#endif
/*=============================== BANK DURUMU ================================  */

#if defined (STM32G4) || defined (STM32L5) || defined (STM32L4)
#define BANK_VAR 1

#elif defined (STM32C0) || defined (STM32G0)
#define BANK_YOK 1
#endif


#endif /* MODBUSRTU_LIBRARY_INC_MB_STM32_MCUSPORTFOLIO_CORE_H_ */
