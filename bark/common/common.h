
#ifndef	_COMMON_H_
#define _COMMON_H_

/*-----------------------------------------------------------------------------
            Заголовки
-----------------------------------------------------------------------------*/
#include <math.h>
// #include <string.h>

#include "typedefs.h"

/*-----------------------------------------------------------------------------
            Типы
-----------------------------------------------------------------------------*/
/* int16 нужен для совместимости с ПО верхнего уровня, т.к. там используется такй тип */
typedef signed short int int16;

/*!
*@union TVariant64
*@brief Объединение для побайтной работы с восьмибайтным числом
*/
typedef volatile union _TVariant64
{
	uint64_t qword;							/*Доступ к 8-байтному слову*/
	uint32_t dword[2];						/*Доступ к отдельным двойным словам*/
	float ffloat[2];						/*Доступ к вещественному значению*/
	uint8_t byte[8];						/*Доступ к отдельным байтам*/
	uint16_t word[4];						/*Доступ к отдельным словам*/
}TVariant64;

/*!
*@union TVariant32
*@brief Объединение для побайтной работы с четырехбайтным числом
*/
typedef volatile union _TVariant32
{
	uint32_t dword;							/*Доступ к двойному слову*/
	float ffloat;							/*Доступ к вещественному значению (4 байта)*/
	uint8_t byte[4];						/*Доступ к отдельным байтам*/
	uint16_t word[2];						/*Доступ к отдельным словам*/
}TVariant32;

/*!
*@union TVariant16
*@brief Объединение для побайтной работы с двухбайтным числом
*/
typedef volatile union _TVariant16
{
	int16_t	signWord;						/*Доступ сразу к 2-м байтам (со знаком)*/
	uint16_t word;							/*Доступ сразу к 2-м байтам (без знака)*/
	uint8_t byte[2];						/*Доступ к отдельным байтам	(без знака)*/
}TVariant16;
//#warning "changed for bits seq: lower two bits - short and break, higher two bits - faults from GTC"
/*!
*@enum FreqStatus
*@brief Статусы каналов измерения частот (ГДК и контроль цепей датчиков оборотов)
*/
typedef enum _FreqStatus
{
	FREQ_CH_ERR_OK=0,						/*Ошибок нет*/
	FREQ_CH_ERR_BREAK,						/*ВСК зафиксировал обрыв цепи датчика оборотов или отказ датчика оборотов*/
	FREQ_CH_ERR_SHORT,						/*ВСК зафиксировал К/З цепи датчика оборотов или отказ по перекрестному контролю*/
//	FREQ_CH_ERR_MAX_LIM,					/*Достигнут верхний предел по ГДК*/
//	FREQ_CH_ERR_MIN_LIM,					/*Достигнут нижний предел по ГДК*/
	FREQ_CH_ERR_MAX_ACCELL,					/*Скорость изменения параметра выше допуска ГДК*/
//	FREQ_CH_ERR_MAX_SLOW,					/*Скорость изменения параметра ниже допуска ГДК*/
	FREQ_CH_ERR_PARAM						/*Ошибка получения/обработки данных*/
}FreqStatus;

/*!
*@enum ThermoStatus
*@brief Статусы каналов измерения температур (похоже, только ГДК)
*/
typedef enum _ThermoStatus
{
	THERMO_CH_ERR_OK=0,						/*Ошибок нет*/
	THERMO_CH_ERR_BREAK,					/*ВСК зафиксировал обрыв в цепи питания*/
	THERMO_CH_ERR_SHORT,					/*ВСК зафиксировал К/З в цепи питания*/
//	THERMO_CH_ERR_MAX_LIM=3,				/*Достигнут верхний предел по ГДК*/
//	THERMO_CH_ERR_MIN_LIM,					/*Достигнут нижний предел по ГДК*/
//	THERMO_CH_ERR_MAX_ACCELL,				/*Скорость изменения параметра выше допуска ГДК*/
//	THERMO_CH_ERR_MAX_SLOW,					/*Скорость изменения параметра ниже допуска ГДК*/
	THERMO_CH_ERR_PARAM						/*Ошибка получения/обработки данных*/
}ThermoStatus;

/*!
*@enum PressStatus
*@brief Статусы каналов измерения давления (ГДК и контроль цепей питания РАПРИЗа)
*/
typedef enum _PressStatus
{
	PRESS_CH_ERR_OK=0,						/*Ошибок нет*/
	PRESS_CH_ERR_BREAK,						/*ВСК зафиксировал обрыв в цепи датчика (сигнал О10?)*/
	PRESS_CH_ERR_SHORT,						/*ВСК зафиксировал К/З в цепи датчика (сигнал О10?)*/
//	PRESS_CH_ERR_MAX_LIM,					/*Достигнут верхний предел по ГДК (сигнал О10?)*/
//	PRESS_CH_ERR_MIN_LIM,					/*Достигнут нижний предел по ГДК, сигнал К51*/
//	PRESS_CH_ERR_MAX_ACCELL,				/*Скорость изменения параметра выше допуска ГДК (сигнал О10?)*/
//	PRESS_CH_ERR_MAX_SLOW,					/*Скорость изменения параметра ниже допуска ГДК (сигнал О10?)*/
	PRESS_CH_ERR_PARAM 						/*Ошибка получения/обработки данных*/
}PressStatus;

/*!
*@enum PowerStatus
*@brief Статусы питания внешних цепей (дискретные сигналы, ЭСК и т.п.)
*/
typedef enum _PowerStatus
{
	POWER_ERR_OK=0,							/*Ошибок нет*/
	POWER_ERR_BREAK,						/*ВСК зафиксировал обрыв в цепи питания*/
	POWER_ERR_SHORT							/*ВСК зафиксировал К/З в цепи питания*/
}PowerStatus;

/*!
*@enum USupplyStatus
*@brief Статусы контроля напряжения питания
*/
typedef enum _USupplyStatus
{
	U_SUPPLY_CH_ERR_OK=0,					/*Напряжение питания в норме*/
	U_SUPPLY_CH_ERR_MAX_LIM,				/*Напряжение питания выше нормы*/
	U_SUPPLY_CH_ERR_MIN_LIM,				/*Напряжение питания ниже нормы*/
//	U_SUPPLY_CH_ERR_PARAM					/*Ошибка получения/обработки данных*/
}USupplyStatus;

/*!
*@enum LogicalSignalsStates
*@brief Состояние сигналов
*/
typedef enum _LogicalSignalsStates
{
	/*SIGNAL_OFF=false,
	SIGNAL_ON=true*/
	SIGNAL_OFF=0,
	SIGNAL_ON,
	SIGNAL_BREAK,
	SIGNAL_SHORT
}LogicalSignalsStates;

/*!
*@enum DiscretesStates
*@brief Состояния дискретных сигналов и команд
*/
typedef enum _DiscretesStates
{
	D_SIGNAL_OFF=0x0,						/*Сигнал/команда выключен*/
	D_SIGNAL_ON,							/*Сигнал/команда включен*/
	D_SIGNAL_BREAK=0x2,						/*Обрыв в цепи сигнала/команды*/
	D_SIGNAL_SHORT=0x4,							/*К/З в цепи сигнала/команды*/
	D_SIGNAL_CTRL_BREAK=0x8,					/*Обрыв в цепи контроля сигнала/команды*/
	D_SIGNAL_CTRL_SHORT=0xC						/*К/З в цепи контроля сигнала/команды*/
}DiscretesStates;

/*!
*@enum CurrentChannelStatus
*@brief Состояния токового канала
*/
typedef enum _CurrentChannelStatus
{
	CURR_CH_ERR_OK=0,						/*Ошибок нет*/
	CURR_CH_ERR_BREAK,						/*ВСК зафиксировал обрыв в цепи датчика (сигнал О10?)*/
	CURR_CH_ERR_SHORT,						/*ВСК зафиксировал К/З в цепи датчика (сигнал О10?)*/
//	CURR_CH_ERR_MAX_LIM,					/*Достигнут верхний предел по ГДК (сигнал О10?)*/
//	CURR_CH_ERR_MIN_LIM,					/*Достигнут нижний предел по ГДК, сигнал К51*/
//	CURR_CH_ERR_MAX_ACCELL,					/*Скорость изменения параметра выше допуска ГДК (сигнал О10?)*/
//	CURR_CH_ERR_MAX_SLOW,					/*Скорость изменения параметра ниже допуска ГДК (сигнал О10?)*/
	CURR_CH_ERR_PARAM						/*Ошибка получения/обработки данных*/
}CurrentChannelStatus;

/*!
*@enum MovementsStatus
*@brief Состояния измерения углов и перемещений
*/
typedef enum _MovementsStatus
{
	MOV_CH_ERR_OK=0,						/*Ошибок нет*/
	MOV_CH_ERR_BREAK,						/*ВСК зафиксировал обрыв в цепи датчика (сигнал О10?)*/
	MOV_CH_ERR_SHORT,						/*ВСК зафиксировал К/З в цепи датчика (сигнал О10?)*/
//	MOV_CH_ERR_MAX_LIM,						/*Достигнут верхний предел по ГДК (сигнал О10?)*/
//	MOV_CH_ERR_MIN_LIM,						/*Достигнут нижний предел по ГДК, сигнал К51*/
//	MOV_CH_ERR_MAX_ACCELL,					/*Скорость изменения параметра выше допуска ГДК (сигнал О10?)*/
//	MOV_CH_ERR_MAX_SLOW,					/*Скорость изменения параметра ниже допуска ГДК (сигнал О10?)*/
	MOV_CH_ERR_PARAM						/*Ошибка получения/обработки данных*/
}MovementsStatus;

