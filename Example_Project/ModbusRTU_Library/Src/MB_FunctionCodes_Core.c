/*
 * MB_FunctionCodes_Core.c
 *
 *  Created on: Feb 6, 2026
 *      Author: hanal
 *      Muhammet Alper Demirhan
 */

#include "MB_FunctionCodes_Core.h"
#include "MB_USER_REGISTERS_MAP_BOX.h"
#include "MB_System_Core.h"

uint8_t Connectivity_Setting_Flag =0;


uint16_t Cevap_Hazirla(uint8_t* Rx_Paket , uint8_t* Tx_Paket)  // *****************************************************
{
	uint16_t Gonderilecek_Paket_Uzunluk = 0;
	if(ModbusRTU_Check() == 1)
	{
		uint8_t FCode = Rx_Paket[1];

		switch(FCode)
		{
		case 0x01 : // Read Coils
			Gonderilecek_Paket_Uzunluk = Read_Coils(Rx_Paket, Tx_Paket);
			break ;
		case 0x02 : // Read Discrete Inputs
			Gonderilecek_Paket_Uzunluk = Read_Discrete_Inputs(Rx_Paket, Tx_Paket);
			break ;
		case 0x03 : // Read Holding Registers
			Gonderilecek_Paket_Uzunluk = Read_Holding_Registers(Rx_Paket, Tx_Paket);
			break ;
		case 0x04 : // Read Input Registers
			Gonderilecek_Paket_Uzunluk = Read_Input_Registers(Rx_Paket, Tx_Paket);
			break ;
		case 0x05 : // Write Single Coil
			Gonderilecek_Paket_Uzunluk = Write_Single_Coil(Rx_Paket, Tx_Paket);
			break ;
		case 0x06 : // Write Single Register
			Gonderilecek_Paket_Uzunluk = Write_Single_Register(Rx_Paket, Tx_Paket);
			break ;
		case 0x0F : // Write Multiple Coils
			Gonderilecek_Paket_Uzunluk = Write_Multiple_Coils(Rx_Paket, Tx_Paket);
			break ;
		case 0x10 : //Write Multiple Registers
			Gonderilecek_Paket_Uzunluk = Write_Multiple_Registers(Rx_Paket, Tx_Paket);
			break ;
		// ......
		// FUNCTİON CODELER , BU KISMIN DEVAMINA case-break YAPISI İLE EKLENEBİLİR. EKLENEN FUNCTİON CODELER TX_PAKET LERİN UZUNLUK (PAKET SİZE) BİLGİSİNİ return ETMELİDİR.

		default :
			Gonderilecek_Paket_Uzunluk = Exception_Codes(Rx_Paket[0], FCode, 0x01, Tx_Paket);
			break;
		}
	}
	else
	{
		return 0 ;
	}

	return Gonderilecek_Paket_Uzunluk ;
}


uint16_t Exception_Codes(uint8_t Slave_ID , uint8_t Function_Code , uint8_t Exception_Code , uint8_t* Cevap)// ********
{
	Cevap[0] = Slave_ID;
	Cevap[1] = (Function_Code | 0x80);
	Cevap[2] = Exception_Code ;
	uint16_t CRc = (CRC16_Hesapla(Cevap, 3));
	Cevap[3] = (uint8_t)(CRc);
	Cevap[4] = (uint8_t)(CRc >> 8);
	return 5 ;
}

