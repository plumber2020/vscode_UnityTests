/**
 * @file  typeMacros_MOVAVE.c
 * @brief Moving Average Filter
 * 
 */
#include "typeMacros_MOVAVE.h"
#include "typeMacros.h"
#include "typeLimits.h"
 

/** @brief	Moving Average Filter
  * @param 	Desc 
    *
  */
void MOVA_Reset(MOVA_type *Desc)
{
  int16 *Data = (int16*)(Desc+1);
  Desc->Sum = Desc->Divisor = Desc->Head = 0; 
  *Data=0;
}

/** @brief	Moving Average Filter
  * @param 	Desc 
  * @param 	NewVal 
  * @param 	NeedDiv 
  * @return int16 
  */
int16 MOVA_Filter(MOVA_type *Desc, int16 NewVal, bool NeedDiv)
{
  int16 *Data=(int16*)(Desc+1);

  if (Desc->Divisor < Desc->Size) 
  { Desc->Divisor +=1; Desc->Sum += NewVal; }           //накопление данных в сумме
  else                            
  { Desc->Sum = Desc->Sum - Data[Desc->Head] + NewVal;} //замена данных в сумме

  Data[Desc->Head]=NewVal;                              //запись нового элемента
  if (++(Desc->Head) >= (Desc->Size)) Desc->Head=0;     //сдвиг позиции записи
    
  return NeedDiv ? (Desc->Sum/Desc->Divisor)            //возврат среднего
                 : _SATURATE32_INT16_(Desc->Sum);       //возврат суммы
}
 