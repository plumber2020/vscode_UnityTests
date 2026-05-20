/*!
*@file crc.c
*@brief Файл реализации функций расчета контрольных сумм
*/

/*------------------------------------------------------------------------------
        Заголовки
-------------------------------------------------------------------------------*/
#include "crc.h"

/*------------------------------------------------------------------------------
        Локальные определения
-------------------------------------------------------------------------------*/
/*!
*@def W
*@brief Ддлина для CRC
*/
#define	W							16
/*!
*@def B
*brief Побайтное
*/
#define	B							8

/*!
*@def LENGHT
*@brief Длина сегмента
*/
#define LENGHT						0x100
/*!
*@var PROG_FIRST_START
*@brief Адрес начала ПО
*/
#define PROG_FIRST_START			(unsigned char *)0x00ul
/*!
*@def PROG_FIRST_STOP
*@brief Адрес конца ПО
*/
#define PROG_FIRST_STOP				0x1FFFFul

/*!
*@var const unsigned short CRCval
*@brief Эталон. Вычисляется экспериментальным путем
*В памяти записано с младшего байта
*/
/* Адрес пока закоментирован из-за ошибок открытия hex-файла в программаторе
Надо позже разобраться!!! */
const unsigned short CRCval /* __attribute__((at(0x30000))) */= 0x6b8a;

/*------------------------------------------------------------------------------
        Глобальные переменные
-------------------------------------------------------------------------------*/
/*!
*@var uint16_t CodeCRC
*@brief Расчитанная контрольная сумма ПО
*/
uint16_t CodeCRC;
/*!
*@var uint16_t soft_crc_for_can
*@brief Контрольная сумма ПО, передаваемая по CAN
*/
uint16_t soft_crc_for_can;

/*------------------------------------------------------------------------------
        Внешние переменные
-------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
        Локальные переменные
-------------------------------------------------------------------------------*/
/*!
*@var static const unsigned short crctab[1<<B]
*@brief Таблица для рассчета CRC по CCITT
*/
static const unsigned short crctab[1<<B] = { /*на uint16_t*/
	0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
	0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
	0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
	0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
	0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
	0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
	0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
	0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
	0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
	0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
	0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
	0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
	0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
	0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
	0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
	0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
	0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
	0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
	0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
	0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
	0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
	0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
	0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
	0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
	0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
	0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
	0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
	0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
	0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
	0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
	0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
	0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

/*------------------------------------------------------------------------------
        Локальные функции (объявления)
-------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
        Локальные функции (реализация)
-------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
        Глобальные функции
-------------------------------------------------------------------------------*/
/*!
*@fn uint16_t crc16(uint16_t icrc,uint8_t *icp,uint32_t icnt)
*Производит вычисление контрольной суммы блока памяти (с учетом
*суммы предыдущих блоков) по способу CCITT
*
*@param uint16_t icrc - исходное значение контрольной суммы
*@param uint8_t *icp - указатель на начало блока памяти
*@param  uint32_t icnt - длина в байтах
*@return Значение контрольной суммы
*/
uint16_t crc16(uint16_t icrc, uint8_t *icp,uint32_t icnt)
{
	uint16_t crc = icrc;
	uint8_t *cp = icp;
	uint32_t cnt = icnt;

	while(cnt--)
		crc = (crc<<B) ^ crctab[(crc>>(W-B)) ^ *cp++];

	return crc;
}

//Возвращает однобайтную КС
uint8_t crc8(uint8_t icrc,uint8_t *icp,uint32_t icnt)
{
	uint16_t Crc16 = crc16(icrc,icp,icnt);
	return Crc16+(Crc16>>8);
}

///*!
//*@fn uint16_t crc_data(uint8_t *data,uint16_t size)
//*@param uint8_t *data - указатель на данных, контрольная сумма которых расчитывается
//*@param uint16_t size - размер данных
//*@return Расчитанная контрольная сумма
//*/
//uint16_t crc_data(uint8_t *data,uint16_t size)
//{
//	/*Локальные переменные*/
//	uint16_t i;
//	uint16_t crc;
//	
//	crc=0;
//	for(i=2;i<size;i++)
//	{
//		crc=crc+*(data+i);
//	}
//	return crc;
//}

/*!
*@fn unsigned short xupdcrc(unsigned short icrc,  unsigned char *icp, unsigned int icnt)
*@param unsigned short icrc - предыдущее значение КС
*@param unsigned char *icp - указатель на начало блока данных типа char
*@param unsigned int icnt - размер блока данных
*@return Расчитанная контрольная сумма блока данных. В отличие от crc16, начальный адрес имеет формат char
*/

unsigned short xupdcrc(unsigned short icrc, unsigned char *icp, unsigned int icnt)
{
	/* Локальные переменные */
	unsigned short crc = icrc;
	unsigned char  *cp = icp;
	unsigned int cnt = icnt;

	while( cnt--)
	{
		crc = (crc<<B) ^ crctab[(crc>>(W-B)) ^ *cp++];
	}

	return( crc );
} 

/*!
*@fn uint16_t CheckProgram(void)
*Функция подсчета КС программы. После подсчета сравнивается с эталоном во флеш-памяти
*@return 0 - КС совпадает с эталоном, 1 - не совпадает
*/
uint16_t CheckProgram(void)
{
	/* Локальные переменные */
	static int8_t IsCheck = 0;
	static uint16_t crc = 0;
	static unsigned char *Start_ptr = PROG_FIRST_START;	/*c начала кода - Сегмент 0*/
	int16 lenght = LENGHT;
	static uint32_t curr_end = PROG_FIRST_STOP;
	static int8_t IsFirst = 1;

	if (Start_ptr + LENGHT > ((unsigned char*)curr_end))
	{ 
		if(IsFirst) 	
		{
			IsFirst=0;	  								 /*заканчивается первый сегмент*/
			curr_end = PROG_FIRST_STOP;
		    lenght=PROG_FIRST_STOP - (uint32_t)Start_ptr;
			crc = xupdcrc(crc, Start_ptr, lenght);
			Start_ptr = PROG_FIRST_START;
			IsCheck = 1;
		}
	}
	else
	{
		crc = xupdcrc(crc, Start_ptr, lenght);
		Start_ptr += lenght;
	}

	if (IsCheck) 
	{
		IsCheck=0;
		IsFirst=1;
		curr_end = PROG_FIRST_STOP;
		Start_ptr = PROG_FIRST_START;
		CodeCRC = crc;
     
//      crc=0;
//      return 0;//Cтроки раскомментарить, чтобы контроль CRC не выполнялся 

		if (crc == CRCval)
		{
			crc=0;
			return 0;
		}//all right !!!

		crc=0;
		return 1;//error in code is found

	}
	return 100;//i'm not checking yet
}