/*!
*@enum WorkRegims
*@brief Режимы работы
*/
typedef enum _WorkRegims
{
	NOP=0x0,								/*Режима нет, режим не определен*/
	START,									/*Режим "Запуск"*/
	STOP,									/*Режим "Останов"*/
	COLDSCROLL,								/*Режим "ХП"*/
	SMALLGAS,								/*Режим "Малый газ", "МГ"*/
	FLIGHT,									/*Режим "Полет"*/
	TRAINING,								/*Режим "Тренировочный", "ТР"*/
	OEI_2_5,								/*Режим "ОНД 2,5 мин"*/
	OEI_30,									/*Режим "ОНД 30 мин"*/
	TAKEOFF									/*Режим "Взлет"*/
}WorkRegims;

#pragma push
#pragma pack(1)
/*!
*@struct _DiscreteInputs1stBlock
*@brief Входные дискретные сигналы и команды, поступающие в БАРК, 1-й блок
*Используется в межмодульном обмене \n
*Состояние входных дискретных сигналов: \n
*0000 - выкл \n
*0001 - вкл \n
*0010 - обрыв \n
*0100 - К/З \n
*1000 - обрыв в цепи контроля \n
*1100 - К/З в цепи контроля \n
*/
typedef struct _DiscreteInputs1stBlock
{
	uint32_t
		mech_stop_tap:4,					/*Сигнал "Механический стоп-кран закрыт"*/
		start_engine:4,						/*Команда "Запуск"*/
		cold_rotation:4,					/*Команда "Холодная прокрутка"*/
		board:4,							/*Сигнал "Признак борта". Правый, если лог. 0*/

		stend:4,							/*Сигнал "Стенд", используется при работе двигателя на стенде*/
		gear_on_ground:4,					/*Сигнал "Шасси обжато" (ШО)*/
		stop_engine:4,						/*Команда "Останов"*/
		small_gas:4;						/*Команда "Малый газ"*/
	
}DiscreteInputs1stBlock;
#pragma pop

/*!
*@union TableOfDiscreteInputs1stBlock
*@brief Таблица состояния входных дискретных сигналов и команд, 1-й блок
*/
typedef union _TableOfDiscreteInputs1stBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения входных дискретных сигналов и команд*/
	DiscreteInputs1stBlock signals;			/*Прочитанные значения входных дискретных сигналов и команд*/
}TableOfDiscreteInputs1stBlock;

#pragma push
#pragma pack(1)
/*!
*@struct _DiscreteInputs2ndBlock
*@brief Входные дискретные сигналы и команды, поступающие в БАРК, 2-й блок
*Используется в межмодульном обмене \n
*Состояние входных дискретных сигналов: \n
*0000 - выкл \n
*0001 - вкл \n
*0010 - обрыв \n
*0100 - К/З \n
*1000 - обрыв в цепи контроля \n
*1100 - К/З в цепи контроля \n
*/
typedef struct _DiscreteInputs2ndBlock
{
	uint32_t
		reserve_system:4,					/* Команда "Переход на резервную систему" */
		workout_right:4,					/* Команда "Тренировка" */
	
		workout_left:4,						/* Команда "nст=103%", переменную необходимо будет переименовать!!! */

		AGFT_control:4,						/* Команда "АЗСТ контроль" от ИДК */
		Reserve_in_1:4,						/* Резервный вход */
	
		sync_Ntk:4,							/* Команда "Синхронизация Nтк" */
		Nft_down_priority:4,				/* Команда "Уменьшение Nст приоритет" */
		Nft_up_priority:4;					/* Команда "Увеличение Nст приоритет" */
		
}DiscreteInputs2ndBlock;
#pragma pop

/*!
*@union TableOfDiscreteInputs2ndBlock
*@brief Таблица состояния входных дискретных сигналов и команд, 2-й блок
*/
typedef union _TableOfDiscreteInputs2ndBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения входных дискретных сигналов и команд*/
	DiscreteInputs2ndBlock signals;			/*Прочитанные значения входных дискретных сигналов и команд*/
}TableOfDiscreteInputs2ndBlock;

#pragma push
#pragma pack(1)
/*!
*@struct _DiscreteInputs3rdBlock
*@brief Входные дискретные сигналы и команды, поступающие в БАРК, 3-й блок
*Используется в межмодульном обмене \n
*Состояние входных дискретных сигналов: \n
*0000 - выкл \n
*0001 - вкл \n
*0010 - обрыв \n
*0100 - К/З \n
*1000 - обрыв в цепи контроля \n
*1100 - К/З в цепи контроля \n
*/
typedef struct _DiscreteInputs3rdBlock
{
	uint32_t
		Nft_down_non_priority:4,			/* Команда "Уменьшение Nст не приоритет" */
		Nft_up_non_priority:4,				/* Команда "Увеличение Nст не приоритет" */
		Voil_min:4,							/* Сигнал "Минимальный запас масла в маслобаке" */
	
		OEI_block:4,						/* Команда "Блокировка ОНД" */
	
		dPgf_max:4,							/* Сигнал "Предельный перепад давления топлива на топливном фильтре" из ARINC429 */
	
		SAU_control:4,						/* Команда "Контроль САУ" */
		
		reserve_ds_1:4,						/* Резерв ДС */
		reserve_ds_2:4;						/* Резерв ДС */
	
}DiscreteInputs3rdBlock;
#pragma pop

/*!
*@union TableOfDiscreteInputs3rdBlock
*@brief Таблица состояния входных дискретных сигналов и команд, 3-й блок
*/
typedef union _TableOfDiscreteInputs3rdBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения входных дискретных сигналов и команд*/
	DiscreteInputs3rdBlock signals;			/*Прочитанные значения входных дискретных сигналов и команд*/
}TableOfDiscreteInputs3rdBlock;


/*--------------------------------------------------------------------------
ПОКА ТОЛЬКО ДЛЯ ОТЛАДКИ ПО МОДУЛЕЙ ЗАЩИТЫ И МОДУЛЯ УПРАВЛЕНИЯ
----------------------------------------------------------------------------*/
/*!
*@enum enMetod
*@brief Метод выдачи дискретного сигнала
*/
typedef enum _enMetod
{
	_UP1_ = 1,								/* Одноканальный сигнал (1: сh_up0/st_up0) */
	_UP1_CAN_ = 2,						/* Одноканальный сигнал + формирование запроса в структуру для CAN */
	_UP2_ = 2,								/* Двухканальный сигнал (1: ch_up0/st_up0; 2: ch_up1/st_up1) */
//	_UP2LOW2_ = 4,							/* Двухканальный сигнал (1: ch_up0/st_up0, сh_low0; 2: ch_up1/st_up1, сh_low1) */
	_UP2_CAN_ = 4,						/* Двухканальный сигнал + формирование запроса в структуру для CAN */
	_UP2_plus_CAN_ = 5	
} enMetod;


/*!
*@enum enDSfault
*@brief Состояние ключа 
*/
typedef enum _enDSfault
{
	_OK_ = 0,								/* Ключ исправен */
//	_KZ_ = 1,								/* КЗ (без возможности восстановления) */
	_KZ_ = 2,								/* КЗ (без возможности восстановления) */
	_OBR_ = 3,								/* Обрыв (с возможностью восстановления) */
	_Z_STATE_	= 4							/* Ключ отключен */
} enDSfault;

#define SET_DS_STATE_FAULT(STATE, NEW_STATE) STATE = (STATE != _Z_STATE_) ? (NEW_STATE) : STATE;
#define GET_BIT(field, bit)			((field >> bit) & 0x1)
#define GET_BITS(field, bit, N)	((field >> bit) & ((1U << N)-1))

#pragma push
#pragma pack(1)
/*!
*@struct tpDSout
*@brief Структура ключа 
*/
typedef struct _tpDSout
{
	enMetod metod;		
	volatile bool *value;							/* Требование на выдачу/снятие "1" в канал */
	volatile enDSfault *faultDS;							/* Отказ сигнала: true	- неисправен, false	- исправен */
} tpDSout;
#pragma pop

#pragma push
#pragma pack(1)
/*!
*@struct tpOUT_CHANNEL
*@brief Структура состояния канала выходного дискретного сигнала
*/
typedef struct _tpOUT_CHANNEL
{
	bool value;		/* 0 - снять сигнал, 1 - выдать */
	enDSfault fault;
} tpOUT_CHANNEL;
#pragma pop

#pragma push
#pragma pack(1)
/*!
*@struct tpINPUT_CHANNEL
*@brief Структура состояния канала входного дискретного сигнала
*/
typedef struct _tpINPUT_CHANNEL
{
	bool value;		/* 0 - снять сигнал, 1 - выдать */
	/*
	Состояние ключа:
	_OK - ключ исправен
	_KZ - КЗ ( без возможности восстановления)
	_OBRIV - ОБРЫВ (с возможностью восстановления)
	*/
	enDSfault fault;
} tpINPUT_CHANNEL;
#pragma pop

