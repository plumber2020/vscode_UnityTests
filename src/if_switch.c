#include "if_switch.h"

/// @def WORK_TIMEOUT
/// @brief defines how many times the function must be called in WORK state before incrementing iResult.
#define WORK_TIMEOUT  100

int iResult = 0;
Param eParameter = IDLE;

/**
 * @brief Uses a static local variable counter that persists between function calls.
 * Global Variables:
 * iResult — an integer that gets incremented when the WORK_TIMEOUT is reached.
 * eParameter — a global variable of type Param, used to control the state in the switch statement.
 * @details  Increments counter until it reaches WORK_TIMEOUT.
 * Once limit is reached, increments iResult by 1 and resets state to IDLE.
 * @note  The function relies on external modification of eParameter. 
          It does not change the state to WORK on its own.
 * 
 */
void ifSwitchFunction() 
{
  static int counter = 0;

  switch (eParameter)
  {
  case WORK:
    if(counter < WORK_TIMEOUT) {counter++;} 
    else { iResult += 1; eParameter = IDLE; }
    break;
  
  default:
    counter = 0;
    break;
  }

}
