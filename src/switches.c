#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "headers.h"

char s1, s2, s3, s4, switch_state_changed; /* effectively boolean */
char button;
static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  s1 = (p2val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  s2 = (p2val & SW2) ? 0 : 1;
  s3 = (p2val & SW3) ? 0 : 1;
  s4 = (p2val & SW4) ? 0 : 1;

  if(s1){
    button = 0;
  }
  else if(s2){
    button = 1;
  }
  else if(s3){
    button = 2;
  }
  else if(s4){
    button = 3;
  }
  
  
    
  change_state(button);
}
 

