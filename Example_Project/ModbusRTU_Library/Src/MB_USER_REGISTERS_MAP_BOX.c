/*
 * MB_USER_REGISTERS_MAP_BOX.c
 *
 *  Created on: Feb 13, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */



#include "MB_USER_REGISTERS_MAP_BOX.h"
#include "MB_System_Core.h"

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ REGİSTER VE COİL DİZİ BOYUTLARINI AYARLAYABİLİRSİNİZ ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
// USER_HoldingRegisters_Size > 4 OLMAK ZORUNDADIR.
#define USER_CoilStatus_Size 20
#define USER_InputStatus_Size 20
#define USER_InputRegisters_Size 20
#define USER_HoldingRegisters_Size 20
//==============================================================================================================================================================================================
//==============================================================================================================================================================================================
const uint16_t CoilStatus_Size = USER_CoilStatus_Size ;
const uint16_t InputStatus_Size = USER_InputStatus_Size;
const uint16_t InputRegisters_Size = USER_InputRegisters_Size;
const uint16_t HoldingRegisters_Size = USER_HoldingRegisters_Size;

uint8_t MB_Coils[USER_CoilStatus_Size]; // 0xxxx , R/W , 1-bit
uint8_t MB_DiscrateInput[USER_InputStatus_Size]; // 1xxxx, R , 1-bit
uint16_t MB_InputRegisters[USER_InputRegisters_Size]; //3xxxx, R , 16-bit/word/2-byte
uint16_t MB_HoldingRegisters[USER_HoldingRegisters_Size]; //4xxxx , R/W , 16-bit/word/2-byte
//==============================================================================================================================================================================================
//==============================================================================================================================================================================================


//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ PROJENİZDE KULLANMAK İSTEDİĞİNİZ REGİSTER VE COİLLERİ AŞAĞIDAKİ LİSTELERE, YÖNERGEYE UYGUN ŞEKİLDE EKLEYEBİLİRSİNİZ ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//==============================================================================================================================================================================================
//================================================================================ REGİSTER , COİL LİST ========================================================================================
//==============================================================================================================================================================================================
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓


/* ********COILS******** */

const Coil_Degisken Coil_Map[] =
{
		// BU Coil_Map[] STRUCT DİZİSİ İÇERİSİNE AŞAĞIDA GÖSTERİLDİĞİ GİBİ İSTENİLEN COİLLER EKLENEBİLİR.
		// {GPIOD , GPIO_PIN_15} , // 0 . coil -> (LİSTE 0 NOLU COİLDEN BAŞLAR , 255 NOLU COİLDE SON BULUR. BİRER ARTARAK ARDIŞIK İLERLER. )
		// {GPIOA , GPIO_PIN_12} , // 1 . coil
		// {GPIOC , GPIO_PIN_11} , // 2 . coil
		// ...
		// GÖSTERİLDİĞİ GİBİ SIRASI İLE İSTENİLEN COİL (GPIO_Output) EKLENEBİLİR.

		{GPIOD , GPIO_PIN_12} ,// 0.coil
		{GPIOD , GPIO_PIN_13} ,// 1.coil
		{GPIOD , GPIO_PIN_14} ,// 2.coil
		{GPIOD , GPIO_PIN_15} ,// 3.coil
// MASTERIN OKUYUP YAZABİLECEĞI COİL PİNLERİNİ BELİRLEDİK.

};
const uint16_t Coil_Map_Size = (sizeof(Coil_Map) / sizeof(Coil_Map[0])) ;


//!!!!!!***********************************************************************************************************************************************************************************!!!!!!
//!!!!!!***********************************************************************************************************************************************************************************!!!!!!


/* ********DISCRETE INPUT******** */

const Discrete_Input_Degisken Discrete_Input_Map[] =
{
		// BU Discrete_Input_Map[] STRUCT DİZİSİ İÇERİSİNE AŞAĞIDA GÖSTERİLDİĞİ GİBİ İSTENİLEN DISCRETE INPUTLAR EKLENEBİLİR.
		// {GPIOD , GPIO_PIN_15}, // 0 . Discrete Input -> (LİSTE 0 NOLU DİSCRETE INPUTTAN BAŞLAR , 255 NOLU DİSCRETE INPUTTA SON BULUR. BİRER ARTARAK ARDIŞIK İLERLER.)
		// {GPIOA , GPIO_PIN_0} , // 1 . Discrete Input
		// {GPIOD , GPIO_PIN_3} , // 2 . Discrete Input
		// ...
		// GÖSTERİLDİĞİ GİBİ SIRASI İLE İSTENİLEN DİSCRETE INPUT (GPIO_Input) EKLENEBİLİR.

		{GPIOA , GPIO_PIN_0} ,// 0. Discrete Input
//MASTERİN İSTEDİĞİ ZAMAN OKUYABİLECEĞİ DISCRETE INPUT PİNİNİ YERLEŞTİRDİK.


};
const uint16_t  Discrete_Input_Size = (sizeof(Discrete_Input_Map) / sizeof(Discrete_Input_Map[0]));


//!!!!!!***********************************************************************************************************************************************************************************!!!!!!
//!!!!!!***********************************************************************************************************************************************************************************!!!!!!


/* ********INPUT REGİSTERS******** */

// main.c İÇERİSİNDE AŞAĞIDAKİ ŞEKİLDE , 0 DAN BAŞLANARAK İSTENİLEN  DEĞER-DEĞİŞKEN LER ATANARAK EN GÜNCEL VERİLER OKUNABİLİR.
// MB_InputRegisters[0] = HAL_ADC_GetValue(&hadc1);
// MB_InputRegisters[X] = HAL_ADC_GetValue(&hadcX);
// .....
// GÖSTERİLDİĞİ GİBİ İSTENİLEN SIRA İLE İSTENİLEN INPUT REGİSTER (GPIO_Analog-DEĞER) EKLENEBİLİR.


//!!!!!!***********************************************************************************************************************************************************************************!!!!!!
//!!!!!!***********************************************************************************************************************************************************************************!!!!!!


/* ********HOLDİNG REGİSTERS******** */

// main.c İÇERİSİNDE AŞAĞIDAKİ ŞEKİLDE , 4 DEN BAŞLANARAK İSTENİLEN  DEĞER-DEĞİŞKEN  OKUNABİLİR - YAZILABİLİR.
// !!!!!!ANCAK!!!!!! :  MB_HoldingRegisters[0] , [1] , [2] , [3]   HABERLEŞME AYARLARINI TUTMAKTADIR. [0]->SlaveID [1]->BaudRate(LOW) [2]->BaudRate(HIGH) [3]->ParityBit(0x01->EVEN,0x02->ODD,0x03->NONE)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! KESİNLİKLE BU İLK 4 DEĞERE main.c YADA BAŞKA BİR DOSYA İÇERİSİNDE MÜDAHALE EDİLMEMELİ , DEĞER YAZILMAYA ÇALIŞILMAMALIDIR. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// MB_HoldingRegisters[4] = Hedef_Voltage  ;
// MB_HoldingRegisters[10] = 0x7C03  ;
// Max_Akım = MB_HoldingRegisters[19]
// MB_HoldingRegisters[X] = xxxx  ;
// .....
// GÖSTERİLDİĞİ GİBİ İSTENİLEN SIRA İLE İSTENİLEN HOLDİNG REGİSTER (Value , Define , VB.) EKLENEBİLİR


//!!!!!!***********************************************************************************************************************************************************************************!!!!!!
//!!!!!!***********************************************************************************************************************************************************************************!!!!!!