uint16_t Read_Coils(uint8_t* Istek , uint8_t* Cevap) //0x01 ***********************************************************
{
	uint16_t Start_Coil = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Okunacak_Adet = Byte_Birlestir(Istek[4], Istek[5]);
	uint8_t Byte_Sayısı = (Okunacak_Adet / 8) ;
	if((Okunacak_Adet % 8) > 0)
	{
		Byte_Sayısı++;
	}

	if((Okunacak_Adet + Start_Coil) > Coil_Map_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x01, 0x02, Cevap);
	}

	for(uint16_t i = Start_Coil; i < (Start_Coil + Okunacak_Adet) ; i++)
	{
		MB_Coils[i] = HAL_GPIO_ReadPin(Coil_Map[i].Port, Coil_Map[i].Pin);
	}

	Cevap[0]= MB_SlaveID;
	Cevap[1] = 0x01 ;
	Cevap[2] = Byte_Sayısı ;

	for(uint16_t k = 0 ; k<Byte_Sayısı ; k++)
	{
		Cevap[3+k] = 0x00 ;
	}

	uint8_t Hedef_bit = 0 ;
	uint8_t Hedef_byte = 0 ;
	for(uint16_t i = Start_Coil ; i < (Start_Coil + Okunacak_Adet) ; i++)
	{
		uint8_t Coil_Durumu = MB_Coils[i];
		if(Coil_Durumu > 0)
		{
			Cevap[Hedef_byte +3] = Cevap[Hedef_byte +3] | (1 << Hedef_bit) ;
		}

		Hedef_bit++;
		if(Hedef_bit > 7)
		{
			Hedef_byte++;
			Hedef_bit = 0 ;
		}
	}

	uint16_t Paket_Uzunlugu = Byte_Sayısı+3 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Read_Discrete_Inputs(uint8_t* Istek , uint8_t* Cevap ) //0x02 ************************************************
{
	uint16_t Start_Discrete_Input = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Okunacak_Adet = Byte_Birlestir(Istek[4], Istek[5]);
	uint8_t Byte_Sayısı = (Okunacak_Adet / 8) ;
	if((Okunacak_Adet % 8) > 0)
	{
		Byte_Sayısı++;
	}

	if((Start_Discrete_Input + Okunacak_Adet) > Discrete_Input_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x02, 0x02, Cevap);
	}

	for(uint16_t i = Start_Discrete_Input ; i < (Start_Discrete_Input + Okunacak_Adet) ; i++)
	{
		MB_DiscrateInput[i] = HAL_GPIO_ReadPin(Discrete_Input_Map[i].Port, Discrete_Input_Map[i].Pin);
	}

	Cevap[0]= MB_SlaveID ;
	Cevap[1] = 0x02 ;
	Cevap[2] = Byte_Sayısı ;

	for(uint16_t k = 0 ; k<Byte_Sayısı ; k++)
	{
		Cevap[3+k] = 0x00 ;
	}

	uint8_t Hedef_bit = 0 ;
	uint8_t Hedef_byte = 0 ;
	for(int16_t i = Start_Discrete_Input ; i < (Start_Discrete_Input + Okunacak_Adet) ; i++)
	{
		uint8_t Discrete_Input_Durumu = MB_DiscrateInput[i];
		if(Discrete_Input_Durumu > 0)
		{
			Cevap[Hedef_byte +3] = Cevap[Hedef_byte +3] | (1 << Hedef_bit) ;
		}

		Hedef_bit++;
		if(Hedef_bit > 7)
		{
			Hedef_byte++;
			Hedef_bit = 0 ;
		}
	}

	uint16_t Paket_Uzunlugu = Byte_Sayısı+3 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Read_Holding_Registers(uint8_t* Istek , uint8_t* Cevap) //0x03 ***********************************************
{
	uint16_t Start_Holding_Register = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Okunacak_Adet = Byte_Birlestir(Istek[4], Istek[5]);
	uint16_t Byte_Sayısı = (Okunacak_Adet*2) ;

	if((Start_Holding_Register + Okunacak_Adet) > HoldingRegisters_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x03, 0x02 , Cevap);
	}

	Cevap[0]= MB_SlaveID ;
	Cevap[1] = 0x03 ;
	Cevap[2] = Byte_Sayısı ;

	for(uint16_t k = 0 ; k<Byte_Sayısı ; k++)
	{
		Cevap[3+k] = 0x00 ;
	}

	for(uint16_t i = 0 ; i < Okunacak_Adet ; i++)
	{
		uint16_t Holding_Register_Durumu = MB_HoldingRegisters[Start_Holding_Register+i];
		Cevap[3 + (i*2)] = (uint8_t)(Holding_Register_Durumu >> 8) ;
		Cevap[4 + (i*2)] = (uint8_t)Holding_Register_Durumu ;
	}

	uint16_t Paket_Uzunlugu = Byte_Sayısı+3 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Read_Input_Registers(uint8_t* Istek ,uint8_t* Cevap) //0x04 **************************************************
{
	uint16_t Start_Input_Register = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Okunacak_Adet = Byte_Birlestir(Istek[4], Istek[5]);
	uint16_t Byte_Sayısı = Okunacak_Adet*2 ;

	if((Start_Input_Register + Okunacak_Adet) > InputRegisters_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x04, 0x02, Cevap);
	}

	Cevap[0]= MB_SlaveID ;
	Cevap[1] = 0x04 ;
	Cevap[2] = Byte_Sayısı ;

	for(uint16_t k = 0 ; k<Byte_Sayısı ; k++)
	{
		Cevap[3+k] = 0x00 ;
	}

	for(uint16_t i = 0 ; i < Okunacak_Adet ; i++)
	{
		uint16_t Input_Register_Durumu = MB_InputRegisters[Start_Input_Register+i];
		Cevap[3 + (i*2)] = (uint8_t)(Input_Register_Durumu >> 8) ;
		Cevap[4 + (i*2)] = (uint8_t)Input_Register_Durumu ;
	}

	uint16_t Paket_Uzunlugu = Byte_Sayısı+3 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Write_Single_Coil(uint8_t* Istek , uint8_t* Cevap) //0x05 ****************************************************
{
	uint16_t Yazilacak_Adres = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Adres_Durumu = Byte_Birlestir(Istek[4], Istek[5]);

	if(Yazilacak_Adres > Coil_Map_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x05, 0x02, Cevap);
	}

	if(Adres_Durumu == 0xFF00)
	{
		MB_Coils[Yazilacak_Adres] = 1 ;
		HAL_GPIO_WritePin(Coil_Map[Yazilacak_Adres].Port, Coil_Map[Yazilacak_Adres].Pin, GPIO_PIN_SET);
	}
	else if (Adres_Durumu == 0x0000)
	{
		MB_Coils[Yazilacak_Adres] = 0 ;
		HAL_GPIO_WritePin(Coil_Map[Yazilacak_Adres].Port, Coil_Map[Yazilacak_Adres].Pin, GPIO_PIN_RESET);
	}

	Cevap[0] = Istek[0] ;
	Cevap[1] = Istek[1] ;
	Cevap[2] = Istek[2] ;
	Cevap[3] = Istek[3] ;
	Cevap[4] = Istek[4] ;
	Cevap[5] = Istek[5] ;

	uint16_t Paket_Uzunlugu = 6 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Write_Single_Register(uint8_t* Istek , uint8_t* Cevap) // 0x06 ***********************************************
{
	uint16_t Yazilacak_Adres = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Adres_Degeri = Byte_Birlestir(Istek[4], Istek[5]);

	if(Yazilacak_Adres > HoldingRegisters_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x06, 0x02, Cevap);
	}

	MB_HoldingRegisters[Yazilacak_Adres] = Adres_Degeri ;

	if(Yazilacak_Adres < 4)
	{
		Connectivity_Setting_Flag = 1;
	}

	Cevap[0] = Istek[0] ;
	Cevap[1] = Istek[1] ;
	Cevap[2] = Istek[2] ;
	Cevap[3] = Istek[3] ;
	Cevap[4] = Istek[4] ;
	Cevap[5] = Istek[5] ;

	uint16_t Paket_Uzunlugu = 6 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Write_Multiple_Coils(uint8_t* Istek , uint8_t* Cevap) //0x0F *************************************************
{
	uint16_t Start_Coil = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Coil_Adedi = Byte_Birlestir(Istek[4], Istek[5]);
	uint8_t  Byte_Adedi = Istek[6];

	if((Start_Coil + Coil_Adedi) > Coil_Map_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x0F, 0x02, Cevap);
	}

	uint16_t Coil_No = 0 ;
	for(uint16_t a = 0 ; a < Byte_Adedi ; a++)
	{
		uint8_t Bit_Sayisi;
		if(((a + 1) * 8) <= Coil_Adedi)
		{
			Bit_Sayisi = 8 ;
		}
		else
		{
			Bit_Sayisi = (Coil_Adedi % 8);
		}
		for(uint8_t i = 0 ; i < Bit_Sayisi ; i++)
		{
			uint8_t Data_Coil_Deger = (Istek[7 + a] >> i) ;
			if((Data_Coil_Deger & 0x01) == 0x01)
			{
				MB_Coils[Start_Coil + Coil_No] = 0x01 ;
				HAL_GPIO_WritePin(Coil_Map[Start_Coil + Coil_No].Port, Coil_Map[Start_Coil + Coil_No].Pin, GPIO_PIN_SET);
			}
			else
			{
				MB_Coils[Start_Coil + Coil_No] = 0x00 ;
				HAL_GPIO_WritePin(Coil_Map[Start_Coil + Coil_No].Port, Coil_Map[Start_Coil + Coil_No].Pin, GPIO_PIN_RESET);
			}

			Coil_No++ ;
		}
	}

	Cevap[0] = Istek[0] ;
	Cevap[1] = Istek[1] ;
	Cevap[2] = Istek[2] ;
	Cevap[3] = Istek[3] ;
	Cevap[4] = Istek[4] ;
	Cevap[5] = Istek[5] ;

	uint16_t Paket_Uzunlugu = 6 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

