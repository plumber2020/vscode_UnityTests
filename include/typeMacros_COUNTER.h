/**
 * @file  typeMacros_COUNTER.h
 * @brief Timeout Counter
 * 
 */
#ifndef TIMEOUT_COUNTER_INCLUDED_H_
#define TIMEOUT_COUNTER_INCLUDED_H_

#include "typeDefs.h"
#include "typeMacros.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///>Check counter greater then condition
#define IF_COUNTER_GT(CNT,LIMIT)    \
    if((LIMIT)>0 && (CNT)<=(LIMIT)) { ++CNT; } else /*{}*/

///>Check counter greater-or-equal condition
#define IF_COUNTER_GE(CNT,LIMIT)    \
    if((LIMIT)>0 && (CNT)< (LIMIT)) { ++CNT; } else /*{}*/


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* TIMEOUT_CHECKER
  Таймер по срабатыванию условия с установленной задержкой по времени
  
  Пример использования:

  #define CONDITION_  (x > UPPER_LEVEL) // верхняя граница
  #define TIMEOUT_    _time_(3)         // задержка пересечения границы
  #define EXECUTOR_   flag = 1;         // набор действий по пересечению границы

  {
    DECLARE_TIMEOUT_CHECKER(int16, _SETTER_);

    bool flag = 0;
    CALLING_TIMEOUT_CHECKER(
        _SETTER_, 
        CONDITION_, TIMEOUT_, EXECUTOR_
    );

    if (flag) { } else { }
  }

*/

/** 
 * @brief	Декларирование переменных объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 */
#define DECLARE_TIMEOUT_CHECKER(int_type, name)             \
    static int_type TIMEOUT_CHECKER_counter_##name = 0;     \

/**
 * @brief Сброс счетчиков	объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 */
#define CLEAR_TIMEOUT_CHECKER(name)                         \
      TIMEOUT_CHECKER_counter_##name = 0;                   \

/**
 * @brief Вызов объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 * @param condition, timeout, action - параметры границы
 */
