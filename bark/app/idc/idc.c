/*!
*@file idc.c
*@brief Файл реализации функций обмена с ИДК
*/

/*------------------------------------------------------------------------------
        Заголовки
-------------------------------------------------------------------------------*/
#include "crc.h"
#include "idc.h"
#include "commio.h"
#include "../ctrl_engine/Control/narabotka.h"
//#include "../narab/Narab_api.h"
#include "../narab/Ring2.h"
#include "../ctrl_engine/Control/messages.h"
#include "../../../common/mcu/ve1t/drivers/uart.h"
#include "../../app/low_level/msg_defs.h"
#include "../../app/bhd_io/Bxd_act.h"

/*------------------------------------------------------------------------------
        Локальные определения
-------------------------------------------------------------------------------*/
#define RBUF(x) RcvBuffer[(x)&(UART_RXD_SIZE-1)]

#define	COEF_RPMS_IDC			100

//Список разрешенных команд для приема от ИДК
#define _COMMANDS_COUNT_ 8
  const CMD_description CMDs[_COMMANDS_COUNT_]={ 
           
										   {0x56,0},					// выдача параметров настойки из ОЗУ
										   {0x57,0},					// выдача параметров настройки из Flash
										   {0x58,0},					// выдача идентификатора устройства из ОЗУ
										   {0x59,0},					// 1ая структура - основной пакет в КПА
										   {0x60,0},					// 2ая структура - пакет со счетчиками отказов
										   {0x61,0},					// 3ая структура
											 {0x80,2},
											 {0x90,2},					// Команды на ППЗ и АВРР

									     };

/*------------------------------------------------------------------------------
        Глобальные переменные
-------------------------------------------------------------------------------*/
/*!
*@var VersionDevice devices[2]
*@brief Список модулей
*@brief Список структур отображаемых в КПА переменных
*/
VersionDevice devices[3];

/*!
*@var volatile VariableGroup1 var_grp_1
*@brief Группа пременных мониторинга
*/
volatile VariableGroup1 var_grp_1;
/*!
*@var volatile VariableGroup2 var_grp_2
*@brief Группа пременных мониторинга
*/
volatile VariableGroup2 var_grp_2;


/*------------------------------------------------------------------------------
        Внешние переменные
-------------------------------------------------------------------------------*/
/*!
*@var extern volatile TableOfDataAndStates data_main_table
*@brief Главная таблица переменных
*/
extern volatile TableOfDataAndStates data_states_table;
/*!
*@var volatile TableOfLogicalSignals6thBlock Reserve1stBlock
*@brief резерв 1-й блок
*/
extern volatile TableOfReserve2ndBlock reserve_table_2;

extern volatile bxd_global_state_type bxd_next_global_state;
extern volatile bool CommandFromIDK;

/*------------------------------------------------------------------------------
        Внешние функции (ассемблерные)
-------------------------------------------------------------------------------*/
/*!
*@fn extern void* mem_cpy(void *to, void *from, uint16_t size)
*@brief Функция копирования памяти
*Реализована в модуле mdr32_mem.s на ассемблере
*/
extern void* mem_cpy(void *to, void *from, uint16_t size);

/*------------------------------------------------------------------------------
        Локальные переменные
-------------------------------------------------------------------------------*/
/*!
*@var static uint16_t virtual_pack_number
@brief Номер очередного виртуального канала
*/
static uint16_t virtual_pack_number = 0;

/*!
*@var static VirtualPackageHeader vph
*@brief Заголовок пакета виртуального канала
*/
static VirtualPackageHeader vph;

/*!
*@var static uint8_t* p_vph
*@brief Указатель на заголовок пакета виртуального канала
*/
static uint8_t *p_vph = (uint8_t *)&vph;