#pragma push
#pragma pack(1)
/*!
*@struct DiscreteSignalsAndCommandsOut1stBlock
*@brief Дискретные сигналы и команды, сформированные в БАРК
*/
typedef struct _DiscreteSignalsAndCommandsOut1stBlock
{
	/*
	true - сигнал установлен
	false - сигнал снят
	*/
	uint32_t
		main_system_main:2,					/*Команда включения основной системы управления двигателем, основной канал*/
		main_system_reserve:2,				/*Команда включения основной системы управления двигателем, резервный канал*/
		EST_main:2,							/*Команда на закрытие электростопкрана, основной канал*/
		EST_reserve:2,						/*Команда на закрытие электростопкрана, резервный канал*/
		ingnition_main:2,					/*Команда на включение агрегата зажигания, основной канал*/
		ingnition_reserve:2,				/*Команда на включение агрегата зажигания, резервный канал*/
		STG_on:2,							/*Команда на включение стартергенератора*/
		RCC_power:2,						/*Питание сигнализатора стружки*/
		grp_2_force_main:2,					/*Команда на подключение II группы форсунок, основной канал*/
		grp_2_force_reserve:2,				/*Команда на подключение II группы форсунок, резервный канал*/
		oil:2,								/*Сигнал "Масло", включение табло "Масло"*/
		lim_reg:2,							/*Сигнал "Ограничение режима", включение табло "ОгрРеж"*/
		main_system:2,						/*Сигнал "Основная система", выключение табло "Резерв"*/
		BARK:2,								/*Сигнал "Отказ автоматического управления", включение табло "БАРК"*/
		fault:2,							/*Синал "Неисправность", включение табло "Неисправность"*/
		stop:2;								/*Сигнал "Выключение двигателя", включение табло "nтк<60%"*/
}DiscreteSignalsAndCommandsOut1stBlock;
#pragma pop

/*!
*@union TableOfDiscreteSignalsAndCommandsOut1stBlock
*@brief Таблица состояния выходных дискретных сигналов и команд
*/
typedef union _TableOfDiscreteSignalsAndCommandsOut1stBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения сформированных дискретных сигналов и команд*/
	/*Сформированные дискретные сигналы и команды*/
	DiscreteSignalsAndCommandsOut1stBlock signals;
}TableOfDiscreteSignalsAndCommandsOut1stBlock;

#pragma push
#pragma pack(1)
/*!
*@struct DiscreteSignalsAndCommandsOut2ndBlock
*@brief Дискретные сигналы и команды, сформированные в БАРК
*/
typedef struct _DiscreteSignalsAndCommandsOut2ndBlock
{
	/*
	true - сигнал установлен
	false - сигнал снят
	*/
	uint8_t
		/*workout_OND:2,*/						/*Сигнал "Тренировка ОНД", включение табло "Тренировка"*/
		training:2,							/*Включение табло "Тренировка"*/
		OEI:2,								/*Включение табло "ОНД" (OEI - One Engine Inpoerative - с одним неработающим двигателем)*/
		reserve_ds_1:2,						/*1-й резервный дискретный выход*/
		reserve_ds_2:2;						/*2-й резервный дискретный выход*/
		
		/*reserve:2;*/							/*Резервный разряды*/
}DiscreteSignalsAndCommandsOut2ndBlock;
#pragma pop

/*!
*@union TableOfDiscreteSignalsAndCommandsOut2ndBlock
*@brief Таблица состояния выходных дискретных сигналов и команд
*/
typedef union _TableOfDiscreteSignalsAndCommandsOut2ndBlock
{
	uint8_t data;							/*Упакованные для передачи по CAN значения сформированных дискретных сигналов и команд*/
	/*Сформированные дискретные сигналы и команды*/
	DiscreteSignalsAndCommandsOut2ndBlock signals;
}TableOfDiscreteSignalsAndCommandsOut2ndBlock;

#pragma push
#pragma pack(1)
/*!
*@struct EmergencySignals
*@brief Логические сигналы, передаваемые на аварийном канале ARINC825
*/
typedef struct _EmergencySignals
{
	uint8_t
		res0:1,								/* резерв */
//		PPG:1,								/*Срабатывание ППЗ (противопомпажная защита) (младший бит)*/
		O11:1,								/*Останов по АЗТГ*/
//		ARR:1,								/*Срабатывание автоматического восстановления режима работы (АВРР)*/
		rpm_FT:1,							/*Обороты СТ (срабатывание АЗСТ)*/
		O04:1,								/*Останов по включению РС Дв на запуске*/
		O06:1,								/*Останов по погасанию КС при помпаже*/
		O07:1,								/*Останов по срабатыванию АЗСТ*/
		O12:1,								/*Останов по отказу прокрутки в ОС*/
		O13:1;								/*Останов по нештатной работе МСХ (старший бит)*/
}EmergencySignals;
#pragma pop

/*!
*@union TableOfEmergencySignals
*@brief Таблица сформированных логических сигналов, передаваемых на аварийном канале ARINC825
*/
typedef union _TableOfEmergencySignals
{
	uint8_t data;							/*Упакованные для передачи по CAN сформированные аварийные сигналы*/
	EmergencySignals signals;				/*Сформированные аварийные сигналы*/
}TableOfEmergencySignals;

#pragma push
#pragma pack(1)
/*!
*@struct InternalEmergencySignals
*@brief Вспомогательные логические сигналы (их нет в Приложении А к ТЗ), передаваемые на аварийном канале ARINC825
*/
typedef struct _InternalEmergencySignals
{
	uint8_t
		lim_Nft:1,							/*Превышение порога срабатывания АЗСТ оборотами СТ*/
		lim_Nftr:1,							/*Превышение порога срабатывания АЗСТ оборотами СТ на входе в редуктор*/
		Fft:1,								/* Флаг получения производной от Nст */
		Fftr:1;								/* Флаг получения производной от Nстр */
}InternalEmergencySignals;
#pragma pop

