#include "header.h"

#define WORK_TIMEOUT  100

int iResult = 0;
Param eParameter = IDLE;

/// @brief 
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


