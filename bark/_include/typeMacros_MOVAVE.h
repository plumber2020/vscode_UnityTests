/**
 * @file  typeMacros_MOVAVE.h
 * @brief Moving Average Filter
 * 
 */
#ifndef MOVING_AVERAGE_INCLUDED_H_
#define MOVING_AVERAGE_INCLUDED_H_

#include "typeDefs.h"

//------------------------------------------------------------------------------
typedef struct {
  uint16 Size,    //размер массива
         Divisor, //делитель (по мере заполнения массива увеличивается с нуля до Size)
         Head;    //позиция записи нового элемента (поверх старого)
  int32  Sum;     //сумма элементов в массиве
}
MOVA_type;
 
void MOVA_Reset(MOVA_type *Desc);
int16 MOVA_Filter(MOVA_type *Desc, int16 NewVal, bool NeedDiv);


//------------------------------------------------------------------------------
/** 
 * @brief	
 * @param 	name 
 * @param 	size 
 */
#define DECLARE_MOVA_FILTER(name,size) \
    static struct { MOVA_type Desc; int16 Data[size]; } \
    name##_struct={size}

/**
 * @brief	
 * @param 	name 
 * @param 	val 
 */
#define CALL_MOVA_FILTER(name,val) \
    MOVA_Filter((MOVA_type*)&name##_struct,val,1)

/**
 * @brief	
 * @param 	name 
 * @param 	val 
 */
#define CALL_MOVA_FILTER_2(name,val) \
    MOVA_Filter((MOVA_type*)&name##_struct,val,0)

/**
 * @brief	
 * @param 	name 
 */
#define RESET_MOVA_FILTER(name) \
    MOVA_Reset((MOVA_type*)&name##_struct)


 
#endif //MOVING_AVERAGE_INCLUDED_H_