/*!
*@union TableOfInternalEmergencySignals
*@brief Таблица вспомогательных логических сигналов, передаваемых на аварийном канале ARINC825
*/
typedef union _TableOfInternalEmergencySignals
{
	uint8_t data;
	InternalEmergencySignals signals;
}TableOfInternalEmergencySignals;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals1stBlock
*@brief Логические сигналы и команды, 1-й блок
*/
typedef struct _LogicalSignals1stBlock
{
	uint32_t
		engine_off:1,						/*Двигатель выключен (ДВ)*/
		engine_work:1,						/*Двигатель работает (ДР)*/
		small_gas:1,						/*Малый газ (МГ)*/

		start_ready:1,						/*Готовность к запуску (ГотЗап)*/
		start_not_ready_est:1,				/*К запуску не готов - ЭСК закрыт (НеготЗап ЭСК)*/
		start_not_ready_ntk:1,				/*К запуску не готов - высокая Nтк (НеготЗап nтк)*/
		start_not_ready_rs:1,				/*К запуску не готов - включение РС (НеготЗап РС)*/
		start_not_ready_est_fail:1,			/*К запуску не готов - отказ управления ЭСК (НеготЗап И23)*/
		start_not_ready_rud_sg:1,			/*К запуску не готов - РУД не на МГ (НеготЗап РУД)*/
		start_not_ready_fault_rot:1,		/*К запуску не готов - отказ прокрутки в ОС (НеготЗап К04)*/
		start_not_ready_fault_ign:1,		/*К запуску не готов - отказ управления системой зажигания (НеготЗап И25)*/
		start_not_ready_fault:1,			/*К запуску не готов - имеются неисправности в двигателе (НеготЗап Неисп)*/
		start_not_ready_no_MG:1,
	
		start_running:1,					/*Запуск идет (Зап идет)*/
		cold_rotation_running:1,			/*Холодная прокрутка идет (ХП идет)*/
		cons_running:1,						/*Консервация идет (Конс идет)*/
		engine_stop:1,						/*Останов (Ост)*/
		engine_fault_stop:1,				/*Аварийный останов (АОст)*/
		
		sync_Ntk:1,							/*Команда "Синхронизация по Nтк" (nтк синхр)*/
		sync_Mkp:1,							/*Команда "Синхронизация по Mкр" (Мкр синхр)*/
	
		cc:1,								/*Срабатывание сигнализатора стружки (СС)*/
		combustor_turn_off:1,				/*Погасание камеры сгорания (ПогКС)*/
		
		
		O01:1,								/*Останов по максимальной температуре газов*/
		O02:1,								/*Останов по отсутствию розжига КС на запуске*/
		O03:1,								/*Останов по максимальному времени запуска*/
		O08:1,								/*Останов по нерозжигу КС при АВРР*/
		O09:1,								/*Отказ СТГ на запуске*/
		O10:1,								/*Отказ маслосистемы*/

		left_training_right:1,				/*Левому тренировка правого (Лев Тр Прав)*/
		left_training_left:1,				/*Левому тренировка правого (Лев Тр Лев)*/
		right_training_right:1,				/*Правому тренировка правого (Прав Тр Прав)*/
		right_training_left:1;				/*Правому тренировка левого (Прав Тр Лев)*/
}LogicalSignals1stBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals1stBlock
*@brief Таблица состояния логических сигналов и команд, 1-й блок
*/
typedef union _TableOfLogicalSignals1stBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals1stBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals1stBlock;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals2ndBlock
*@brief Логические сигналы и команды, 2-й блок
*/
typedef struct _LogicalSignals2ndBlock
{
	uint32_t
		/*Оставшиеся сигналы САУ двигателя*/
		lim_Ntk_max:1,						/*Работа ограничителя nтк_макс (Огр nтк)*/
		lim_Ntk_pr_max:1,					/*Работа ограничителя nтк_пр_макс (Огр nтк_пр)*/
		lim_tg_max:1,						/*Работа ограничителя tг_макс (Огр tг)*/
		lim_Mkp_max:1,						/*Работа ограничителя Мкр_макс (Огр Мкр)*/
		lim_Gt_max:1,						/*Работа ограничителя Gт_мин (Огр Gт_макс)*/
		lim_Gt_min:1,						/*Работа ограничителя Gт_мин (Огр Gт_мин)*/
		lim_N_max:1,						/*Работа ограничителя N_макс (Огр N)*/
		lim_max_reg:1,						/*Ограничение максимального режима (Огр Макс)*/
		lim_min_reg:1,						/*Ограничение минимального режима (Огр Мин)*/
		
		training_rotation_FT:1,				/*Учебная раскрутка СТ (СТ раскрут)*/
	
		/*Логические сигналы системы контроля*/
		/*Двигатель*/
		H11:1,								/*nтк предельная*/
		H12:1,								/*nтк_пр предельная*/
		H13:1,								/*tг предельная*/
		H14:1,								/*nст предельная*/
		H15:1,								/*Мкр предельный*/
		H17:1,								/*Проскальзывание МСХ*/
		
		H21:1,								/*Повышенная вибрация по X1тк*/
		H22:1,								/*Опасная вибрация по X1тк*/
	
		H23:1,								/*Повышенная вибрация по X1ст*/
		H24:1,								/*Опасная вибрация по X1ст*/
		
		H31:1,								/*Помпаж*/
		H32:1,								/*Погасание КС при открытом стоп-кране*/
		H33:1,								/*Время запуска велико*/
		H41:1,								/*Время выбега ротора ТК мало*/
		H42:1,								/*Стружка в масле*/
		H45:1,								/*Время выбега ротора СТ мало*/
		H46:1,								/*Стружка в сигнализаторе*/
		H47:1,								/*Отказ сигнализатора стружки*/
	
		/*САУ*/
		K01:1,								/*Отказ основной САУ по управлению двигателем*/
		K03:1,								/*Отказ АЗСТ*/
		K04:1,								/*Отказ прокрутки в ОС*/
		K05:1;								/*Отказ ограничения Мкр*/
}LogicalSignals2ndBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals2ndBlock
*@brief Таблица состояния логических сигналов и команд, 2-й блок
*/
typedef union _TableOfLogicalSignals2ndBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals2ndBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals2ndBlock;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals3rdBlock
*@brief Логические сигналы и команды, 3-й блок
*/
typedef struct _LogicalSignals3rdBlock
{
	uint32_t
		K06:1,								/*Отказ ППЗ*/
		K08:1,								/*Отказ ограничения nтк*/
		K09:1,								/*Отказ ограничения nст*/
		K10:1,								/*Отказ ограничения tг*/
		K11:1,								/*Отказ ЭЗУ СНП основного модуля*/
		K12:1,								/*Отказ ЭЗУ СНП резервного модуля*/
		K13:1,								/*Отказ ЭЗУ основного модуля*/
		K14:1,								/*Отказ ЭЗУ резервного модуля*/
		K15:1,								/*Отказ основного модуля*/
		K16:1,								/*Отказ резервного модуля*/
		K17:1,								/*Провал уровня напражения питания на входе в БАРК*/
	
		/*Топливная система*/
		K32:1,								/*Высокая температура топлива на входе в топливный фильтр*/
		K34:1,								/*Засорение топливного фильтра*/
	
		/*Исполнительная гидромеханическая часть САУ двигателя*/
		K43:1,								/*Отклонение Gт от заданного значения*/
		K44:1,								/*Неуправляемость ДК*/
		
		/*Маслосистема двигателя*/
		K51:1,								/*Минимальное давление масла на входе в двигатель*/
		K53:1,								/*Маслофильтр засорен*/
		K54:1,								/*Клапан перепуска на маслофильтре открыт*/
		K55:1,								/*Масла мало*/
		K57:1,								/*Велико давление масла на входе в двигатель*/
		K58:1,								/*Высокая температура масла на выходе из двигателя*/
		K59:1,								/*Масло прогрето*/
		K60:1,								/*Двигатель прогрет*/
	
		K70:1,								/*Повышенная неравномерность температурного поля*/
	
		/*Режимы работы ДУ*/
		OND2_5:1,							/*ОНД 2,5*/
		OND30:1,							/*ОНД 30*/
		OND_MP:1,							/*ОНД МП*/
		take_off:1,							/*Взлет (Взл)*/
		max_duration:1,						/*Максимальный продолжительный (МП)*/
	
		/*Сигналы по времени работы на режиме*/
		K81:1,								/*До превышения времени работы на режиме "ОНД 2,5 мин" осталось менее 30 с*/
		K82:1,								/*Время непрерывной работы на режиме "ОНД 2,5 мин" превышено*/
		K83:1;								/*До превышения времени работы на режиме "ОНД 30 мин" осталось менее 30 с*/
}LogicalSignals3rdBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals3rdBlock
*@brief Таблица состояния логических сигналов и команд, 3-й блок
*/
typedef union _TableOfLogicalSignals3rdBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals3rdBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals3rdBlock;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals4thBlock
*@brief Логические сигналы и команды, 4-й блок
*/
typedef struct _LogicalSignals4thBlock
{
	uint32_t
		K84:1,								/*Время непрерывной работы на режиме «ОНД 30 мин» превышено*/
		K85:1,								/*До превышения времени работы на режиме «Взл» осталось менее 30 с*/
		K86:1,								/*Время непрерывной работы на режиме «Взл» превышено*/
	
		/*Сигналы системы контроля БАРК*/
		/*Сигналы отказов датчиков*/
		D111:1,								/*Отказ измерения частоты вращения ротора ТК nтк1*/
		D112:1,								/*Отказ измерения частоты вращения ротора ТК nтк2*/
		D113:1,								/*Отказ получения информации по nтк от БАРК сос*/
		D117:1,								/*Отказ nтк по перекрёстному контролю*/
		D121:1,								/*Отказ измерения частоты вращения ротора ТК соседнего двигателя nтк_сос*/
		D123:1,								/*Отказ получения информации по nтк_сос от БАРК сос*/
		D127:1,								/*Отказ nтк_сос по перекрёстному контролю*/
		D13:1,								/*Отказ измерения частоты вращения ротора СТ*/
		D131:1,								/*Отказ измерения частоты вращения ротора СТ nст1*/
		D132:1,								/*Отказ измерения частоты вращения ротора СТ nст2*/
		D133:1,								/*Отказ измерения частоты вращения ротора СТ на входе в редуктор nрст1*/
		D134:1,								/*Отказ измерения частоты вращения ротора СТ на входе в редуктор nрст2*/
		D137:1,								/*Отказ nст по перекрёстному контролю*/
		D14:1,								/*Отказ измерения частоты вращения НВ nнв*/
		D311:1,								/*Отказ измерения температуры газов перед СТ, первая термопара tг1*/
		D321:1,								/*Отказ измерения температуры газов перед СТ, первая термопара tг2*/
		D331:1,								/*Отказ измерения температуры газов перед СТ, первая термопара tг3*/
		D411:1,								/*Отказ измерения температуры воздуха на входе в двигатель tвх1*/
		D412:1,								/*Отказ измерения температуры воздуха на входе в двигатель tвх2*/
		D417:1,								/*Отказ tвх по перекрёстному контролю*/
		D423:1,								/*Отказ получения информации по tвх сос от БАРК сос*/
		D441:1,								/*Отказ измерения температуры топлива на входе в ТФ, tт вх*/
		D511:1,								/*Отказ измерения абсолютного давления воздуха на входе в двигатель, рвх*/
		D523:1,								/*Отказ получения информации по рвх сос от БАРК сос*/
		D551:1,								/*Отказ измерения абсолютного давления воздуха за компрессором рк*/
		D561:1,								/*Отказ измерения абсолютного давления масла в ИКМ рикм1*/
		D562:1,								/*Отказ измерения абсолютного давления масла в ИКМ рикм2*/
		D567:1,								/*Отказ рикм по перекрёстному контролю*/
		D631:1;								/*Отказ измерения абсолютного давления масла перед масло-фильтром, рм_ф*/
}LogicalSignals4thBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals4thBlock
*@brief Таблица состояния логических сигналов и команд, 4-й блок
*/
typedef union _TableOfLogicalSignals4thBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals4thBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals4thBlock;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals5thBlock
*@brief Логические сигналы и команды, 5-й блок
*/
typedef struct _LogicalSignals5thBlock
{
	uint32_t
		D711:1,								/*Отказ измерения угла положения РУД, Aруд1*/
		D712:1,								/*Отказ измерения угла положения РУД, Aруд2*/
		D717:1,								/*Отказ Aруд по перекрёстному контролю*/
		D911:1,								/*Отказ измерения угла положения РОШ, Aрош1*/
		D912:1,								/*Отказ измерения угла положения РОШ, Aрош2*/
		D917:1,								/*Отказ Aрош по перекрёстному контролю*/
		D741:1,								/*Отказ измерения положения дозирующей топливной иглы в агрегате НР Lди1*/
		D742:1,								/*Отказ измерения положения дозирующей топливной иглы в агрегате НР Lди2*/
	
		D747:1,								/*Отказ Lди по перекрёстному контролю*/
		D811:1,								/*Отказ измерения виброскорости Х1*/
	
		/*Сигналы отказов исполнительных механизмов*/
		I011:1,								/*Отказ исполнительного механизма управления расходом топлива в двигатель, основной канал*/
		I012:1,								/*Отказ исполнительного механизма управления расходом топлива в двигатель, резервный канал*/
		I211:1,								/*Отказ исполнительного механизма включения основной системы управления двигателем, основной канал*/
		I212:1,								/*Отказ исполнительного механизма включения основной системы управления двигателем, резервный канал*/
		I231:1,								/*Отказ исполнительного механизма управления электрическим стоп-краном, основной канал*/
		I232:1,								/*Отказ исполнительного механизма управления электрическим стоп-краном, резервный канал*/
	
		I241:1,								/*Команда на подключение II группы форсунок, основной канал*/
		I242:1,								/*Команда на подключение II группы форсунок, резервный канал*/
		I251:1,								/*Отказ управления системой зажигания, основной канал*/
		I252:1,								/*Отказ управления системой зажигания, резервный канал*/
		I261:1,								/*Команда на стартовое реле (команда на СТГ?)*/
		I341:1,								/*Отказ питания сигнализатора стружки*/
		
		I411:1,								/*Отказ выдачи сигнала "Выключение двигателя" ("nтк<60%")*/
		I421:1,								/*Отказ выдачи сигнала "Ограничение режима"*/
		I441:1,								/*Отказ выдачи сигнала «Основная система»*/
		I451:1,								/*Отказ выдачи сигнала «Включение ОНД»*/
		I461:1,								/*Отказ выдачи сигнала «Масло»*/
		I471:1,								/*Отказ выдачи сигнала «Неисправность»*/
		I491:1,								/*Отказ выдачи сигнала «Тренировка ОНД»*/
		I501:1,								/*Отказ выдачи сигнала «Отказ БАРК»*/
	
		A02:1,								/*Отказ передачи в БАРКсос, БХД от БАРК*/
		A14:1;								/*Отсутствие информации от БАРКсос в БАРК*/
}LogicalSignals5thBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals5thBlock
*@brief Таблица состояния логических сигналов и команд, 5-й блок
*/
typedef union _TableOfLogicalSignals5thBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals5thBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals5thBlock;


