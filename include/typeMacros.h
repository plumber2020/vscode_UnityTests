//
// TYPEMACROS.H - файл объявлений макросов
//
#ifndef _TYPEMACROS_H_
#define _TYPEMACROS_H_

//------------------------------------------------------------------------------
//Проверка условий на этапе компиляции

#define STATIC_CHECK(condition, msg)  extern int near msg[(condition)?1:0xFFFF]

//------------------------------------------------------------------------------
//Склейка
#define MERGE(m1, m2)             m1##m2
#define MERGE3(m1, m2, m3)        m1##m2##m3
#define _MERGE(m1, m2)            MERGE(m1, m2)
#define _MERGE3(m1, m2, m3)       MERGE3(m1, m2, m3)

//------------------------------------------------------------------------------
//Операции с массивами

#ifndef ARRAY_SIZE
  #define ARRAY_SIZE(x)           (sizeof(x)/sizeof(x[0]))
#endif
#define _SIZE_OF_(name)           ARRAY_SIZE(name)
#define ARRAY_CLEAR(name)         memset(name,0,sizeof(name));
#define STRUCT_CLEAR(name)        memset(&name,0,sizeof(name));

#define _LIMIT_(x,lo,hi)          ((x)<(hi)?((x)>(lo)?(x):(lo)):(hi))
#define _LIMIT_loBOUND(x,lo)      ((x)>(lo)?(x):(lo))
#define _LIMIT_hiBOUND(x,hi)      ((x)<(hi)?(x):(hi))
#define _LIMIT_exCHANGE(x,lo,hi)  ((x)<(hi)?(x):(lo))

#define _INCR_lo2hiBOUND(x,lo,hi) ((++x)<(hi)?(x):(x=lo))
#define _DECR_hi2loBOUND(x,lo,hi) ((x)<=(lo)?(x=hi-1):(--x))

//------------------------------------------------------------------------------

#define _MAX_SATURATE(x,hi)       if ((x)>(hi)) x=hi;
#define _MIN_SATURATE(x,lo)       if ((x)<(lo)) x=lo;

#define _MAX_(a,b)                (((a)>(b))?(a):(b))
#define _MIN_(a,b)                (((a)<(b))?(a):(b))

#define _MAX_ELEM_(val,p1,p2)     val=*p1; while(++p1<p2){ if(val<*p1) val=*p1; } 
#define _MIN_ELEM_(val,p1,p2)     val=*p1; while(++p1<p2){ if(val>*p1) val=*p1; }
#define _MAX_INDX_(ind,p1,p2)     { int j=ind; while(j+p1<p2) { if(p1[ind]<p1[j]) ind=j; ++j;}}
#define _MIN_INDX_(ind,p1,p2)     { int j=ind; while(j+p1<p2) { if(p1[ind]>p1[j]) ind=j; ++j;}}

#define _MINMAX_CHECK_(x,min,max) { if((x)<(min)) min=x; else if((x)>(max)) max=x; }

//------------------------------------------------------------------------------

#define _FILL_BUFF_(buf,n,init) { int16 k=0; while(k<n) { buf[k++]= init; } }
#define _IOTA_BUFF_(buf,n,init) { int16 k=0; while(k<n) { buf[k++] = init++; } }

//------------------------------------------------------------------------------

#define LOG2_32768              15
#define LOG2_16384              14
#define LOG2_8192               13
#define LOG2_4096               12
#define LOG2_2048               11
#define LOG2_1024               10
#define LOG2_512                9
#define LOG2_256                8
#define LOG2_128                7
#define LOG2_64                 6
#define LOG2_32                 5
#define LOG2_16                 4
#define LOG2_8                  3

//Показатель степени двойки
#define LOG2_2N(N)              _MERGE(LOG2_,N)   

//------------------------------------------------------------------------------
//макросы выхода

//RETURN
#define RETURN_IF(RET, FUNC)    RET = FUNC; if (RET<0) return RET;
#define CONTINUE_IF(RET, FUNC)  RET = FUNC; if (RET<0) continue;
#define BREAK_IF(RET, FUNC)     RET = FUNC; if (RET<0) break;


#endif //_TYPEMACROS_H_