/*!
*@var static uint8_t type_of_pkg
*@brief Тип пакета
*/
static volatile uint8_t type_of_pkg = VAR_GROUP_1;
// static volatile uint8_t type_of_pkg = VAR_GROUP_2;
// static volatile uint8_t type_of_pkg = VAR_GROUP_3;

/*!
*@var uint16_t RbGetP
*@brief Указатель на выбираемый из буфера в канал байт
*/
uint16_t RbGetP;
/*!
*@var uint16_t RbPutP
*@brief указатель на помещаемый из приемника UART в буфер байт
*/
uint16_t RbPutP;
/*!
*@var uint8_t RcvBuffer[UART_RXD_SIZE]
*@brief Приемный буфер
*/
uint8_t RcvBuffer[UART_RXD_SIZE];

/*!
*@var static uint8_t curr_dev
*@brief Номер текущей структуры переменных мониторинга, выдаваемые в ИДК/КПА
*/
/*static */uint8_t curr_dev = MODULE_ID_1;
// uint8_t curr_dev = MODULE_ID_2;
// uint8_t curr_dev = MODULE_ID_3;


/*!
*@var static bool flag_ram_change
*@brief Флаг завершения записи СНП в ОЗУ
*/
static bool flag_ram_change = false;

//test
/*------------------------------------------------------------------------------
        Локальные функции (объявления)
-------------------------------------------------------------------------------*/
#if !defined(WorkMode) && defined(unityTest)
	#define statical
#else
	#define statical static
#endif

/*!
*@fn static uint16_t create_idc_frame(uint8_t volatile* virtual_addr, uint8_t* real_addr, uint16_t virtual_len, uint16_t real_len, uint16_t virt_pack_num)
*@brief Функция создания пакета с данными для ИДК
*/
statical 
uint16_t create_idc_frame(uint8_t volatile* virtual_addr, uint8_t* real_addr, uint16_t virtual_len, uint16_t real_len, uint16_t virt_pack_num);

/*!
*@fn static void send_data_to_idc(uint16_t virt_pack_num)
*@brief Функция выдачи данных в ИДК
*/
static void send_data_to_idc(uint16_t virt_pack_num);
/*!
*@fn static uint16_t do_command(uint16_t RbGetP, uint16_t lenght)
*@brief Функция обработки команды от КПА
*/
static uint16_t do_command(uint16_t RbGetP, uint16_t lenght);
/*!
*@fn static void set_virtual_channel_data(uint8_t *src, uint16_t lenght, uint8_t pack_type)
*@brief Функция формирования данных виртуального канала ИДК
*/
static void set_virtual_channel_data(uint8_t *src, uint16_t lenght, uint8_t pack_type);

/*!
*@fn static void write_ram(uint16_t start, uint8_t *data, uint16_t lenght)
*@brief Функция записи полученных от ИДК данных в ОЗУ
*/
static void write_ram(uint16_t start, uint8_t *data, uint16_t lenght);

/* extern void SetFLASHstate_RAM_CHANGE(void); */


