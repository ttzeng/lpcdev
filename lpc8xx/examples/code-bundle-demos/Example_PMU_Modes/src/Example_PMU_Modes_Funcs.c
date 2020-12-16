/*
 * Example_PMU_Modes_Funcs.c
 *
 *  Created on: Mar 24, 2016
 *      Author: usb06326
 */

#include "LPC8xx.h"
#include "lpc8xx_syscon.h"
#include "lpc8xx_uart.h"
#include "utilities.h"
#include "lpc8xx_swm.h"
#include "Example_PMU_Modes.h"


unsigned char rx_buffer[RX_BUFFER_SIZE];
extern volatile enum {false, true} handshake;
const static uint32_t rx_char_counter = 0;

#define WaitForUART0txRdy  while(((LPC_USART0->STAT) & (1<<2)) == 0)


/*****************************************************************************
** Function name:		PININT7_IRQHandler
**
** Description:		    Pin interrupt 7 ISR.
**                      This is the wake up ISR for Sleep, Deep-sleep, and Power-down modes.
**                      This ISR turns on the green LED (by driving P0.17 low),
**                      clears all pin interrupt flags, clears the SleepDeep
**                      bit, restores clocks to GPIO and UART0 for main, then
**                      returns.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void PININT7_IRQHandler(void)
{

  // Startup time ends on this falling edge of P0.17 (green LED on)
  LEDs_On(GREEN);

  // Clear the pin interrupt, and any other irrelevant edge-detect flags
  LPC_PIN_INT->IST = 0xFF;

  // Clear the SleepDeep bit
  SCB->SCR &= ~(1<<2);

  // Activate clocks to the needed peripherals for normal mode if they are off
  LPC_SYSCON->SYSAHBCLKCTRL |= (UART0 | GPIO);

  return;
}




/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Description:		    UART0 interrupt service routine.
**                      This ISR reads one received char from the UART0 RXDAT register,
**                      stores it in the rx_buffer array, and echos it back via the
**                      UART0 transmitter. If the char. is 0xD (carriage return),
**                      both a new line char (0xA) and a CR (0xD) are echoed.
**                      Only '0', '1', '2', '3', and 'CR' are handled, any other
**                      value is ignored.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void UART0_IRQHandler() {
  unsigned char temp;

  temp = LPC_USART0->RXDATA;                // Get the current character

  if ((temp >= 0x30 && temp <= 0x33)) {     // Only integers 0-3 and CR are accepted
    rx_buffer[rx_char_counter] = temp;      // Append the current character to the rx_buffer
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDATA = temp;              // Echo it back to the terminal
  }

  if (temp == 0x0D) {                       // CR (carriage return) is current character. End of entry.
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDATA = 0x0A;              // Echo a NL (new line) character to the terminal.
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDATA = 0x0D;              // Echo a CR character to the terminal.
    handshake = true;                       // Set handshake for main()
  }

  return;
}




/*****************************************************************************
** Function name:		Setup_UART0_9600
**
** Description:		    Configures UART0 for 9600, 8 bits, no parity, 1 stop
**                      when PCLK_UART0 = 12 MHz.
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void Setup_UART0_9600() {
  // Configure UART0 for 9600 baud, 8 data bits, no parity, 1 stop bit.
  // For asynchronous mode (UART mode) the formula is:
  // (BRG + 1) * (1 + (m/256)) * (UARTCLKDIV) * (16 * baudrate Hz.) = MainClock Hz.
  // For this example, we set UARTCLKDIV = 1,
  // We proceed in 2 steps.
  // Step 1: Let m = 0, and round (down) to the nearest integer value of BRG for the desired baudrate.
  // Step 2: Plug in the BRG from step 1, and find the nearest integer value of m, (for the FRG fractional part).
  //
  // Step 1 (with m = 0)
  // BRG = ((MainClock Hz.) / (16 * baudrate Hz.)) - 1
  //     = (12,000,000/(16 * 9600)) - 1
  //     = 77.125 -> so let BRG = 77
  //
  // Step 2.
  // m = 256 * [-1 + {(MainClock Hz.) / (16 * baudrate Hz.)(BRG + 1)}]
  //   = 256 * [-1 + {(12,000,000) / (16*9600)(78)}]
  //   = 0.41 -> so let m = 0

  // Configure the UARTCLKDIV
  LPC_SYSCON->UARTCLKDIV = 1;

  // Configure the FRG (nothing to be done here)
  LPC_SYSCON->UARTFRGMULT = 0;
  LPC_SYSCON->UARTFRGDIV = 255;

  // Setup USART0 ...
  // Give USART0 a reset
  LPC_SYSCON->PRESETCTRL &= (UART0_RST_N);
  LPC_SYSCON->PRESETCTRL |= ~(UART0_RST_N);

  // Configure the USART0 baud rate generator
  LPC_USART0->BRG = 77;

  // Configure the USART0 CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, asynchronous mode
  LPC_USART0->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1;

  // Configure the USART0 CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART0->CTL = 0;

  // Clear any pending flags (Just to be safe, isn't necessary after the peripheral reset)
  LPC_USART0->STAT = 0xFFFF;

  return;
}



/*****************************************************************************
** Function name:		Setup_Low_Power_Example_PMU_Modes
**
** Description:		    Copies PDRUNCFG contents into PDAWAKECFG register and
**                      turns off clocks to UART0, IOCON, and SWM.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void Setup_Low_Power_Example_PMU_Modes() {

  LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;

  // Turn clocks off to unused peripherals (Leave GPIO on!)
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(UART0 | IOCON | SWM);

  return;
}

