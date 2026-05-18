#ifndef IF_SWITCH_H
#define IF_SWITCH_H

#include "header.h"

///>Check counter greater-or-equal condition
#define BREAK_IF_COUNTER_LT(CNT,LIMIT)    if((CNT++)< (LIMIT)) { break; }


///>Types
typedef enum {IDLE, WORK} Param;

/// @def IF_SWITCH_WORK_TIMEOUT
/// @brief defines how many times the function must be called in WORK state before incrementing iResult.
#define IF_SWITCH_WORK_TIMEOUT  100


///>Global variables
extern int iResult;
extern Param eParameter;


///>Function declaration
void ifSwitchFunction();



#endif // IF_SWITCH_H