#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignals6thBlock
*@brief Логические сигналы и команды, 6-й блок
*/
typedef struct _LogicalSignals6thBlock
{
	uint8_t
		/*Сигналы отказов по обмену информацией*/
		A16:1,								/*Отсутствие информации от БХД в БАРК*/
		A21:1,								/*Отказ обмена БАРК с ИДК*/
		A11:1,								/*Отказ обмена БАРК с БИСК*/
		
		D141:1,								/*Отказ измерения частоты вращения НВ Nнв1*/
		D142:1,								/*Отказ измерения частоты вращения НВ Nнв2*/
		
		D552:1,								/*Отказ измерения давления воздуха за компрессором Pк1*/
		D553:1,								/*Отказ измерения давления воздуха за компрессором Pк2*/
		K61:1;								/*Предельная температура масла на выходе из двигателя*/
}LogicalSignals6thBlock;
#pragma pop

/*!
*@union TableOfLogicalSignals6thBlock
*@brief Таблица состояния логических сигналов и команд, 6-й блок
*/
typedef union _TableOfLogicalSignals6thBlock
{
	uint8_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignals6thBlock signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignals6thBlock;

#pragma push
#pragma pack(1)
typedef struct _DiscreteInputs4thBlock
{
	uint8_t 
		Pk1:2,
	DC_MZ1:2,
	Pk2:2,
	DC_MZ2:2,
	P_mf:2,
	DC_MK:2,
	DC_MU:2,
	EW_sos:1,
	res8:1;
}DiscreteInputs4thBlock;
#pragma pop

typedef union _TableOfDiscreteInputs4thBlock
{
	uint8_t data;
	DiscreteInputs4thBlock signals;
}TableOfDiscreteInputs4thBlock;

#pragma push
#pragma pack(1)
/*!
*@struct ReserveDiscrette
*@brief резерв, 1-й блок
*/
typedef struct _Reserve1stBlock
{
	uint32_t
		res1:4,
		res2:1,
		res3:1,
		res4:1,
		res5:1,
		res6:1,
		res7:1,
		res8:1,
		res9:1,
		res10:1,
		res11:1,
		res12:1,
		res13:1,
		res14:1,
		res15:1,
		res16:1,
		res17:1,
		res18:1,
		res19:1,
		res20:1,
		res21:1,
		res22:1,
		res23:1,
		res24:1,
		res25:1,
		res26:1,
		res27:1,
		res28:1,
//		res29:1,
//		res30:1,
//		res31:1,
		res32:1;
		
}Reserve1stBlock;
#pragma pop

/*!
*@union TableOfReserve1stBlock
*@brief Резерв дискретов, 1-й блок
*/
typedef union _TableOfReserve1stBlock
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	Reserve1stBlock signals;			/*Сформированные логические сигналы*/
}TableOfReserve1stBlock;

#pragma push
#pragma pack(1)
/*!
*@struct ReserveDiscrette
*@brief  Резерв, 2-й блок
*/
typedef struct _Reserve2ndBlock
{
	uint16_t
		BARK_state_CAN:4,  //состояние БАРКа, которое передаем в CAN
		Rapriz_Pk1_power:1,//флаги на выдачу дискретов +27 по требованию ИДК
		Rapriz_Pk2_power:1,
		Rapriz_Pmf_power:1,
		Power_DS_MU1:1,
		Power_DS_MU2:1,
		Power_DS_MZ1:1,
		Power_DS_MZ2:1,
		RightOut_CAN:1,
		CC_power:1,
		Power_DS_MK:1,
		Rapriz_MU2_POWER:1,
		AGFT_control:1;
//		res212:1,
//		res213:1,
//		res214:1,
//		res215:1,
//		res216:1;
}Reserve2ndBlock;
#pragma pop

/*!
*@union TableOfReserve1stBlock
*@brief Резерв дискретов, 2-й блок
*/
typedef union _TableOfReserve2ndBlock
{
	uint16_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	Reserve2ndBlock signals;			/*Сформированные логические сигналы*/
}TableOfReserve2ndBlock;

#pragma push
#pragma pack(1)
/*!
*@struct States
*@brief Состояния БАРК-5В
*/
typedef struct _States
{
	uint8_t 
		states:4;
}States;
#pragma pop

/*!
*@union BARKTableOfStates
*@brief Таблица состояний БАРК-5В
*Значения: \n
*0 - Включение питания (краткий тест) \n
*1 - Ожидание (ожидание команд и т.п.) \n
*2 - Запуск (получена команда "Запуск") \n
*3 - ХП (получена команда "ХП") \n
*4 - Консервация (получена команда "Консервация") \n
*5 - Работа (работает основная система) \n
*6 - РС (работает резервная система) \n
*7 - ИДК (БАРК не производит управление, все команды от ИДК) \n
*8 - Отказ БАРК \n
*9 - Аварийный останов (сформирована команда аварийного останова) \n
*10 - Останов (получена команда "Останов") \n \n
*/
typedef union _BARKTableOfStates
{
	uint8_t data;
	States signals;
}BARKTableOfStates;

#pragma push
#pragma pack(1)
/*!
*@struct RegimOfWork
*@brief Режимы работы БАРК-5В
*Значения:
*0 - Режима нет
*1 - Запуск
*2 - Останов
*3 - ХП
*4 - МГ
*5 - Полет
*6 - Тренировочный
*7 - ОНД 2,5
*8 - ОНД 30
*9 - Взлет
*/
typedef struct _RegimOfWork
{
	uint8_t
		regims:4;							/*Режимы/сигналы работы*/
}RegimOfWork;
#pragma pop

/*!
*@union TableOfRegimOfWork
*@brief Таблица режимов работы БАРК-5В
*/
typedef union _TableOfRegimOfWork
{
	uint8_t data;							/*Упакованные данные о режимах/сигналах работы*/
	RegimOfWork signals;					/*Сформированные режимы/сигналы работы*/
}TableOfRegimOfWork;

