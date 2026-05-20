/*!
*@file idc.h
*@brief Файл объявлений функций обмена с ИДК
*/

#ifndef _IDC_H_
#define	_IDC_H_

/*-----------------------------------------------------------------------------
            Заголовки
-----------------------------------------------------------------------------*/
#include "../../board.h"
#include "idc_defs.h"
#include "idc_structs.h"

/*
Файл idc_structs.h должен быть определен для конкретного модуля,
для которого реализуется обмен с КПА. В этом файле определяются
2 структуры отображаемых переменных
*/

/*-----------------------------------------------------------------------------
            Типы
-----------------------------------------------------------------------------*/
typedef struct CMD_description_taG
{
	uint16_t  cmd;
	uint16_t  len;
} CMD_description;

/*-----------------------------------------------------------------------------
            Определения
-----------------------------------------------------------------------------*/
/*!
*@def START_OF_FRAME
*@brief Маркер начала пакета
*/
#define START_OF_FRAME			0x55
/*!
*@def TYPE_OF_FRAME
*@brief Тип пакета при передаче из БАРК в ИДК
*/
#define	TYPE_OF_FRAME			0xCC

/*!
*@def RXD_SIZE
*@brief Размер приемного буфера
*/
// #define	RXD_SIZE				/*3072*/4096

/*!
*@def COEF_SCALE_1000
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_1000			1000
/*!
*@def COEF_SCALE_100
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_100			100
/*!
*@def COEF_SCALE_10
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_10			10



/*-----------------------------------------------------------------------------
            Глобальные переменные
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
            Внешние переменные
-----------------------------------------------------------------------------*/
/*!
*@var extern VersionDevice devices[2]
*@brief Список структур отображаемых в КПА переменных
*/
extern VersionDevice devices[3];

/*!
*@var extern volatile VariableGroup1 var_grp_1
*@brief 1-я группа пременных мониторинга
*/
extern volatile VariableGroup1 var_grp_1;
/*!
*@var extern volatile VariableGroup2 var_grp_2
*@brief 2-я группа пременных мониторинга
*/
extern volatile VariableGroup2 var_grp_2;


/*-----------------------------------------------------------------------------
            Глобальные функции
-----------------------------------------------------------------------------*/
/*!
*@fn void init_idc_data(void)
*@brief Функция-заглушка инициализации данных для обмена с КПА
*/
void init_idc_data(void);
/*!
*@fn void get_ptr_to_pss(DEVICE_NUMBERS curr_dev, PSS_ptr_t *pss_ptr)
*@brief Функция-заглушка получения указателя на СНП
*/
void get_ptr_to_pss(DEVICE_NUMBERS curr_dev, PSS_ptr_t *pss_ptr);
/*!
*@fn void read_pss_from_flash(void)
*@brief Функция-заглушка чтения СНП из внешней флеш-памяти
*/
void read_pss_from_flash(DEVICE_NUMBERS id);
/*!
*@fn void write_pss_to_flash(void)
*@brief Функция-заглушка записи СНП во внешнюю флеш-память
*/
void write_pss_to_flash(void);

/*!
*@fn void flush_rxd_buffer(void)
*@brief Функция очистки приемного буфера
*/
void flush_rxd_buffer(void);
/*!
*@fn uint16_t get_rxd_bytes(void)
*@brief Функция выдачи количества принятых байтов
*/
uint16_t get_rxd_bytes(void);
/*!
*@fn uint16_t find_command(void)
*@brief Функция поиска команды КПА
*/
uint16_t find_command(void);
/*!
*@fn void start_send_data_idc(void)
*@brief Функция запуска выдачи данных в ИДК
*/
void start_send_data_idc(void);
/*!
*@fn void uart_post_irq_handler(uint8_t data)
*@brief Функция пост-обработки прерывания UART2
*/
void uart_post_irq_handler(uint8_t data);
/*!
*@fn void SetFLASHstate_RAM_CHANGE(void)
*@brief Функция обновления данных после записи СНП в RAM/EEPROM
*/
void SetFLASHstate_RAM_CHANGE(void);

int CheckCommand(uint16_t Cmd, uint16_t Len);

uint8_t Check_SNP_size(uint16_t buf, uint16_t lenght_snp);

#endif			/* End of _IDC_H_ */
