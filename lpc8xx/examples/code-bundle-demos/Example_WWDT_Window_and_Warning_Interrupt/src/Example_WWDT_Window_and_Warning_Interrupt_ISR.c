/*
 * Example_WWDT_Window_and_Warning_Interrupt_ISR.c
 *
 *  Created on: Apr 12, 2016
 *      Author:
 */

#include "LPC8xx.h"
#include "utilities.h"
#include "lpc8xx_wwdt.h"

void WDT_IRQHandler(void) {
  uint32_t temp;

  // Clear the WDINT interrupt flag by writing '1' to it (go figure)
  LPC_WWDT->MOD |= (1<<WDINT);

  // Save the currently enabled interrupts
  temp = NVIC->ISER[0];

  // Disable all interrupts around the feed sequence, re-enable before returning
  NVIC->ICER[0] = 0xFFFFFFFF;
  LPC_WWDT->FEED = 0xAA;
  LPC_WWDT->FEED = 0x55;
  NVIC->ISER[0] = temp;

  return;
}
