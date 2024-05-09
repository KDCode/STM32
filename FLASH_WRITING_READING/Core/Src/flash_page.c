#include <flash_page.h>
#include "string.h"
#include "stdio.h"
/*
 * This code is for the STM32F030K6T6 microcontroller, where flash memory is divided into pages.
 * Before writing to flash at a specific page, it's necessary to erase the page first, then store the new value at that location.
 * Therefore, it's recommended to first read from flash, then update only the required location value.
 *
 * For example:
 * Let's say you want to write at location 0x08007C00. Assuming each data unit is 32 bits, the values from 0x08007C00 to 0x08007C10 will be stored, accommodating four values.
 *
 * Example:
 * 1st Loc    2nd Loc    3rd Loc    4th Loc
 * 00000000   00000001   00000002   00000003
 *
 * If you want to update the value at the 2nd location from 00000001 to 00000005 without altering the values at locations 1st, 3rd, and 4th:
 *
 * For this, consider the following example:
 * Take a 32-bit array with a size of 32:
 * uint32_t flash_buffer[32];
 * Now, assign values:
 * flash_buffer[0] = 0x00;
 * flash_buffer[1] = 0x01;
 * flash_buffer[2] = 0x02;
 * flash_buffer[3] = 0x03;
 *
 * Flash_Write_Data(0x08007C00, flash_buffer, 4);
 *
 * It will store the values as per the initial example:
 * 1st Loc    2nd Loc    3rd Loc    4th Loc
 * 00000000   00000001   00000002   00000003
 *
 * Now, if you want to change the second location from 00000001 to 00000005:
 *
 * Flash_Read_Data(0x08007C00, flash_buffer, 4);
 *
 * flash_buffer[1] = 0x05;
 *
 * It will update the flash as follows:
 * 1st Loc    2nd Loc    3rd Loc    4th Loc
 * 00000000   00000005   00000002   00000003
 *
 * It's recommended to create a read function and call it before the main loop (infinite loop) where all functions are initialized.
 *
 *
 * Points to be noted if you want to use this for other microcontrollers:
 *
 * EraseInitStruct:
 *
 * Different microcontrollers have different members in this structure.
 * For example:
 * - In STM32F030K6T6, there are 3 members: TypeErase, PageAddress, and NbPages.
 * - In STM32G070CBT6, there are 4 members: TypeErase, Banks, Page, and NbPages.
 *
 * So make sure to check the FLASH_EraseInitTypeDef structure first.
 *
 */

/* FLASH_PAGE_SIZE should be able to get the size of the Page according to the controller */


static uint32_t GetPage(uint32_t Address)
{
  for (int indx=0; indx<128; indx++)
  {
	  if((Address < (0x08000000 + (FLASH_PAGE_SIZE *(indx+1))) ) && (Address >= (0x08000000 + FLASH_PAGE_SIZE*indx)))
	  {
		  return (0x08000000 + FLASH_PAGE_SIZE*indx);
	  }
  }

  return 0;
}

uint8_t bytes_temp[4];

void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;

	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();

	   /* Erase the user Flash area*/

	  uint32_t StartPage = GetPage(StartPageAddress);
	  uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	  uint32_t EndPage = GetPage(EndPageAdress);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.PageAddress = StartPage;
	   EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  return HAL_FLASH_GetError ();
	   }

	   /* Program the user Flash area word by word*/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	   HAL_FLASH_Lock();

	   return 0;
}


void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{

		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}
void Convert_To_Str (uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char *)Data)/4) + ((strlen((char *)Data) % 4) != 0)) *4;

	for (int i=0; i<numberofbytes; i++)
	{
		Buf[i] = Data[i/4]>>(8*(i%4));
	}
}


void Flash_Write_NUM (uint32_t StartSectorAddress, float Num)
{

	float2Bytes(bytes_temp, Num);

	Flash_Write_Data (StartSectorAddress, (uint32_t *)bytes_temp, 1);
}


float Flash_Read_NUM (uint32_t StartSectorAddress)
{
	uint8_t buffer[4];
	float value;

	Flash_Read_Data(StartSectorAddress, (uint32_t *)buffer, 1);
	value = Bytes2float(buffer);
	return value;
}
