#include <msp430.h>
#include "switches.h"

char state;

void change_state(button){
  
  switch(button){
  case 0:
    state = 0;
    break;
  case 1:
    state = 1;
    break;
  case 2:
    state = 2;
  case 3:
    state = 3;
  default:
    break;
  }
}
