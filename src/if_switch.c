#include "if_switch.h"


int iResult = 0;
Param eParameter = IDLE;

/**
 * @brief Uses a static local variable counter that persists between function calls.
 * Global Variables:
 * iResult — an integer that gets incremented when the IF_SWITCH_WORK_TIMEOUT is reached.
 * eParameter — a global variable of type Param, used to control the state in the switch statement.
 * @details  Increments counter until it reaches IF_SWITCH_WORK_TIMEOUT.
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
    BREAK_IF_COUNTER_LT(counter, IF_SWITCH_WORK_TIMEOUT);
    iResult += 1; 
    
  default:
    counter = 0;
    eParameter = IDLE;
    break;
  }

}