/*------------------------------------------------------------------------------
        Локальные функции (реализация)
-------------------------------------------------------------------------------*/
/*!
*@fn static uint16_t create_idc_frame(uint8_t volatile* virtual_addr, uint8_t* real_addr, uint16_t virtual_len, uint16_t real_len, uint16_t virt_pack_num)
*@param uint8_t volatile* virtual_addr - адрес структуры виртуального канала
*@param uint8_t* real_addr - адрес структуры пакета основного канала
*@param uint16_t virtual_len - размер структуры пакета виртуального канала
*@param uint16_t real_len - размер структуры пакета основного канала
*@param uint16_t virt_pack_num - номер блока пакета виртуального канала (начиная с 0)
*@return Номер следующего блока пакета виртуального канала (либо следующий,либо 0, если пакет передан)
*/
statical
uint16_t create_idc_frame(uint8_t volatile* virtual_addr, uint8_t* real_addr, uint16_t virtual_len, uint16_t real_len, uint16_t virt_pack_num)
{
	/* Локальные переменные */
	uint16_t i;  //type was uint8, for 16-bit size must be uint16, else did not cacl CRC
	uint16_t CRC;
	uint8_t* real_virtual;

	/* Сформируем заголовок пакета данных */
	*real_addr = START_OF_FRAME;
	*(real_addr + 1) = TYPE_OF_FRAME;
	//*(uint16_t*)(real_addr + 2) = (real_len - 6); //6: one byte start frame, one byte type of frame, two bytes size of frame, two bytes CRC
	//because pointer from uint16 get hard fault on 2-nd str
	*(real_addr + 2) = (real_len - 6) & 0xFF;   //lower byte of size
	*(real_addr + 3) = (real_len - 6) >> 8;			//higher byte of size
	/* Начиная с этой позиции данные из в.п. помещаем в пакет для передачи */
	virtual_addr += 6 * (virt_pack_num - 1); 
	/* Идем на начало виртуального канала в пакете для передачи */
	real_virtual = real_addr + real_len - 8;		/* на конец пакета - (6 байт -виртуальный + 2 -CRC) */

	if (!virt_pack_num)
	{
//		memcpy(real_virtual, p_vph, 6);				/* заголовок */
		mem_cpy(real_virtual, p_vph, 6);
	}
	else
	{
		/* Записываем виртуальные данные */
		for (i = 0; i < 6; i++)
		{
			/* В.п. кончился - пишем FF */
			if ((virt_pack_num * 6 - 6 + i) >= virtual_len)
			{
				*(real_virtual++) = 0xFF;
			}
			else
			{
				*(real_virtual++) = *(virtual_addr++);
			}
		}
	}

	/* Начинаем считать CRC */
	CRC = 0;
	for (i = 4; i < real_len - 2; i++) //start calc CRC from begin of data in str, skip 4 bytes header+size
	{
		CRC += *(real_addr + i);
	}
	*(real_addr + real_len - 2) = CRC & 0xFF;		/* поместили СКС в поле структуры */
	*(real_addr + real_len - 1) = CRC >> 8;

	/* 6 - длина виртуальных данных во всем пакете */
	if (virt_pack_num * 6 >= virtual_len)
	{
		return 0;
	}
	/* заменить на (VirNum * 6 + 6) >= VirLen */
	else
	{
		virt_pack_num++;
		/* Возвращаем: номер следующего блока виртуального пакета */
		return virt_pack_num;
	}
}

/*!
*@fn static void send_data_to_idc(uint16_t virt_pack_num)
*@param нет
*@return нет
*/
static void send_data_to_idc(uint16_t virt_pack_num)
{
	/* Локальные переменные */
	uint8_t* data_to_send;
	uint16_t lenght_data_to_send;  //uint16 for two-byte size of str

	if (type_of_pkg == VAR_GROUP_1)
	{
		data_to_send = bitoutbuf;
		
		/* monstr */
		BitIoRewind();
		PackBlock((void *)&var_grp_1.time_ticks, (MONSTR_t *)monstr1);
		BitOutFlush();
		
		/* We need added 11 bytes that contain preambule, virt. data, CRC...
		 * But we start count @bitoutlen from 3 and now we add only 8 bytes
		 */
		lenght_data_to_send = bitoutlen + 8; 
	}
	if (type_of_pkg == VAR_GROUP_2)
	{	
		/* monstr */		
		data_to_send = bitoutbuf;
		
		BitIoRewind();
		PackBlock((void *)&var_grp_1.time_ticks, (MONSTR_t *)monstr2);
		BitOutFlush();

		lenght_data_to_send = bitoutlen + 8; 
	}
	if (type_of_pkg == VAR_GROUP_3)
	{
		/* monstr */		
		data_to_send = bitoutbuf;
		
		BitIoRewind();
		PackBlock((void *)&var_grp_1.time_ticks, (MONSTR_t *)monstr3);
		BitOutFlush();

		lenght_data_to_send = bitoutlen + 8; 
	}
	
//	if (type_of_pkg == GET_FALUTS)
//	{
//		size = GetSize(Ring_tst);
//		//читаем поочередно кадр

//		volatile uint32_t check = GetDataFromRing(Ring_tst,i);
//		w25n01gv_data_read(0, (uint8_t*)buff, sizeof(BufferCYCLE[0].monstr_FLASH), 1);
//		i++;
//		if(i>=size)
//			i=0;   //?
//		//и копируем для вывода в ИДК
//		mem_cpy((MONSTR_t *)monstr1, (MONSTR_t *)buff, sizeof(BufferCYCLE[0].monstr_FLASH));
//		
//		/* monstr */		
//		data_to_send = bitoutbuf;
//		
//		BitIoRewind();
//		PackBlock((void *)&var_grp_1.time_ticks, (MONSTR_t *)monstr1);
//		BitOutFlush();

//		lenght_data_to_send = bitoutlen + 8; 
//	}
	
	virtual_pack_number = create_idc_frame(vph.DataAddress, data_to_send, vph.lenght, lenght_data_to_send, virt_pack_num);
	while(xmt_block(data_to_send, lenght_data_to_send));
}

