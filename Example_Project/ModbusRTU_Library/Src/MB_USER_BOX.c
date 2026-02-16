/*
 * MB_USER_BOX.c
 *
 *  Created on: Feb 3, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan , Kocaeli Uni.
 */

#include "MB_USER_BOX.h"
#include "MB_System_Core.h"
#include "MB_ConnectivitySystem_Core.h"
#include "MB_FunctionCodes_Core.h"


/* =====================================================MODBUSRTU-SLAVE KÜTÜPHANESİNİ KULLANAMK İÇİN :  =====================================================*/

//*** Core > Src > main.c İÇERİSİNDE /* USER CODE BEGIN Includes */  SATIRI ALTINA ->  #include "MB_USER_BOX.h" EKLENMELİDİR.
//*** Core > Src > stm32fXxx_it.c(ÖR:stm32f4xx_it.c) İÇERİSİNDE /* USER CODE BEGIN Includes */  SATIRI ALTINA ->  #include "MB_USER_BOX.h" EKLENMELİDİR.
//*** AŞAĞIDA YER ALAN ÜÇ ADET FONKSİYONU YORUM SATIRLARINDA ANLATILDIĞI ŞEKİLDE, BELİRTİLEN YERLERE YERLEŞTİRİNİZ.
//***ModbusRTU_Drivers > ModbusRTU_Library > Src > MB_USER_REGISTERS_MAP_BOX.c İÇERİSİNDEKİ BÖLÜMLERİ KENDİ PROJENİZE GÖRE DOLDURUNUZ.
//↓↓↓Device Configuration Tool DOSYASINDAN Asynchronous MOD da İSTENİLEN USART BİRİMİ AKTİF EDİLMELİDİR.
//***NVIC Settings BÖLÜMÜNDEN "USARTx global interrupt" AKTİF EDİLMELİDİR. DMA Settings BÖLÜMÜNDEN USARTx_RX(Mode:Circular) , USARTx_TX(Mode:Normal) EKLENMELİDİR.

/* ========================================================================================================================================================== */


/* ********************************************************************* FONKSİYONUN KULLANIMI: ************************************************************* */
//*** Ör: stm32f4xx_it.c
// void USART2_IRQHandler(void)
// {
//  /* USER CODE BEGIN USART2_IRQn 0 */
//  MODBUSRTU_InWrite_USARTx_IRQHandler(&huart2);
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//*** Core > Src > stm32fXxx_it.c(ÖR:stm32f4xx_it.c) İÇERİSİNDE KENDİ KULLANDIĞINIZ USART BİRİMİNİN  void USART1_IRQHandler(void)  FONKSİYONU İÇERİSİNDE ÇAĞRILMALIDIR.

//*** ======= FONKSİYONUN PARAMETER KISMINA : =======

//*** huart -> KULLANILAN UART ÇEVRE BİRİMİ YAZILMALIDIR. ÖR:(&huart2)

void MODBUSRTU_InWrite_USARTx_IRQHandler(UART_HandleTypeDef *huart)
{
	ModbusRTU_Receive_InWrite_USARTx_IRQHandler(huart);
	ModbusRTU_Transmit_InWrite_USARTx_IRQHandler(huart);
}

/* ********************************************************************* FONKSİYONUN KULLANIMI: ************************************************************* */
//*** Ör: main.c
// while (1)
// {
//  /* USER CODE BEGIN 3 */
//  MODBUSRTU_Write_While();
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//*** main.c İÇERİSİNDE BULNAN ANA DÖNGÜ (WHİLE (1)) İÇERİSİNDE ÇAĞRILMALIDIR.

void MODBUSRTU_Write_While(void)
{
	if(MB_DataReady_Flag == 1)
	{
		MB_DataReady_Flag = 0 ;
		uint16_t Cevap_Size = Cevap_Hazirla(MB_RxBuffer, MB_TxBuffer);
		if(Cevap_Size >= 4)
		{
		    ModbusRTU_Start_Transmit(MB_huart, MB_TxBuffer, Cevap_Size, MB_RS485_Control_Mode, MB_RS485_ControlPort, MB_RS485_Control_Pin);
		}
		else
		{
			HAL_UART_Receive_DMA(MB_huart, MB_RxBuffer, RxBuffer_Size);
		}
	}
}

/* ********************************************************************* FONKSİYONUN KULLANIMI: ************************************************************* */
//*** Ör: main.c
// /* USER CODE BEGIN PV */
// uint8_t SLAVE_ID;
// uint32_t BAUD_RATE;
// uint32_t PARITY_BIT;

// /* USER CODE BEGIN 2 */
// MODBUSRTU_Start(&huart2, &SLAVE_ID, &BAUD_RATE, &PARITY_BIT, 0, NULL, 0, GPIOA, GPIO_PIN_0, GPIOD, GPIO_PIN_12, 1, 9600, 3, 0x08040000); */
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//*** main.c İÇERİSİNDE /* USER CODE BEGIN 2 */ SATIRI ALTINDA ÇAĞRILMALIDIR.

//*** ======= FONKSİYONUN PARAMETER KISMINA : =======