#pragma push
#pragma pack(1)
/*!
*@struct LogicalSignalsFromModel
*@brief Логические сигналы и команды из модели, которых нет в оригинальном 5В
*/
typedef struct _LogicalSignalsFromModel
{
	uint32_t
		Ntk_channel:2,
		Nst_channel:2,
		Nstr_channel:2,
		Tg_channel:3,
		Pk_channel:2,
		Pikm_channel:2,
		RUD_channel:2,
		new_reg:1, 
		kontur:4,
		O11:1,								/*Останов по АЗТГ*/
		O13:1,								/*Останов по АЗТК*/
		ASR:1,								/*Флаг сигнала АСР*/
		//перенесено сюда, чтобы впихнуть "Негот зап - нет МГ". По ТЗ нет, а по логике - есть
		enable_OND:1;						/*Разрешение ОНД (ОНД)*/

}LogicalSignalsFromModel;
#pragma pop

/*!
*@union LogicalSignalsFromModel
*@brief Логические сигналы и команды из модели, которых нет в оригинальном 5В
*/
typedef union _TableOfLogicalSignalsFromModel
{
	uint32_t data;							/*Упакованные для передачи по CAN значения логических сигналов*/
	LogicalSignalsFromModel signals;			/*Сформированные логические сигналы*/
}TableOfLogicalSignalsFromModel;

/*-----------------------------------------------------------------------------
            Определения
-----------------------------------------------------------------------------*/
/*!
*@def _DS_ON_
*@brief Дискретный сигнал/команда включен
*/
#define	_DS_ON_							true
/*!
*@def _DS_OFF_
*@brief Дискретный сигнал/команда выключен
*/
#define	_DS_OFF_						false

/*!
*@def COEF_SCALE_1000
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_1000					1000
/*!
*@def COEF_SCALE_100
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_100					100
/*!
*@def COEF_SCALE_10
*@brief Масштабный коэффициент для коррекции передаваемых в ИДК данных
*/
#define	COEF_SCALE_10					10

/*!
*@def FREQ_CH_BASE
*@brief База для приведения значения оборотов
*/
#define	FREQ_CH_BASE					8192
/*!
*@def FREQ_CH_COEF
*@brief Масштабный коэффициент для приведения значения оборотов
*/
#define	FREQ_CH_COEF					50
/*!
*@def FREQ_CH_OFFSET
*@brief Смещение для приведения значения оборотов
*/
#define	FREQ_CH_OFFSET					0

/*!
*@def THERMO_COUPLE_CH_BASE
*@brief База для приведения значения температуры термопары
*/
#define	THERMO_COUPLE_CH_BASE			8192
/*!
*@def THERMO_COUPLE_CH_COEF
*@brief Масштабный коэффициент для приведения значения температуры термопары
*/
#define	THERMO_COUPLE_CH_COEF			5
/*!
*@def THERMO_COUPLE_CH_OFFSET
*@brief Смещение для приведения значения температуры термопары
*/
#define	THERMO_COUPLE_CH_OFFSET			273

/*!
*@def THERMO_RESISTANCE_CH_BASE
*@brief База для приведения значения температуры термистора
*/
#define	THERMO_RESISTANCE_CH_BASE		8192
/*!
*@def THERMO_RESISTANCE_CH_COEF
*@brief Масштабный коэффициент для приведения значения температуры термистора
*/
#define	THERMO_RESISTANCE_CH_COEF		25
/*!
*@def THERMO_RESISTANCE_CH_OFFSET
*@brief Смещение для приведения значения температуры термистора
*/
#define	THERMO_RESISTANCE_CH_OFFSET		80

/*!
*@def MOVEMENTS_CH_BASE_LRUD
*@brief База для приведения значений РУД/РОШ
*/
#define	MOVEMENTS_CH_BASE_LRUD				8192
/*!
*@def MOVEMENTS_CH_COEF_LRUD
*@brief Масштабный коэффициент для приведения значений РУД/РОШ
*/
#define	MOVEMENTS_CH_COEF_LRUD				20//20
/*!
*@def MOVEMENTS_CH_OFFSET_LRUD
*@brief Смещение для приведения значений РУД/РОШ
*/
#define	MOVEMENTS_CH_OFFSET_LRUD				120//8192

/*!
*@def MOVEMENTS_CH_BASE_LDK
*@brief База для приведения значений Lди/Фикм
*/
#define	MOVEMENTS_CH_BASE_LDK				8192
/*!
*@def MOVEMENTS_CH_COEF_LDK
*@brief Масштабный коэффициент для приведения значений Lди/Фикм
*/
#define	MOVEMENTS_CH_COEF_LDK				1000//20
/*!
*@def MOVEMENTS_CH_OFFSET_LDK
*@brief Смещение для приведения значений Lди/Фикм
*/
#define	MOVEMENTS_CH_OFFSET_LDK				1//8192

/*!
*@def PRESS_CH_BASE
*@brief База для приведения значения давления
*/
#define	PRESS_CH_BASE					8192
/*!
*@def PRESS_CH_COEF
*@brief Масштабный коэффициент для приведения значения давления
*/
#define	PRESS_CH_COEF					200
/*!
*@def PRESS_CH_OFFSET
*@brief Смещение для приведения значения давления
*/
#define	PRESS_CH_OFFSET					0

/*!
*@def U_SUPPLY_CH_BASE
*@brief База для приведения значения напряжения питания
*/
#define	U_SUPPLY_CH_BASE				8192
/*!
*@def U_SUPPLY_CH_COEF
*@brief Масштабный коэффициент для приведения значения напряжения питания
*/
#define	U_SUPPLY_CH_COEF				50
/*!
*@def U_SUPPLY_CH_OFFSET
*@brief Смещение для приведения значения напряжения питания
*/
#define	U_SUPPLY_CH_OFFSET				0

/*!
*@def VIBRO_CH_BASE
*@brief База для приведения значения вибраций
*/
#define	VIBRO_CH_BASE					8192
/*!
*@def VIBRO_CH_COEF
*@brief Масштабный коэффициент для приведения значения вибраций
*/
#define	VIBRO_CH_COEF					20
/*!
*@def VIBRO_CH_OFFSET
*@brief Смещение для приведения значения вибраций
*/
#define	VIBRO_CH_OFFSET					0

/*!
*@def PROPORTIONAL_CH_BASE
*@brief База для приведения значения тока
*/
#define	PROPORTIONAL_CH_BASE			8192
/*!
*@def PROPORTIONAL_CH_COEF
*@brief Масштабный коэффициент для приведения значения тока
*/
#define	PROPORTIONAL_CH_COEF			100
/*!
*@def PROPORTIONAL_CH_OFFSET
*@brief Смещение при приведения значения тока
*/
#define	PROPORTIONAL_CH_OFFSET			35


/*----- Для обмена по протоколу ARINC825 -----*/
/*!
*@def PGN_RPMS_FRAME_0
*@brief Номер группы сигналов/команд "Обороты. Кадр 0"
*/
#define	PGN_RPMS_FRAME_0						0x1
/*!
*@def OFFSET_N_FT
*@brief Позиция оборотов свободной турбины (после ГДК) в передаваемом пакете
*/
#define	OFFSET_N_FT								0x3
/*!
*@def OFFSET_N_FTR
*@brief Позиция оборотов свободной турбины на входе в редуктор (после ГДК) в передаваемом пакете
*/
#define	OFFSET_N_FTR							0x0
/*!
*@def OFFSET_N_VV
*@brief Позиция оборотов на выводном валу (после ГДК) в передаваемом пакете
*/
#define OFFSET_N_VV								0x1
/*!
*@def OFFSET_N_TK
*@brief Позиция оборотов турбокомпрессора (после ГДК) в передаваемом пакете
*/
#define	OFFSET_N_TK								0x2
/*!
*@def PGN_RPMS_FRAME_1
*@brief Номер группы сигналов/команд "Обороты. Кадр 1"
*/
#define	PGN_RPMS_FRAME_1						0x2
/*!
*@def OFFSET_RAW_N_TK_ADJ
*@brief Позиция оборотов турбокомпрессора ("сырые" значения) соседнего двигателя в передаваемом пакете
*/
#define	OFFSET_RAW_N_TK_ADJ						0x2
/*!
*@def OFFSET_RAW_N_MR
*@brief Позиция оборотов несущего винта ("сырые" значения) в передаваемом пакете
*/
#define OFFSET_RAW_N_MR							0x3
/*!
*@def OFFSET_RAW_N_FT
*@brief Позиция оборотов свободной турбины ("сырые" значения)
*/
#define	OFFSET_RAW_N_FT							0x0
/*!
*@def OFFSET_RAW_N_FTR
*@brief Позиция оборотов свободной турбины на входе в редуктор ("сырые" значения)
*/
#define	OFFSET_RAW_N_FTR						0x1
/*!
*@def PGN_RPMS_FRAME_2
*@brief Номер группы сигналов/команд "Обороты. Кадр 2"
*/
#define	PGN_RPMS_FRAME_2						0x10
/*!
*@def OFFSET_N_TK_ADJ
*@brief Позиция оборотов турбокомпрессора соседнего двигателя (после ГДК) в передаваемом пакете
*/
#define	OFFSET_N_TK_ADJ							0x03
/*!
*@def OFFSET_N_MR
*@brief Позиция оборотов несущего винта (после ГДК) в передаваемом пакете
*/
#define	OFFSET_N_MR								0x2
/*!
*@def OFFSET_N_TK_PRIV
*@brief Позиция оборотов приведенных оборотов тукбокомпрессора в передаваемом пакете
*/
#define	OFFSET_N_TK_PRIV						0x0
/*!
*@def OFFSET_N_FT_PROG
*@brief Позиция программного значения оборотов свободной турбины в передаваемом пакете
*/
#define	OFFSET_N_FT_PROG						0x1
/*!
*@def PGN_RPMS_FRAME_3
*@brief Номер группы сигналов/команд быстрой передачи оборотов
*/
#define PGN_RPMS_FRAME_3 						0x1E
/*!
*@def OFFSET_RAW_N_TK
*@brief Позиция оборотов турбокомпрессора ("сырые" значения) в передаваемом пакете
*/
#define	OFFSET_RAW_N_TK							0x0
/*!
*@def OFFSET_RAW_N_VV
*@brief Позиция оборотов выводного вала в передаваемом пакете
*/
#define	OFFSET_RAW_N_VV							0x1
/*!
*@def OFFSET_FAST_TIN
*@brief Позиция температуры воздуха на входе в двигатель в передаваемом пакете
*/
#define	OFFSET_FAST_TIN							0x2
/*!
*@def OFFSET_FAST_PIN
*@brief Позиция давления воздуха на входе в двигатель в передаваемом пакете
*/
#define	OFFSET_FAST_PIN							0x3