/*!
*@fn static uint16_t do_command(uint16_t RbGetP, uint16_t lenght)
*@param uint16_t RbGetP - указатель на выбираемый байт данных в кольцевом буфере
*@param uint16_t lenght - длина данных в кольцевом буфере
*@return Код команды
*/
static uint16_t do_command(uint16_t RbGetP, uint16_t lenght)
{
	/*Локальные переменные */
	uint8_t code;
//	uint16_t crc;
	PSS_ptr_t current_pss;
	
	RbGetP++;
	code = RcvBuffer[RbGetP & (UART_RXD_SIZE - 1)];
	/* на блок данных - 1 добавили для команды */
	RbGetP += 3;

	switch (code)
	{
		case 0x01:
			break;
		case 0x02:
			break;
		case 0x03:
			break;
		
		/* Запись параметров в ОЗУ */
		case SET_RAM_DATA:
			get_ptr_to_pss(curr_dev, &current_pss);
			if(Check_SNP_size(RbGetP, current_pss.lenght))
				write_ram(RbGetP, current_pss.data, current_pss.lenght);
			break;
		/* Запись параметров во Flash и ОЗУ */
		case SET_ROM_DATA:
			get_ptr_to_pss(curr_dev, &current_pss);
			if(Check_SNP_size(RbGetP, current_pss.lenght))
			{
				write_ram(RbGetP, current_pss.data, current_pss.lenght);
				while (!flag_ram_change);
				/* flash_write_data(0x0, 0x0, addr, current_pss.data, current_pss.lenght); */
				write_pss_to_flash();
			}
			break;
		
		/* Выдача параметров настойки из ОЗУ */
		case GET_RAM_DATA:
			get_ptr_to_pss(curr_dev, &current_pss);
			set_virtual_channel_data(current_pss.data, current_pss.lenght, GET_RAM_DATA);
			virtual_pack_number = 0;
			break;
		/* Выдача параметров настройки из Flash */
		case GET_ROM_DATA:
			get_ptr_to_pss(curr_dev, &current_pss);
			/* На время чтения данных из flash-памяти
			приходится отключить все прерывания */
			__disable_irq();
			read_pss_from_flash(curr_dev);
			/* Восттанавливаю прерывания после чтения flash-памяти прерывания */
			__enable_irq();
			set_virtual_channel_data(current_pss.data, current_pss.lenght, GET_ROM_DATA);
			virtual_pack_number = 0;
			break;
		
		/* Выдача идентификатора устройства из ОЗУ */
		/* Пока так, потом переделаю на идентификаторы различных модулей */
		case GET_DEV_NUMBER:
			set_virtual_channel_data((uint8_t *)&devices[curr_dev], sizeof(devices[curr_dev]), GET_DEV_NUMBER);
			virtual_pack_number = 0;
			
			break;
		
		/* Выбор стуктур переменных */
		case VAR_GROUP_1:
			curr_dev = MODULE_ID_1;
			type_of_pkg = VAR_GROUP_1;
			virtual_pack_number = 0;
			break;
		case VAR_GROUP_2:
			curr_dev = MODULE_ID_2;
			type_of_pkg = VAR_GROUP_2;
			virtual_pack_number = 0;
			break;
		case VAR_GROUP_3:
			curr_dev = MODULE_ID_3;
			type_of_pkg = VAR_GROUP_3;
			virtual_pack_number = 0;
			break;
		case GET_FALUTS:
			curr_dev = MODULE_ID_1;
			type_of_pkg = GET_FALUTS;
			virtual_pack_number = 0;
			break;
			
		default:
			set_virtual_channel_data((uint8_t *)&devices[curr_dev], sizeof(devices[curr_dev]), GET_DEV_NUMBER);
			virtual_pack_number = 0;
			break;
	}	/* End of switch */
	/* Возврашаю код команды */
	return code;
}

