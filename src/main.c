#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "headers.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-35, screenHeight-8};
int x = 50;
int y = 50;
char signed state = 4;

short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 30) {		/* 4/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  switch_init();
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      and_sr(~8);
      if(state != 4){
	P1OUT |= LED;
	switch(state){
	case 0:
	  drawString5x7(35, 30, "hello", COLOR_GREEN, COLOR_BLUE);
	  state = 4; // returns to state 3 which turns of cpu
	  break;
	case 1:
	  clearScreen(COLOR_BLUE);
	  fillRectangle(x, y, 60, 60, COLOR_GREEN);
	  x = x - 4;
	  state = 4;
	  break;
	case 2:
	  clearScreen(COLOR_BLUE);
	  fillRectangle(x, y, 60 ,60, COLOR_GREEN);
	  x = x + 4;
	  state = 4;
	  break;
	case 3:
	  clearScreen(COLOR_BLUE);
	  state = 4;
	  break;
	}
      }
      else
	{
	  P1OUT &= ~LED;
	}
      or_sr(8);			/* enable interrupts */
    }
    or_sr(0x10);
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SWITCHES){
    P2IFG &=~SWITCHES;
    switch_interrupt_handler();
  }
}

    



