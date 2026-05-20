//
// TYPELIMITS.H - файл объявлений ограничения типов
//
#ifndef _TYPELIMITS_H_
#define _TYPELIMITS_H_

//------------------------------------------------------------------------------
// Minimum and maximum values a `signed short int' can hold.
#define INT8_MAX              0x7fL
#define INT8_MIN              (-INT8_MAX - 1)
#define UINT8_MAX             0xffUL
#define INT16_MAX             0x7fffL
#define INT16_MIN             (-INT16_MAX - 1)
#define UINT16_MAX            0x7fffUL
#define INT32_MAX             0x7fffffffL
#define INT32_MIN             (-INT32_MAX - 1L)
#define UINT32_MAX            0xffffffffUL

//------------------------------------------------------------------------------

#define _SATURATE16_INT8_(X)    _LIMIT_(X, (int16)INT16_MIN, (int16)INT16_MAX)
#define _SATURATE16_UINT8_(X)   _LIMIT_hiBOUND(X, (uint16)UINT8_MAX)
#define _SATURATE32_INT16_(X)   _LIMIT_(X, (int32)INT16_MIN, (int32)INT16_MAX)
#define _SATURATE32_UINT16_(X)  _LIMIT_hiBOUND(X, (uint32)UINT16_MAX)

#define _SATURATE16_ADD_UINT16_(X,Y) ((X < UINT16_MAX-Y) ? (X+Y) : UINT16_MAX) 
#define _SATURATE32_ADD_UINT32_(X,Y) ((X < UINT32_MAX-Y) ? (X+Y) : UINT32_MAX) 

//------------------------------------------------------------------------------

#endif //_TYPELIMITS_H_