/*!
*@fn static void write_ram(uint16_t start, uint8_t *data, uint16_t lenght)
*@param uint32_t start - начало записываемых данных
*@param uint8_t *data - указатель на область ОЗУ, в которую записываются данные
*@param uint16_t lenght - длина записываемых данных
*@return нет
*/
static void write_ram(uint16_t start, uint8_t *data, uint16_t lenght)
{
	/*Локальные переменные */
	uint16_t i;
	/* Здесь надо задавать адрес записываемой СНП */
	uint8_t *pointer = data;		/* Адрес начала СНП */
	
	for (i = 0; i < lenght; i++)
	{
		*pointer = RcvBuffer[(start + i) & (UART_RXD_SIZE - 1)];
		/* Через memcpy нельзя, т.к. приемный буфер
		циклический. Могут быть проблемы! */
		*pointer++;
	}
	flag_ram_change = true;
	SetFLASHstate_RAM_CHANGE();
	//check_snp_bxd();
}

/*!
*@fn static void set_virtual_channel_data(uint8_t *src, uint16_t lenght, uint8_t package_type)
*@param uint8_t *src - указатель на передаваемые на виртуальном канале данные
*@param uint16_t lenght - размер передаваемых данных
*@param uint8_t package_type - тип передаваемого на виртуальном канале пакета
*@return нет
*/
static void set_virtual_channel_data(uint8_t *src, uint16_t lenght, uint8_t package_type)
{
	vph.header = START_OF_FRAME;
	vph.type = package_type;
	vph.lenght = lenght;
	vph.DataAddress = src;
	vph.CRC=crc16(0, vph.DataAddress, vph.lenght);
}

/*------------------------------------------------------------------------------
        Глобальные функции
-------------------------------------------------------------------------------*/
#ifndef ENGINE_MODEL_CPP_ // при работе в рамках C++ модели двигателя эти определения исключаются
/*!
*@fn __weak void get_ptr_to_pss(DEVICE_NUMBERS curr_dev, PSS_ptr_t *pss_ptr)
*В этом модуле функция получения указателей на структуры настроечных параметров \n
*является только заглушкой. Реализация этой функции зависит \n
*от конкретного модуля и располагается в другом месте \n
*На работу ПО не сказывается, т.к. используется __weak \n
*Если функция получения указателей нигде более не реализована, то \n
*эта функция-заглушка позволит без ошибок скомпилировать ПО \n
*Кроме этого, использование __weak позволяет использовать \n
*модуль обмена с КПА в любом ПО без необходимости подключения \n
*лишних заголовочных файлов, зависящих от реализуемого ПО \n \n
*
*@param DEVICE_NUMBERS curr_dev - номер текущего устройства, СНП которого читается
*@param PSS_ptr_t *pss_ptr - указатель, через который возвращается прочитанная СНП
*@return нет
*/
__weak void get_ptr_to_pss(DEVICE_NUMBERS curr_dev, PSS_ptr_t *pss_ptr)
{

}