uint16_t Write_Multiple_Registers(uint8_t* Istek , uint8_t* Cevap) //0x10 **********************************************
{
	uint16_t Start_Register = Byte_Birlestir(Istek[2], Istek[3]);
	uint16_t Register_Adedi =  Byte_Birlestir(Istek[4], Istek[5]);

	if((Start_Register + Register_Adedi) > HoldingRegisters_Size)
	{
		//Hata
		return Exception_Codes(Istek[0], 0x10, 0x02, Cevap);
	}

	for(uint16_t i = 0 ; i < Register_Adedi ; i++)
	{
		uint16_t Register = Byte_Birlestir(Istek[7 + (i*2)], Istek[8 + (i*2)]);
		MB_HoldingRegisters[Start_Register + i] = Register ;

		if((Start_Register + i) < 4)
		{
			Connectivity_Setting_Flag = 1 ;
		}
	}

	Cevap[0] = Istek[0] ;
	Cevap[1] = Istek[1] ;
	Cevap[2] = Istek[2] ;
	Cevap[3] = Istek[3] ;
	Cevap[4] = Istek[4] ;
	Cevap[5] = Istek[5] ;

	uint16_t Paket_Uzunlugu = 6 ;
	Cevap[Paket_Uzunlugu] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu));
	Cevap[Paket_Uzunlugu+1] = (uint8_t)(CRC16_Hesapla(Cevap, Paket_Uzunlugu) >> 8 );

	return (Paket_Uzunlugu+2) ;
}

