/*
 * digiTOS-Core.c
 *
 *  Created on: 8 дек. 2018 г.
 *      Author: Nick Nifontov
 */

#include "digiTOS-Core.h"

volatile enum fBoardStatus BoardStatus=sBoot;
volatile int DevMode1=0;

char uart_buff[100];
char crc_buff[20];
volatile int huart1_finished_tx = 1;
volatile enum fSinWave SinWave=swStop;

button_struct_t DevModeKey; // button DEV_MODE1
button_struct_t DevModeKey2; // button DEV_MODE2

/*volatile uint16_t sin_data[SinRes] =
{7,
13,
20,
26,
33,
39,
46,
52,
59,
65,
72,
78,
85,
92,
98,
105,
111,
118,
124,
131,
137,
143,
150,
156,
163,
169,
176,
182,
189,
195,
202,
208,
214,
221,
227,
233,
240,
246,
252,
259,
265,
271,
278,
284,
290,
297,
303,
309,
315,
321,
328,
334,
340,
346,
352,
358,
364,
371,
377,
383,
389,
395,
401,
407,
413,
419,
425,
431,
436,
442,
448,
454,
460,
466,
471,
477,
483,
489,
494,
500,
506,
511,
517,
522,
528,
534,
539,
545,
550,
556,
561,
566,
572,
577,
582,
588,
593,
598,
604,
609,
614,
619,
624,
629,
634,
639,
644,
649,
654,
659,
664,
669,
674,
679,
684,
688,
693,
698,
702,
707,
712,
716,
721,
725,
730,
734,
739,
743,
748,
752,
756,
760,
765,
769,
773,
777,
781,
785,
789,
793,
797,
801,
805,
809,
813,
817,
820,
824,
828,
831,
835,
839,
842,
846,
849,
853,
856,
859,
863,
866,
869,
872,
876,
879,
882,
885,
888,
891,
894,
897,
900,
903,
905,
908,
911,
914,
916,
919,
921,
924,
926,
929,
931,
934,
936,
938,
940,
943,
945,
947,
949,
951,
953,
955,
957,
959,
961,
962,
964,
966,
968,
969,
971,
972,
974,
975,
977,
978,
979,
981,
982,
983,
984,
986,
987,
988,
989,
990,
991,
991,
992,
993,
994,
995,
995,
996,
996,
997,
997,
998,
998,
999,
999,
999,
999,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
1000,
999,
999,
999,
999,
998,
998,
997,
997,
996,
996,
995,
995,
994,
993,
992,
991,
991,
990,
989,
988,
987,
986,
984,
983,
982,
981,
979,
978,
977,
975,
974,
972,
971,
969,
968,
966,
964,
962,
961,
959,
957,
955,
953,
951,
949,
947,
945,
943,
940,
938,
936,
934,
931,
929,
926,
924,
921,
919,
916,
914,
911,
908,
905,
903,
900,
897,
894,
891,
888,
885,
882,
879,
876,
872,
869,
866,
863,
859,
856,
853,
849,
846,
842,
839,
835,
831,
828,
824,
820,
817,
813,
809,
805,
801,
797,
793,
789,
785,
781,
777,
773,
769,
765,
760,
756,
752,
748,
743,
739,
734,
730,
725,
721,
716,
712,
707,
702,
698,
693,
688,
684,
679,
674,
669,
664,
659,
654,
649,
644,
639,
634,
629,
624,
619,
614,
609,
604,
598,
593,
588,
582,
577,
572,
566,
561,
556,
550,
545,
539,
534,
528,
522,
517,
511,
506,
500,
494,
489,
483,
477,
471,
466,
460,
454,
448,
442,
436,
431,
425,
419,
413,
407,
401,
395,
389,
383,
377,
371,
364,
358,
352,
346,
340,
334,
328,
321,
315,
309,
303,
297,
290,
284,
278,
271,
265,
259,
252,
246,
240,
233,
227,
221,
214,
208,
202,
195,
189,
182,
176,
169,
163,
156,
150,
143,
137,
131,
124,
118,
111,
105,
98,
92,
85,
78,
72,
65,
59,
52,
46,
39,
33,
26,
20,
13,
};*/

volatile uint16_t sin_data[SinRes] =
{0,13,26,39,52,65,78,91,104,117,130,143,156,169,182,195,207,220,233,246,258,271,284,296,309,321,333,346,358,370,
382,394,406,418,430,442,453,465,477,488,500,511,522,533,544,555,566,577,587,598,608,619,629,639,649,659,669,678,688,697,
707,716,725,734,743,751,760,768,777,785,793,801,809,816,824,831,838,845,852,859,866,872,878,884,891,896,902,908,913,918,
923,928,933,938,942,946,951,955,958,962,965,969,972,975,978,980,983,985,987,989,991,993,994,995,996,997,998,999,999,999,
999,999,999,998,997,996,995,994,993,991,989,987,985,983,980,978,975,972,969,965,962,958,955,951,946,942,938,933,928,923,
918,913,908,902,896,891,884,878,872,866,859,852,845,838,831,824,816,809,801,793,785,777,768,760,751,743,734,725,716,707,
697,688,678,669,659,649,639,629,619,608,598,587,577,566,555,544,533,522,511,500,488,477,465,453,442,430,418,406,394,382,
370,358,346,333,321,309,296,284,271,258,246,233,220,207,195,182,169,156,143,130,117,104,91,78,65,52,39,26,13,0};