//*** huart -> KULLANILAN UART ÇEVRE BİRİMİ YAZILMALIDIR. ÖR:(&huart2)
//↓↓↓ SLAVE_ID , BAUD_RATE , PARİTY -> HABERLEŞME AYAR BİLGİSİ OKUMAK İÇİN OLUŞTURULAN GLOBAL DEĞİŞKENLER YAZILMALIDIR. ÖR:(&SLAVEID , &BAUD , &PARITY)
//*** BU DEĞİŞKENLER SLAVEID(8 BİT) , BAUD(32BİT) , PARITY(32BİT) OLMALIDIR. CİHAZIN AÇILIŞ ANINDAKİ HABERLEŞME AYARLARINI GÖSTERİRLER. İÇİNE DEĞER ATANMAMALIDIR.
//↓↓↓ RS485_MODE_1_OR_0 -> KULLANILAN "TTL to RS485" CONVERTER DEVRESİNDE TETİK PİNİ: DE RE (MAX484 IC) VAR MI ? ÖR: VAR->(1) or YOK->(0)
//↓↓↓ RS485_PORT_GPIOx , RS485_GPIO_PIN_X -> EĞER VAR İSE PİNLERİ TETİKLEYECEK GPIO_Output YAZILIR. ÖR: VAR->(GPIOA , GPIO_PIN_4) or YOK->(NULL , 0)
//*** EĞER KULLANILAN TTL to RS485 CONVERTERDE TETİK PİNLERİ BULUNUYORSA (DE RE) BU PİNLER KÖPRÜLENMELİ VE MCU nun GPIO_Output PİNİNE BAĞLANMALIDIR.
//↓↓↓ RstBtn_GPIOx , RstBtn_GPIO_PIN_x -> HABERLEŞME AYARLARINI, FABRİKA AYARLARINA GÜNCELLEYEN GPIO_Input(Pull-Down RESİSTOR) YAZILIR. ÖR: (GPIOB , GPIO_PIN_5) or (NULL , 0)
//*** PUSH PUTTON VDD den GPIO_Input a ÇEKMELİDİR(Pull Up Push Button). RESİSTOR İSE YAZILIMSAL YADA DONANIMSAL GPIO_Input - GND (Pull Down Resistor) OLMALIDIR.
//↓↓↓ Led_GPIOx , Led_GPIO_PIN_x -> HABERLEŞME AYARLARI GÜNCELLEMESİNİN TAMAMLANDIĞINI KULLANICIYA BİLDİREN LED GPIO_Output YAZILIR. ÖR: (GPIOD , GPIO_PIN_15) or (NULL , 0)
//*** KULLANICI PUSH BUTTONA BASTIKTAN İKİ Sn SONRA LED YANIP SÖNMEYE BAŞLAR SONRA SÜREKLİ YANAR EN SONDA SÖNER. LED SÖNDÜĞÜNDE FACTORY RESET İŞLEMİ TAMAMLANMIŞ OLUR.
//*** CİHAZ AÇILIP KAPATILARAK YADA RESETLENEREK GÜNCELLENEN YENİ AYARLAR DEVREYE ALINIR.
//↓↓↓ Factory_SlaveID , Factory_Baud , Factory_Parity_1E_2O_3N -> HABERLEŞME AYARLARININ İSTENİLEN FABRİKA ÇIKIŞI DEĞERLERİ YAZILIR. ÖR: (1 , 9600 , 3)
//*** SlaveID-> 0-247 ARASINDA SEÇİLEBİLİR. BaudRate-> DECİMAL OLARAK YAZILMALIDIR. ParityBit-> 1-2-3 DEĞERLERİNDEN BİRİSİ SEÇİLMELİDİR. 1(EVEN) , 2(ODD) , 3(NONE)
//↓↓↓ Flash_Address -> HABERLEŞME AYARLARININ KAYDEDİLECEĞİ BAŞLANGIÇ FLASH HAFIZA ADRESİ YAZILMALIDIR. ÖR: (0x08020000)
//*** YAZILAN ADRES VE SONRAKİ ARDIŞIL DİĞER İKİ ADRESİN BOŞ OLDUĞUNDAN EMİN OLUN. STM32CubeProgrammer UYGULAMASI İLE KONTOL EDİN. GÜNCELLEME ESNASINDA TÜM SECTOR-PAGE SİLİNİR.

void MODBUSRTU_Start(UART_HandleTypeDef *huart , uint8_t* SLAVE_ID , uint32_t* BAUD_RATE , uint32_t* PARİTY,
		             uint8_t RS485_MODE_1_OR_0, GPIO_TypeDef* RS485_PORT_GPIOx,uint16_t RS485_GPIO_PIN_X,
					 GPIO_TypeDef* RstBtn_GPIOx,uint16_t RstBtn_GPIO_PIN_x,GPIO_TypeDef* Led_GPIOx,uint16_t Led_GPIO_PIN_x,
					 uint8_t Factory_SlaveID , uint32_t Factory_Baud , uint32_t Factory_Parity_1E_2O_3N , uint32_t Flash_Address)
{
	MB_huart = huart;
	MB_FactorySlaveID = Factory_SlaveID ;
	MB_FactoryBaudRate = Factory_Baud;
	MB_FactoryParityBit =  Factory_Parity_1E_2O_3N;
	MB_FlashStart_Address = Flash_Address;

	MB_RS485_Control_Mode = RS485_MODE_1_OR_0;
	MB_RS485_ControlPort = RS485_PORT_GPIOx ;
	MB_RS485_Control_Pin = RS485_GPIO_PIN_X ;


	Connectivity_Default_Update(RstBtn_GPIOx, RstBtn_GPIO_PIN_x, Led_GPIOx, Led_GPIO_PIN_x);

	Connectivity_Init_Update(huart, &MB_SlaveID, &MB_BaudRate, &MB_ParityBit);

	if(SLAVE_ID != NULL)
	{
		*SLAVE_ID = MB_SlaveID ;

	}
	if(BAUD_RATE != NULL)
	{
		*BAUD_RATE = MB_BaudRate ;
	}

	if(PARİTY != NULL)
	{
		*PARİTY = MB_ParityBit ;
	}

	if(MB_RS485_Control_Mode == 1)
	{
		HAL_GPIO_WritePin(RS485_PORT_GPIOx, RS485_GPIO_PIN_X, GPIO_PIN_RESET);
	}
	ModbusRTU_Start_Receive();
}