/*!
*@fn __weak void read_pss_from_flash(void)
*В этом модуле функция чтения структуры настроечных параметров \n
*является только заглушкой. Реализация этой функции зависит \n
*от конкретного модуля и располагается в другом месте \n
*На работу ПО не сказывается, т.к. используется __weak \n
*Если функция чтения нигде более не реализована, то \n
*эта функция-заглушка позволит без ошибок скомпилировать ПО \n
*Кроме этого, использование __weak позволяет использовать \n
*модуль обмена с КПА в любом ПО без необходимости подключения \n
*лишних заголовочных файлов, зависящих от реализуемого ПО \n \n
*
*@param
*@return
*/
__weak void read_pss_from_flash(DEVICE_NUMBERS id)
{
	
}

/*!
*@fn __weak void write_pss_to_flash(void)
*В этом модуле функция записи структуры настроечных параметров \n
*является только заглушкой. Реализация этой функции зависит \n
*от конкретного модуля и располагается в другом месте \n
*На работу ПО не сказывается, т.к. используется __weak \n
*Если функция записи нигде более не реализована, то \n
*эта функция-заглушка позволит без ошибок скомпилировать ПО \n
*Кроме этого, использование __weak позволяет использовать \n
*модуль обмена с КПА в любом ПО без необходимости подключения \n
*лишних заголовочных файлов, зависящих от реализуемого ПО \n \n
*
*@param
*@return
*/
__weak void write_pss_to_flash(void)
{
	
}
#endif

/*!
*@fn void flush_rxd_buffer(void)
*@param нет
*@return нет
*/
void flush_rxd_buffer(void)
{
	RbGetP = RbPutP = 0;
}