volatile uint16_t sin_step=0;
volatile uint8_t sinStatus=0;

uint32_t TM_CRC_Calculate8(uint8_t* arr, uint32_t count, uint8_t reset) {
	uint32_t cnt;

	/* Reset CRC data register if necessary */
	if (reset) {
		/* Reset generator */
		CRC->CR = CRC_CR_RESET;
	}

	/* Calculate number of 32-bit blocks */
	cnt = count >> 2;

	/* Calculate */
	while (cnt--) {
		/* Set new value */
		CRC->DR = *(uint32_t *)arr;

		/* Increase by 4 */
		arr += 4;
	}

	/* Calculate remaining data as 8-bit */
	cnt = count % 4;

	/* Calculate */
	while (cnt--) {
		/* Set new value */
		*((uint8_t *)&CRC->DR) = *arr++;
	}

	/* Return data */
	return CRC->DR;
}

uint32_t TM_CRC_Calculate16(uint16_t* arr, uint32_t count, uint8_t reset) {
	uint32_t cnt;

	/* Reset CRC data register if necessary */
	if (reset) {
		/* Reset generator */
		CRC->CR = CRC_CR_RESET;
	}

	/* Calculate number of 32-bit blocks */
	cnt = count >> 1;

	/* Calculate */
	while (cnt--) {
		/* Set new value */
		CRC->DR = *(uint32_t *)arr;

		/* Increase by 2 */
		arr += 2;
	}

	/* Calculate remaining data as 8-bit */
	cnt = count % 4;

	/* Calculate */
	while (cnt--) {
		/* Set new value */
		*((uint8_t *)&CRC->DR) = *arr++;
	}

	/* Return data */
	return CRC->DR;
}

uint32_t TM_CRC_Calculate32(uint32_t* arr, uint32_t count, uint8_t reset) {
	/* Reset CRC data register if necessary */
	if (reset) {
		/* Reset generator */
		CRC->CR = CRC_CR_RESET;
	}

	/* Calculate CRC */
	while (count--) {
		/* Set new value */
		CRC->DR = *arr++;
	}

	/* Return data */
	return CRC->DR;
}

/*uint16_t crc16Update(uint16_t crc, uint8_t a)
{
  int iii;
  crc ^= a;
  for (iii = 0; iii < 8; ++iii)  {
    if (crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }
  return crc;
}

uint32_t Get_CRC(uint8_t * p, uint16_t len) {
	  uint32_t res= ~0;
	  for (uint16_t i_index = 0; i_index < len - 2; ++i_index)
		 res = crc16Update( res, *p++);
	  return res;
}*/

void AddUART_Header() {
	ClearUART_Buff();
	strcat(uart_buff,"@data@");
}

void AddUART_CRC() {
	memset(crc_buff, '\0', sizeof(crc_buff));
	//uint32_t crc=Get_CRC((uint8_t *) &uart_buff, sizeof(uart_buff));
	uint32_t crc=TM_CRC_Calculate8((uint8_t *) &uart_buff, sizeof(uart_buff), 1);
	sprintf(crc_buff, "@crc@%08X@\r\n", crc);
	strcat(uart_buff,crc_buff);

}

void Get_ChipID(void) {
	ClearUART_Buff();


	/* Format unique ID */
	sprintf(uart_buff, "@id@%08X-%08X-%08X", TM_ID_GetUnique32(0), TM_ID_GetUnique32(1), TM_ID_GetUnique32(2));

	AddUART_CRC();
}

/*void Get_Signature(void) {
	uart_buff[0]=0;


    // Format device signature
    sprintf(uart_buff, "Device signature: %04X", TM_ID_GetSignature());
    strcat(uart_buff,"\r\n");

}

void Get_Revision(void) {
	uart_buff[0]=0;

    // Format revision
    sprintf(uart_buff, "Revision: %04X", TM_ID_GetRevision());
    strcat(uart_buff,"\r\n");

}

void Get_Package(void) {
	uart_buff[0]=0;

    // Format package
    sprintf(uart_buff, "Package: %04X", TM_ID_GetPackage());
    strcat(uart_buff,"\r\n");

}*/

void Get_FlashSize(void) {
	ClearUART_Buff();

    /* Format flash size */
    sprintf(uart_buff, "@flash_size@%02d", TM_ID_GetFlashSize());

    AddUART_CRC();
}

void Get_Version(void) {
	ClearUART_Buff();

	strcat(uart_buff,"@version@");

    strcat(uart_buff,__DATE__);

    AddUART_CRC();
}

void SerialPrintln(int WaitForTX) {

	huart1_finished_tx = 0;
	while (HAL_UART_Transmit_DMA(&huart1, (uint8_t*) uart_buff, sizeof(uart_buff)-1) == HAL_BUSY);
	if (WaitForTX) {
		WaitForUART_TX();
	}
}

void ClearUART_Buff() {
	memset(uart_buff, '\0', sizeof(uart_buff));
}

void WaitForUART_TX() {
	while (!huart1_finished_tx);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)  // change USART instance
    {
    	huart1_finished_tx = 1;
    	ClearUART_Buff();
    }
}

void ResetWDG() {
	 HAL_IWDG_Refresh(&hiwdg);
	 //IWDG->KR = 0x0000AAAAU;
}