//#define	PGN_RPMS_FRAME_2						0x42
///*!
//*@def OFFSET_N_VV
//*@brief Позиция оборотов выводного вала в передаваемом пакете
//*/
//#define	OFFSET_N_VV								0x2

///*!
//*@def PGN_FAST_RPMS
//*@brief Номер группы сигналов/команд "Обороты. Кадр 3"
//*/
//#define PGN_FAST_RPMS 							0x15
///*!
//*@def OFFSET_RAW_N_FT
//*@brief Позиция оборотов свободной турбины (до ГДК) )в передаваемом пакете
//*/
//#define	OFFSET_RAW_N_FT							0x1
///*!
//*@def OFFSET_RAW_N_FTR
//*@brief Позиция оборотов свободной турбины (редуктора) (до ГДК) в передаваемом пакете
//*/
//#define OFFSET_RAW_N_FTR						0x2

/*!
*@def PGN_THERMO_PRESS_FRAME_0
*@brief Номер группы сигналов/команд "Температуры и давления. Кадр 0"
*/
#define	PGN_THERMO_PRESS_FRAME_0				0x3
/*!
*@def OFFSET_THERMO_COUPLE_1
*@brief Позиция температуры газов (1-я термопара) в передаваемом пакете
*/
#define	OFFSET_THERMO_COUPLE_1					0x0
/*!
*@def OFFSET_THERMO_COUPLE_2
*@brief Позиция температуры газов (2-я термопара) в передаваемом пакете
*/
#define	OFFSET_THERMO_COUPLE_2					0x1
/*!
*@def OFFSET_THERMO_COUPLE_3
*@brief Позиция температуры газов (3-я термопара) в передаваемом пакете
*/
#define	OFFSET_THERMO_COUPLE_3					0x2
/*!
*@def PGN_THERMO_PRESS_FRAME_1
*@brief Номер группы сигналов/команд "Температуры и давления. Кадр 1"
*/
#define	PGN_THERMO_PRESS_FRAME_1				0x4
/*!
*@def OFFSET_T_IN_FUEL_MAIN
*@brief Позиция температуры топлива на входе в топливный фильтр в передаваемом пакете, основной канал в модуле контроля
*/
#define	OFFSET_T_IN_FUEL_MAIN					0x0
/*!
*@def OFFSET_P_OIL_FILTER_RESERVE
*@brief Позиция температуры топлива на входе в топливный фильтр в передаваемом пакете, резервный канал в модуле контроля
*/
#define	OFFSET_T_IN_FUEL_RESERVE				0x0
/*!
*@def OFFSET_P_OIL_FILTER
*@brief Позиция температуры топлива на входе в топливный фильтр в передаваемом пакете
*/
#define	OFFSET_T_IN_FUEL						0x0
/*!
*@def OFFSET_P_K
*@brief Позиция давления воздуха за турбокомпрессором в передаваемом пакете
*/
#define	OFFSET_P_K								0x2
/*!
*@def OFFSET_P_OIL_IN
*@brief Позиция давления масла на входе в двигатель в передаваемом пакете
*/
#define	OFFSET_P_OIL_IN							0x1
/*!
*@def OFFSET_T_OIL_OUT
*@brief Позиция температуры масла на выходе из двигателя в передаваемом пакете
*/
#define	OFFSET_T_OIL_OUT						0x3
/*!
*@def PGN_THERMO_PRESS_FRAME_2
*@brief Номер группы сигналов/команд "Температуры и давления. Кадр 2"
*/
#define	PGN_THERMO_PRESS_FRAME_2				0x5
/*!
*@def OFFSET_P_OIL_FILTER_MAIN
*@brief Позиция давления масла на входе маслофильтра в передаваемом пакете, основной канал в модуле контроля
*/
#define	OFFSET_P_OIL_FILTER_MAIN				0x0
/*!
*@def OFFSET_P_OIL_FILTER_RESERVE
*@brief Позиция давления масла на входе маслофильтра в передаваемом пакете, резервный канал измерения в модуле контроля
*/
#define	OFFSET_P_OIL_FILTER_RESERVE				0x0
/*!
*@def OFFSET_P_OIL_FILTER
*@brief Позиция давления масла на входе маслофильтра в передаваемом пакете
*/
#define	OFFSET_P_OIL_FILTER						0x0
/*!
*@def OFFSET_P_IKM
*@brief Позиция давления ИКМ в передаваемом пакете
*/
#define	OFFSET_P_IKM							0x3
/*!
*@def OFFSET_T_AIR_IN
*@brief Позиция температуры воздуха на входе в двигатель в передаваемом пакете
*/
#define	OFFSET_T_AIR_IN							0x2
/*!
*@def OFFSET_P_AIR_IN
*@brief Позиция давления воздуха на входе в двигатель в передаваемом пакете
*/
#define	OFFSET_P_AIR_IN							0x1

/*!
*@def PGN_T_BLOCK_U_SUPPLY_FRAME_0
*@brief Номер группы сигналов/команд "Температура блока и напряжения питания. Кадр 0"
*/
#define	PGN_T_BLOCK_U_SUPPLY_FRAME_0			0x6
/*!
*@def OFFSET_T_BLOCK
*@brief Позиция температуры блока в передаваемом пакете
*/
#define	OFFSET_T_BLOCK							0x1
/*!
*@def OFFSET_U_SUPPLY_1
*@brief Позиция 1-го наппряжения питания (модули защиты) в передаваемом пакете
*/
#define OFFSET_U_SUPPLY_1						0x0
/*!
*@def OFFSET_U_SUPPLY_2
*@brief Позиция 2-го наппряжения питания (модули управления) в передаваемом пакете
*/
#define OFFSET_U_SUPPLY_2						0x2
/*!
*@def OFFSET_U_SUPPLY_3
*@brief Позиция 3-го наппряжения питания (модули управления) в передаваемом пакете
*/
#define OFFSET_U_SUPPLY_3						0x3

/*!
*@def PGN_D_IN_OUT_FRAME_0
*@brief Номер группы сигналов/команд "Дискретные сигналы и команды. Кадр 0"
*/
#define	PGN_D_IN_OUT_FRAME_0					0x07
/*!
*@def OFFSET_D_INPUTS_1
*@brief Позиция 1-го блока дискретных входов в передаваемом пакете
*/
#define	OFFSET_D_INPUTS_1						0x0
/*!
*@def OFFSET_D_INPUTS_2
*@brief Позиция 2-го блока дискретных входов в передаваемом пакете
*/
#define	OFFSET_D_INPUTS_2						0x1
/*!
*@def PGN_D_IN_OUT_FRAME_1
*@brief Номер группы сигналов/команд "Дискретные сигналы и команды. Кадр 1"
*/
#define	PGN_D_IN_OUT_FRAME_1					0x0D

/*!
*@def OFFSET_D_INPUTS_3
*@brief Позиция 3-го блока дискретных входов в передаваемом пакете
*/
#define	OFFSET_D_INPUTS_3						0x1//0x2
/*!
*@def OFFSET_D_OUTPUTS_1
*@brief Позиция 1-го блока дискретных выходов в передаваемом пакете
*/
#define	OFFSET_D_OUTPUTS_1						0x0
/*!
*@def PGN_D_IN_OUT_FRAME_2
*@brief Номер группы сигналов/команд "Дискретные сигналы и команды. Кадр 2"
*/
#define	PGN_D_IN_OUT_FRAME_2					0x0E
/*!
*@def OFFSET_D_OUTPUTS_2
*@brief Позиция 2-го блока дискретных выходов в передаваемом пакете
*/
#define	OFFSET_D_OUTPUTS_2						0x0

//Позиция блока, в котором передаются отказы по питанию РАПРИЗов и питанию ДС
#define	OFFSET_DS_POWER_FAULTS					0x1

