/*
===============================================================================
 Name        : Example_Multi_Timer_Blinky.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC8xx.h"
#endif

#include <cr_section_macros.h>

#include "core_cm0plus.h"
#include "Example_Multi_Timer_Blinky.h"
#include "lpc8xx_syscon.h"
#include "lpc8xx_wkt.h"
#include "lpc8xx_mrt.h"
#include "utilities.h"

// Program flow:
// 1. Configure clocking (IRC_Only_SystemInit function in utilities_lib)
//   A. SYSPLLCLKSEL = IRC (which is not used, see clocking diagram in SYSCON chapter of user manual)
//   B. MAINCLKSEL = IRC oscillator
//   C. SYSAHBCLKDIV = 1
// 2. Turn on clocks to peripherals
// 3. Configure the WKT
//    A. Run from IRC/16 (this is the default clock)
//    B. Enable the WKT interrupt
// 4. Configure the SYSTICK
//    A. SYST_CSR(2) = 1 (clksource = system clock div-by-1) SYST_CSR(1) = 1 int enabled
//    B. SYST_RVR = value from header file
//    C. Clear the SYST_CVR register by writing to it
//    D. Enable the interrupt in the NVIC
// 5. Configure the MRT channels 0 and 1
//    A. Write to CTRL registers 1<<0 | 0<<1 (enable interrupt and Repeat mode)
//    B. Enable the MRT0 interrupt in the NVIC
// 6. Enter the main while(1) loop
//    A. Light an LED, start a timer, and wait for two interrupts
//       First one turns off the LED, 2nd one stops the timer and switches to the next LED.
//    B. Proceed to the next timer


volatile enum {false, true} handshake;
volatile uint8_t interrupt_counter;
volatile enum {red, blue, green} current_led;


void activate_current_led() {
  handshake = false;
  switch (current_led) {
    case(red)  : LEDs_On(RED); break;
    case(blue) : LEDs_On(BLUE); break;
    case(green): LEDs_On(GREEN); break;
  }
}


int main(void) {

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL |= (WKT|MRT|GPIO);

  // Configure the WKT ...
  // Give the  module a reset
  LPC_SYSCON->PRESETCTRL &= (WKT_RST_N);
  LPC_SYSCON->PRESETCTRL |= ~(WKT_RST_N);

  // Control register setup
  // WKT clock source is divided IRC (IRC/16)
  LPC_WKT->CTRL = (DIVIDED_IRC<<WKT_CLKSEL);

  // Enable the WKT interrupt in the NVIC
  NVIC_EnableIRQ(WKT_IRQn);

  // Configure the SYSTICK
  // clock = system_clock, tick interrupt enabled
  SysTick->CTRL = (1<<SysTick_CTRL_CLKSOURCE_Pos) | (1<<SysTick_CTRL_TICKINT_Pos);
  // Reload value
  SysTick->LOAD = SYSTICK_TIME;
  // Clear the counter and the countflag bit by writing any value to SysTick_VAL
  SysTick->VAL = 0;
  // Enable the SYSTICK interrupt in the NVIC
  NVIC_EnableIRQ(SysTick_IRQn);

  // Configure the MRT
  // Give the module a reset
  LPC_SYSCON->PRESETCTRL &= (MRT_RST_N);
  LPC_SYSCON->PRESETCTRL |= ~(MRT_RST_N);

  // Mode = repeat, interrupt = enable
  LPC_MRT->Channel[0].CTRL = (MRT_Repeat<<MRT_MODE) | (1<<MRT_INTEN);
  LPC_MRT->Channel[1].CTRL = (MRT_Repeat<<MRT_MODE) | (1<<MRT_INTEN);
  // Enable the MRT interrupt in the NVIC
  NVIC_EnableIRQ(MRT_IRQn);

  Config_LEDs(RED|BLUE|GREEN);

  // Initial values for current LED and interrupt counter
  current_led = red;
  interrupt_counter = 0;

  while(1) {

    // Clear handshake flag, turn on current LED
    activate_current_led();
    // Start the WKT
    LPC_WKT->COUNT = WKT_TIME;
    // Wait until 2 interrupts are complete
    while(!handshake);

    // Clear handshake flag, turn on current LED
    activate_current_led();
    // Start the SYSTICK
    SysTick->CTRL |= 1<<SysTick_CTRL_ENABLE_Pos;
    // Wait until 2 interrupts are complete
    while(!handshake);

    // Clear handshake flag, turn on current LED
    activate_current_led();
    // Start MRT channel 0
    LPC_MRT->Channel[0].INTVAL = MRT0_TIME;
    // Wait until 2 interrupts are complete
    while(!handshake);

    // Clear handshake flag, turn on current LED
    activate_current_led();
    // Start MRT channel 1
    LPC_MRT->Channel[1].INTVAL = MRT1_TIME;
    // Wait until 2 interrupts are complete
    while(!handshake);

  } // end of while(1)

  return 0 ;
} // end of main