#define CALLING_TIMEOUT_CHECKER(                            \
    name,                                                   \
    condition,timeout,action                                \
  )                                                         \
  if ( condition ) {                                        \
    IF_COUNTER_GE(TIMEOUT_CHECKER_counter_##name, timeout)  \
    { action; }                                             \
  }                                                         \
  else { CLEAR_TIMEOUT_CHECKER(name); }
   

/**
 * @brief	Счетчик объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 */
#define CNT_TIMEOUT_CHECKER(name)  TIMEOUT_CHECKER_counter_##name




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* TIMEOUT_YELLOW_RED
  Таймер по срабатыванию 2-х "вложенных" условий с установленной задержкой по времени.
  При этом :
    1-ое условие YELLOW - слабое, "желтая зона", с более длительным временем срабатывания 
    2-ое условие RED - сильное, "красная зона", с более коротким временем срабатывания
  Состояние режимов { IDLE = 0, YELLOW = 1, RED = 2}

  Пример использования:

  #define UPPER_LEVEL_Y   10                  // верхняя граница - "желтая зона"
  #define UPPER_LEVEL_R   50                  // верхняя граница - "красная зона"
  #define CONDITION_Y     (x > UPPER_LEVEL_Y) // превышение границы "желтая зона"
  #define CONDITION_R     (x > UPPER_LEVEL_R) // превышение границы "красная зона"
  #define TIMEOUT_Y       _time_(5)           // задержка пересечения границы "желтая зона"
  #define TIMEOUT_R       _time_(1)           // задержка пересечения границы "красная зона"
  #define ACTION_SET_Y    {...} // набор действий по пересечению границы "желтая зона"
  #define ACTION_SET_R    {...} // набор действий по пересечению границы "желтая зона"
  #define ACTION_CLR      {...} // набор действий по выходу из "желтая зона"

  {
    DECLARE_TIMEOUT_YELLOW_RED(uint16, _YELLOW_RED_);

    int flag = 0, flag_2= 0;
    CALLING_TIMEOUT_YELLOW_RED(_YELLOW_RED_, 
        CONDITION_Y, TIMEOUT_Y, ACTION_SET_Y,
        CONDITION_R, TIMEOUT_R, ACTION_SET_R,
        ACTION_CLR
    );

  }

*/

/** 
 * @brief	Декларирование переменных объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 * @param int_type целочисленный тип счетчика
 */
#define DECLARE_TIMEOUT_YELLOW_RED(int_type, name)          \
    static int_type TIMEOUT_YR_counter_Y_##name = 0,        \
                    TIMEOUT_YR_counter_R_##name = 0;

/**
 * @brief Сброс счетчиков	объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 */
#define CLEAR_TIMEOUT_YELLOW_RED(name)                      \
      TIMEOUT_YR_counter_Y_##name = 0;                      \
      TIMEOUT_YR_counter_R_##name = 0;

/**
 * @brief Вызов объекта TIMEOUT_CHECKER
 * @param name Именование объекта TIMEOUT_CHECKER
 * @param condition, timeout, action - параметры границы
 */
#define CALLING_TIMEOUT_YELLOW_RED(                         \
        name,                                               \
        condition_Y,timeout_Y, action_Y,                    \
        condition_R,timeout_R, action_R,                    \
        action_CLR                                          \
        )                                                   \
  if ( condition_Y ) {                                      \
    if ( condition_R ) {                                    \
      IF_COUNTER_GE(TIMEOUT_YR_counter_R_##name, timeout_R) \
      { action_R; }                                         \
    } else {                                                \
      TIMEOUT_YR_counter_R_##name = 0;                      \
    }                                                       \
    IF_COUNTER_GE(TIMEOUT_YR_counter_Y_##name, timeout_Y)   \
    { action_Y; }                                           \
  }                                                         \
  else { action_CLR; CLEAR_TIMEOUT_YELLOW_RED(name); }
   



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* TIMEOUT_ONOFF
  Переключатель (Гистерезис) режимов { IDLE = 0, ON = 1, OFF = -1}
  по срабатыванию условия переключения с установленной задержкой по времени
  
  Пример использования:

  #define CONDITION_SET_  (A>B)         // верхняя граница гистерезиса
  #define TIMEOUT_SET_    _time_(0)     // задержка пересечения верхней границы
  #define EXECUTOR_SET_   { ... }       // набор действий по пересечению верхней границы

  #define CONDITION_CLR_  (A<C)         // нижняя граница гистерезиса
  #define TIMEOUT_CLR_    _time_(5)     // задержка пересечения нижней границы
  #define EXECUTOR_CLR_   { ... }       // набор действий по пересечению нижней границы

  //Note! Если  CONDITION_CLR_=!CONDITION_SET_ , 
  //            необходимо явно указать CONDITION_CLR_=_NEGATION_
  //            для правильной оптимизации компилятором 
  //            и исключении дублирующей проверки if( condition_OFF )

  {
    DECLARE_TIMEOUT_ONOFF(uint16, _SWITCHER_);

    int flag = 0;
    if ( A==INVALID_VALUE ) {
      CLEAR_TIMEOUT_ONOFF(_SWITCHER_);
    }
    else {
      CALLING_TIMEOUT_ONOFF(_SWITCHER_,
          CONDITION_SET_, TIMEOUT_SET_, { flag = 1; },
          CONDITION_CLR_, TIMEOUT_CLR_, { flag =-1; }
      );
    }

    switch (flag) {
    case  1: { ... } break;
    case -1: { ... } break;
    default: { ... } break;
    };
  }


*/

#define _NEGATION_(SET_CONDITION)       1

/** 
 * @brief	Декларирование переменных объекта TIMEOUT_ONOFF
 * @param name Именование объекта TIMEOUT_ONOFF
 * @param int_type целочисленный тип счетчика
 */
#define DECLARE_TIMEOUT_ONOFF(int_type, name)                   \
    static int_type TIMEOUT_ONOFF_cnt_ON_##name = 0,            \
                    TIMEOUT_ONOFF_cnt_OFF_##name = 0;

/**
 * @brief Вызов объекта TIMEOUT_ONOFF
 * @param name Именование объекта TIMEOUT_ONOFF
 * @param condition_ON, timeout_ON,  executor_ON  - параметры верхней границы
 * @param condition_OFF,timeout_OFF, executor_OFF - параметры нижней границы
 */
#define CALLING_TIMEOUT_ONOFF(                                  \
        name,                                                   \
        condition_ON, timeout_ON, executor_ON,                  \
        condition_OFF,timeout_OFF,executor_OFF                  \
        )                                                       \
  if ( condition_ON ) {                                         \
    TIMEOUT_ONOFF_cnt_OFF_##name = 0;                           \
    IF_COUNTER_GE(TIMEOUT_ONOFF_cnt_ON_##name,timeout_ON)       \
    { executor_ON; }                                            \
  }                                                             \
  else {                                                        \
    TIMEOUT_ONOFF_cnt_ON_##name = 0;                            \
    if ( condition_OFF ) {                                      \
      IF_COUNTER_GE(TIMEOUT_ONOFF_cnt_OFF_##name,timeout_OFF)   \
      { executor_OFF; }                                         \
    }                                                           \
    else { TIMEOUT_ONOFF_cnt_OFF_##name = 0; }                  \
  }

/**
 * @brief Сброс счетчиков	объекта TIMEOUT_ONOFF
 * @param name Именование объекта TIMEOUT_ONOFF
 */
#define CLEAR_TIMEOUT_ONOFF(name)                               \
    TIMEOUT_ONOFF_cnt_ON_##name = 0;                            \
    TIMEOUT_ONOFF_cnt_OFF_##name = 0;                           \


#endif //TIMEOUT_COUNTER_INCLUDED_H_