/*!
*@def PGN_MOVEMENTS_FRAME_0
*@brief Номер группы сигналов/команд "Углы и перемещения. Кадр 0"
*/
#define	PGN_MOVEMENTS_FRAME_0					0x8
/*!
*@def OFFSET_A_CL
*@brief Позиция угла РОШ в передаваемом пакете
*/
#define	OFFSET_A_CL								0x1
/*!
*@def OFFSET_TLA
*@brief Позиция угла РУД в передаваемом пакете
*/
#define	OFFSET_TLA								0x0
/*!
*@def OFFSET_LDI
*@brief Позиция положения дозирующей иглы в передаваемом пакете
*/
#define	OFFSET_LDI								0x2
/*!
*@def OFFSET_LSKR
*@brief Позиция угла скрутки в передаваемом пакете
*/
#define	OFFSET_LSKR								0x3

/*!
*@def PGN_VIBRATIONS_FRAME_0
*@brief Номер группы сигналов/команд "Вибрации. Кадр 0"
*/
#define	PGN_VIBRATIONS_FRAME_0					0x09
/*!
*@def OFFSET_VIBR_X1TK
*@brief Позиция канала измерения вибрации X1тк в передаваемом пакете
*/
#define	OFFSET_VIBR_X1TK						0x0
/*!
*@def OFFSET_VIBR_X1FT
*@brief Позиция канала измерения вибрации X1ст в передаваемом пакете
*/
#define	OFFSET_VIBR_X1FT						0x1

/*!
*@def PGN_LOGICAL_DATA_FRAME_0
*@brief Номер группы сигналов/команд "Логические сигналы и команды. Кадр 0"
*/
#define	PGN_LOGICAL_DATA_FRAME_0				0x0A
/*!
*@def OFFSET_LOGICAL_DATA_1
*@brief Позиция 1-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_1					0x0
/*!
*@def OFFSET_LOGICAL_DATA_2
*@brief Позиция 2-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_2					0x1
/*!
*@def PGN_LOGICAL_DATA_FRAME_1
*@brief Номер группы сигналов/команд "Логические сигналы и команды. Кадр 1"
*/
#define	PGN_LOGICAL_DATA_FRAME_1				0x0B
/*!
*@def OFFSET_LOGICAL_DATA_3
*@brief Позиция 3-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_3					0x0
/*!
*@def OFFSET_LOGICAL_DATA_4
*@brief Позиция 4-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_4					0x1
/*!
*@def PGN_LOGICAL_DATA_FRAME_2
*@brief Номер группы сигналов/команд "Логические сигналы и команды. Кадр 2"
*/
#define	PGN_LOGICAL_DATA_FRAME_2				0x0C
/*!
*@def OFFSET_LOGICAL_DATA_5
*@brief Позиция 5-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_5					0x0
/*!
*@def OFFSET_LOGICAL_DATA_6
*@brief Позиция 6-го блока логических сигналов и команд в передаваемом пакете
*/
#define	OFFSET_LOGICAL_DATA_6					0x02

/*!
*@def PGN_PROPORTIONAL_DATA
*@brief Номер группы сигналов/команд "Пропорциональные команды. Кадр 0"
*/
#define	PGN_PROPORTIONAL_DATA					0x0F
/*!
*@def OFFSET_Im_GT_CTRL 
*@brief Позиция значения выданного тока в передаваемом пакете
*/
#define	OFFSET_Im_GT_CTRL						0x00
/*!
*@def OFFSET_Im_GT_OUT 
*@brief Позиция значения требуемого тока в передаваемом пакете
*/
#define	OFFSET_Im_GT_OUT						0x01
///*!
//*@def OFFSET_N
//*@brief Позиция рассчитанного значения мощности в передаваемом пакете
//*/
//#define	OFFSET_N								0x02
///*!
//*@def OFFSET_MKR
//*@brief Позиция рассчитанного значения крутящего момента в передаваемом пакете
//*/
//#define	OFFSET_MKR								0x03

/*!
*@def PGN_FAULTS_FRAME_0
*@brief Номер группы сигналов/команд "Аварийные сигналы и команды. Кадр 0"
*/
#define	PGN_FAULTS_FRAME_0						0x20
/*!
*@def OFFSET_FAULTS
*@brief Позиция аварийных сигналов и команд в передаваемом пакете
*/
#define	OFFSET_FAULTS							0x0
/*!
*@def OFFSET_INTERNAL_FAULTS
*@brief Позиция вспомогательных аварийных сигналов и команд в передаваемом пакете
*/
#define	OFFSET_INTERNAL_FAULTS					0x1
/*!
*@def OFFSET_AZTK_AZST
*@brief Позиция флагов АЗТК и АЗСТ
*/
#define	OFFSET_AZTK_AZST						0x1

/*!
*@def PGN_THERMO_PRESS_FRAME_4
*@brief Номер группы сигналов/команд "Температуры и давления. Кадр 4"
*/
#define	PGN_THERMO_PRESS_FRAME_4				0x13
/*!
*@def OFFSET_DELTA_P_AIR
*@brief Позиция пульсационной составляющей давления воздуха за компрессором в передаваемом пакете
*/
#define	OFFSET_DELTA_P_AIR					0x1
// #define	OFFSET_DELTA_P_AIR						0x3

//Дискретные сигналы и команды. Кадр 1. Другой PGN
/*!
*@def PGN_D_IN_OUT_FRAME_1_NEW
*@brief Номер группы сигналов/команд "Дискретные сигналы и команды, отказы. Кадр 0"
*/
#define	PGN_D_IN_OUT_FRAME_1_NEW				0x1D
/*!
*@def PGN_D_IN_OUT_FRAME_2_NEW
*@brief Номер группы сигналов/команд "Дискретные сигналы и команды, отказы. Кадр 1"
*/
#define	PGN_D_IN_OUT_FRAME_2_NEW				0x1A

//new
//#define PGN_THERMO_PRESS_FRAME_0_NEW			0x3B
//#define PGN_THERMO_PRESS_FRAME_1_NEW			0x3A

/*!
*@def PGN_CRC_FRAME_0
*@brief Номер группы сигналов/команд "КС ПО и СНП. Кадр 0"
*/
#define PGN_CRC_FRAME_0							0x34
/*!
*@def OFFSET_CRC_SOFT
*@brief Позиция КС ПО в передаваемом пакете
*/
#define OFFSET_CRC_SOFT   						0x0
/*!
*@def OFFSET_CRC_SNP_MODULE
*@brief Позиция КС настроечной СНП в передаваемом пакете
*/
#define OFFSET_CRC_SNP_MODULE   				0x1
/*!
*@def OFFSET_CRC_SNP_ENGINE
*@brief Позиция КС двигательной СНП в передаваемом пакете
*/
#define OFFSET_CRC_SNP_ENGINE   				0x2
/*!
*@def OFFSET_HELICOPTER_MODEL
*@brief Позиция кода модели вертолета в передаваемом пакете
*/
#define	OFFSET_HELICOPTER_MODEL					0x3



///*!
//*@def PGN_CRC_MU_OTHER
//*@brief Номер группы сигналов/команд "КС соседнего модуля управления"
//*/
//#define PGN_CRC_MU_OTHER						0x34
///*!
//*@def PGN_CRC_MZ1
//*@brief Номер группы сигналов/команд "КС основной платы защиты"
//*/
//#define PGN_CRC_MZ1								0x35
///*!
//*@def PGN_CRC_MZ2
//*@brief Номер группы сигналов/команд "КС резервной платы защиты"
//*/
//#define PGN_CRC_MZ2								0x36
///*!
//*@def PGN_CRC_MK
//*@brief Номер группы сигналов/команд "КС платы контроля"
//*/
//#define PGN_CRC_MK								0x37
///*!
//*@def PGN_COMM_CRC
//*@brief Номер группы сигналов/команд "КС коммутатора CAN"
//*/
//#define PGN_COMM_CRC							0x3E


#define PGN_NARAB_SEND							0x30
#define PGN_NARAB_REQ							0x31
#define PGN_BHD_RECIEVE							0x32

//Команды для записи наработок в БХД
#define NARAB_INDEX_SEND						0x320
#define NARAB_COMMAND_SEND						0x23

//Команды для чтения наработок из БХД
#define NARAB_INDEX_REQ							0x320
#define NARAB_COMMAND_REQ						0x40

//Команда получения наработки от БХД
#define NARAB_INDEX_RECEIVE						0x320
#define NARAB_COMMAND_RECEIVE					0x43

/*!
*@def INVALID_ANALOG_DATA
*@brief Значение аналогового параметра при отказе (или, если параметр не измеряется)
*/
#define	INVALID_ANALOG_DATA						0x7FFF
/*!
*@def MAX_POSITIVE_VALUE
*@brief Максимальное положительное целочисленное значение
*/
#define	MAX_POSITIVE_VALUE						0x7FFF

/*!
*@def __weak
*@brief Не используемый параметр
*/
#ifndef __weak
	#define	__weak								__attribute__((weak))
#endif

#define	UNUSED(x)								(void)x

#define	MERGE2(m1,m2)							m1##m2
#define	MERGE3(m1,m2,m3)						m1##m2##m3

#define	_MERGE2(m1,m2)							MERGE2(m1,m2)
#define	_MERGE3(m1,m2,m3)						MERGE3(m1,m2,m3)



/*-----------------------------------------------------------------------------
            Внешние переменные
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
            Глобальные функции
-----------------------------------------------------------------------------*/


#endif		/* End of _COMMON_H_ */
