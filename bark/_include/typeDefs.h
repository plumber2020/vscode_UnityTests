//
// TYPEDEFS.H - файл объявлений типов
//
#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include <stdbool.h>
#if !defined(__bool_true_false_are_defined)
typedef enum  { false = 0, true = 1 } bool;
#endif

//------------------------------------------------------------------------------
// описание типов

typedef	char                  int8;
typedef unsigned char         uint8;
typedef short                 int16;
typedef unsigned short	      uint16;
typedef long                  int32;
typedef unsigned long         uint32; 
          
// #ifndef __cplusplus 
//   typedef enum BoolEnum { false, true } bool;
// #endif  

#define	INVALID_ANALOG_VALUE  (int16)0x7FFF
#define IS_VALID(a)           (INVALID_ANALOG_VALUE!=(a))
#define IS_FAULT(a)           (INVALID_ANALOG_VALUE==(a))

//------------------------------------------------------------------------------
//Неиспользуемый параметр в функции для избежания warning'a

#define UNREF_PARAM(p)        p=p

//------------------------------------------------------------------------------

#define ASSERT_FAULT(str)     assert(0,##str)


#endif// _TYPEDEFS_H_

