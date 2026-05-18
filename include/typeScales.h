//
// TYPESCALESS.H - файл  объявлений масштабирования типов
//
#ifndef _TYPESCALES_H_
#define _TYPESCALES_H_

#include "typeDefs.h"

//------------------------------------------------------------------------------
//Масштабы представления физических величин 

#define FREQ_SCALE        100		// Масштаб частоты, %
#define LSKR_SKALE        100		// Масштаб угла скрутки, град
#define TEMP_SCALE        10		// Масштаб температур ,°С
#define TCJ_SCALE         10		// Масштаб температуры блока ,°С
#define ANGLE_SCALE       64		// Масштаб угловых величин ,°
#define VIBR_SCALE        100		// Масштаб вибраций, мм/с
#define G_PRESS_SCALE     100		// Масштаб давления для 100 кг/см2 датчиков
#define PRESS_SCALE       1000  // Масштаб давления, кг/см2
#define MOMENT_SCALE      100		// Масштаб крутящего момента кгс•м
#define POWER_SCALE       1     // Масштаб мощности, л.с.
#define VOLUME_SCALE      10		// Масштаб объема, л
#define OM_SCALE          100		// Масштаб Омов для объема, Ом
#define FUEL_SCALE        10		// Масштаб расхода топлива кг/ч
#define PERCENT_SCALE     100		// Масштаб управляющего тока в процентах
#define SPEED_SCALE       100		// Масштаб скорости м/с 
#define AMPER_SHIFT       7
#define AMPER_SCALE       (1<<AMPER_SHIFT) // Масштаб тока в миллиамперах (ИМы)
#define AZ_SHIFT          12
#define AZ_SCALE          (1<<AZ_SHIFT)	   // Масштаб тока в aмперах
#define CNT27_SHIFT       6
#define CNT27_SCALE       (1<<CNT27_SHIFT) // Масштаб напряжения питания = 27
#define SCOBS_SHIFT       8
#define SCOBS_SCALE       (1<<SCOBS_SHIFT) // Масштаб стружкана
#define HERZ_SCALE        10
#define CYCLE_SCALE       100  // масштаб повреждаемостей в циклических наработках

//------------------------------------------------------------------------------
//	Преобразование тиков времени

#ifndef TIME_SCALE
#define TIME_SCALE        40    // Масштаб времени, с
#endif
#define _hour_(tick)      (uint16)((tick)/(3600L*TIME_SCALE))
#define _min_(tick)       (uint16)(((tick)%(3600L*TIME_SCALE))/(60*TIME_SCALE))
#define _sec_(tick)       (uint16)((((tick)%(3600L*TIME_SCALE))%(60*TIME_SCALE))/TIME_SCALE)
#define _fulsec_(tick)    (uint16)((tick)/TIME_SCALE)
#define _tick_(sec)       ((uint16)(sec*TIME_SCALE))

//------------------------------------------------------------------------------
//	Преобразование типов переменных

#define _time_(x)         (uint16)((x)*TIME_SCALE)
#define _longtime_(x)     (uint32)((x)*(uint32)(TIME_SCALE))
#define _freq_(x)         (int16)((x)*FREQ_SCALE)
#define _temp_(x)         (int16)((x)*TEMP_SCALE)
#define _tcj_(x)          (int16)((x)*TCJ_SCALE)
#define _angle_(x)        (int16)((x)*ANGLE_SCALE)
#define _fuel_(x)         (int16)((x)*FUEL_SCALE)
#define _press_(x)        (int16)((x)*PRESS_SCALE)
#define _g_press_(x)      (int16)((x)*G_PRESS_SCALE)
#define _power_(x)        (int16)((x)*POWER_SCALE)
#define _percent_(x)      (int16)((x)*PERCENT_SCALE)
#define _amper_(x)        (int16)((x)*AMPER_SCALE)
#define _az_(x)           (int16)((x)*AZ_SCALE)
#define _cnt27_(x)        (int16)((x)*CNT27_SCALE)
#define _scobs_(x)        (int16)((x)*SCOBS_SCALE)
#define _moment_(x)       (int16)((x)*MOMENT_SCALE)
#define _vibr_(x)         (int16)((x)*VIBR_SCALE)
#define _lskr_(x)         (int16)((x)*LSKR_SKALE)
#define _volume_(x)       (int16)((x)*VOLUME_SCALE)

//------------------------------------------------------------------------------
#define _BORT01_          0x0800  //	признак левого борта (11 и 12 биты для ARINC)
#define _BORT11_          0x1800  //	признак правого борта (11 и 12 биты для ARINC)
#define _BORT00_          0x0000  //	признак левого борта (11 и 12 биты для ARINC)
#define _BORT10_          0x1000  //	признак правого борта 
//------------------------------------------------------------------------------


#endif //_TYPESCALES_H_