/*!
*@fn uint16_t get_rxd_bytes(void)
*@param нет
*@return количество принятых байтов
*/
uint16_t get_rxd_bytes(void)
{
	return RbPutP - RbGetP;
}
/*!
*@fn uint16_t find_command(void)
*Производит поиск команды от ИДК в кольцевом приемном \n
*буфере данных с последующей ее обработкой \n \n
*
*
*@param нет
*@return нет
*/
uint16_t find_command(void)
{
	/*Локальные переменные*/
	uint16_t GetP;
	uint8_t data;						/* байт из буфера */
	uint16_t available;
	uint16_t Lenght;
	uint16_t CRC = 0;
	uint16_t PackCRC = 0;
	uint16_t i;
	uint16_t cmd;
	static bool ddd=false;
	
	for (;;)
	{
		/* Доступно в буфере */
		available = (RbPutP - RbGetP) & (UART_RXD_SIZE - 1);
		if (available == 0)
		{
			/* Нет данных (не пришел пакет), по умолчанию шлем ID*/
			if(!ddd)
			{
				set_virtual_channel_data((uint8_t *)&devices[curr_dev], sizeof(devices[curr_dev]), GET_DEV_NUMBER);
				virtual_pack_number = 0;
				ddd = true;
			}
			return 0;
		}
	//data_states_table.IDK_OK = true;
		/* Байт из буфера */
		data = RBUF(RbGetP);
		/* Заголовок */
		if (data == START_OF_FRAME)
		{
			if (available < 4)
			{
				/* Еще нет длины */
				return 0;
			}
			_SET_MSG_(IDK_flag);
			GetP = RbGetP + 1;
			/* На команду */
			data = RBUF(GetP);
			if (data == 0x51)
			{
				if (available < 7)
				{
					return 0;
				}
				RbGetP += 7;
				/* При перезагрузке сюда уже не попадаем */
				continue;
			}
			if (data == 0x80)
			{
				if (available < 8)
					return 0;
				data_states_table.send_logs = true;
				data_states_table.logs_lenght = RBUF(GetP+3)|(RBUF(GetP+4)<<8);
				//if (RBUF(GetP + 1) == 0x02
				RbGetP += 8;
			}
			if (data == 0x81)
			{
				if (available < 8)
					return 0;
				data_states_table.erase_logs = true;
			}
			if (data == 0x90)
			{
				if (available < 8)
				{
					return 0;
				}
				
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x05
					&& RBUF(GetP + 4) == 0x05 && RBUF(GetP + 5) == 0x0A && RBUF(GetP + 6) == 0x0)
				{
					bxd_next_global_state = BXD_GLOBAL_STATE_SEND_NRB;
					CommandFromIDK = true;
				}
				
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x06
					&& RBUF(GetP + 4) == 0x06 && RBUF(GetP + 5) == 0x0c && RBUF(GetP + 6) == 0x0)
				{
					bxd_next_global_state = BXD_GLOBAL_STATE_GET_NRB;
					CommandFromIDK = true;
				}
				//0x55 0x90 0x02 0x0 0x01 0x01 0x02 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x01
					&& RBUF(GetP + 4) == 0x01 && RBUF(GetP + 5) == 0x02 && RBUF(GetP + 6) == 0x0)
				{
					data_states_table.AVRR_IDC = data_states_table.AVRR_IDC ^ 1;
					if (data_states_table.AVRR_test_flag)
					{
						data_states_table.AVRR_test_cnt = 0;
						data_states_table.AVRR_test_flag = false;
					}
				}
				//0x55 0x90 0x02 0x0 0x02 0x02 0x04 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x02
					&& RBUF(GetP + 4) == 0x02 && RBUF(GetP + 5) == 0x04 && RBUF(GetP + 6) == 0x0)
				{
					data_states_table.PPZ_IDC = data_states_table.PPZ_IDC ^ 1;
				}
				//0x55 0x90 0x02 0x0 0x04 0x04 0x08 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x03
					&& RBUF(GetP + 4) == 0x03 && RBUF(GetP + 5) == 0x06 && RBUF(GetP + 6) == 0x0)
				{
					bxd_next_global_state = BXD_GLOBAL_STATE_SEND_SNP;
				}
				//0x55 0x90 0x02 0x0 0x08 0x08 0x0A 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x04
					&& RBUF(GetP + 4) == 0x04 && RBUF(GetP + 5) == 0x08 && RBUF(GetP + 6) == 0x0)
				{
					bxd_next_global_state = BXD_GLOBAL_STATE_GET_SNP;
				}
				//0x55 0x90 0x02 0x0 0x0A 0x0A 0x0C 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x0A
					&& RBUF(GetP + 4) == 0x0A && RBUF(GetP + 5) == 0x14 && RBUF(GetP + 6) == 0x0)
				{
					data_states_table.Mkr_off = data_states_table.Mkr_off ^ 1;
				}
				//0x55 0x90 0x02 0x0 0x0C 0x0C 0x18 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x0C
					&& RBUF(GetP + 4) == 0x0C && RBUF(GetP + 5) == 0x18 && RBUF(GetP + 6) == 0x0)
				{
					data_states_table.K01_Imitation = data_states_table.K01_Imitation ^ 1;
					if(!data_states_table.K01_Imitation && _GET_MSG_(K_01))
							_CLR_MSG_(K_01);					
				}				
				//0x55 0x90 0x02 0x0 0x0B 0x0B 0x16 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x0B
					&& RBUF(GetP + 4) == 0x0B && RBUF(GetP + 5) == 0x16 && RBUF(GetP + 6) == 0x0)
				{
					//reserve_table_2.signals.AGFT_control = reserve_table_2.signals.AGFT_control ^ 1;
					data_states_table.CheckAZST = data_states_table.CheckAZST ^ 1;
				}
				//0x55 0x90 0x02 0x0 0x0D 0x0D 0x1A 0x0
				if (RBUF(GetP + 1) == 0x02 && RBUF(GetP + 2) == 0x0 && RBUF(GetP + 3) == 0x0D
					&& RBUF(GetP + 4) == 0x0D && RBUF(GetP + 5) == 0x1A && RBUF(GetP + 6) == 0x0)
				{
					DEFVAL.IsStend = DEFVAL.IsStend ^ 1;
				}					
				RbGetP += 8;
			}
			GetP++;
			Lenght = RBUF(GetP) + (RBUF(GetP + 1) << 8);
			if(CheckCommand(data, Lenght))
			{
				RbGetP++;
				continue;
			}
			/* Может весь пакет еще не доступен */
			if (available < Lenght + 5)
				return 0;      
			/* Заголовочные 4 байта правильные. Проверяем кoнтрольную сумму */
			GetP += 2;
			/* Не простая команда */
			if (Lenght > 0)
			{
				for (i = 0; i < Lenght; i++) 
				{
					data = RBUF(GetP);
					/* Простое суммирование */
					CRC += data;
					GetP++;          
				}
			}
			else
			{
				/* Для команды СRС = 0 */
				CRC = 0;
			}
			/* Указатель остался на первом (младшем) байте контрольной суммы */
			PackCRC = RBUF(GetP) + (RBUF(GetP + 1) << 8);
			/* Сравниваем с истиной CRC */
			if (CRC != PackCRC)
			{
				RbGetP++;
				continue;
			}
			/* Обработка принятых данных */
			cmd = do_command(RbGetP, Lenght);
			/* Указатель - после считанного пакета */
			RbGetP = GetP + 1;
			return cmd;
		}	/*End of if (data == START_OF_FRAME) */
		RbGetP++;
	}		/* End of for (;;) */
	return 0;
}

