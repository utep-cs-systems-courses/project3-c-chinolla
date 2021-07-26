#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-35, screenHeight-8};

short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 30) {		/* 4/sec */
    secCount = 0;
    for (char axis = 0; axis < 2; axis++) {
      short newVal = controlPos[axis] + velocity[axis];
      if (newVal < 10 || newVal > limits[axis])
	velocity[axis] = -velocity[axis];
      else
	controlPos[axis] = newVal;
    }
    fontFgColor = (fontFgColor == COLOR_GREEN) ? COLOR_BLACK : COLOR_GREEN;
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
      if(s1){
	drawString5x7(20, 20, "sup", COLOR_RED, COLOR_BLUE);
    }
    else if(s2){
      drawString5x7(20, 20, "sup", COLOR_BLUE, COLOR_BLUE);
    }
    else if(s3){
      fillRectangle(30, 30, 60, 60, COLOR_RED);
    }
    else if(s4){
      clearScreen(COLOR_BLUE);
    }
      or_sr(8);			/* enable interrupts */
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if(P2IFG & SWITCHES){
    P2IFG &=~SWITCHES;
    switch_interrupt_handler();
  }
}

    