/*!
*@fn void start_send_data_idc(void)
*@param нет
*@return нет
*/
void start_send_data_idc(void)
{
	send_data_to_idc(virtual_pack_number);
}

/*!
*@fn void uart_post_irq_handler(uint8_t data)
*@param uint8_t data - принятый по UART байт данных
*@return
*/
void uart_post_irq_handler(uint8_t data)
{
	if ((RbPutP - RbGetP) < UART_RXD_SIZE)
	{
		RcvBuffer[RbPutP & (UART_RXD_SIZE -1 )] = data;
		RbPutP += 1;
	}
}

/*!
*@fn __weak void SetFLASHstate_RAM_CHANGE(void)
*Эта функция реализуется в модели управления двигателем \n
*Здесь это просто заглушка \n \n
*
*@param нет
*@return нет
*/
#ifndef ENGINE_MODEL_CPP_ // при работе в рамках C++ модели двигателя это определение исключается
__weak void SetFLASHstate_RAM_CHANGE(void)
{
	
}
#endif
//#endif

//Проверка команд от ИДК
int CheckCommand(uint16_t Cmd, uint16_t Len)
{
  int i;								// {CMD, Len}
	if(Cmd == 0x53 || Cmd == 0x54)
		return 0;
  for(i=0;i<_COMMANDS_COUNT_;i++)
  {	
		if(Cmd==CMDs[i].cmd)
		{
			if(Len==CMDs[i].len) return 0;
			else return 1;
		}
  }
  return 1;  
}

//Проверка размера записываемой СНП
uint8_t Check_SNP_size(uint16_t buf, uint16_t lenght_snp)
{
	uint16_t size_from_idc;
	
	//Вычитаем, поскольку при вызове функции указатель уже сразу сдвинут на данные
	size_from_idc = RBUF(buf - 2) + (RBUF(buf - 1) << 8);
	
	if(size_from_idc == lenght_snp)
		return 1;
	else
		return 0;
}
