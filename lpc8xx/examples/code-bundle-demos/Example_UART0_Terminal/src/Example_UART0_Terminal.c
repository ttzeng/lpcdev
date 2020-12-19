/*
===============================================================================
 Name        : Example_UART0_Terminal.c
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

#include <stdio.h>

#include "Example_UART0_Terminal.h"
#include "lpc8xx_swm.h"
#include "lpc8xx_syscon.h"
#include "utilities.h"
#include "lpc8xx_uart.h"

unsigned char thestring[] = "What do you have to say for yourself now?\n\r";
extern unsigned char rx_buffer[RX_BUFFER_SIZE];
volatile enum {false, true} handshake;

int main(void) {

  LPC_SYSCON->SYSAHBCLKCTRL |= (UART0 | SWM);

  // Connect UART0 TXD, RXD signals to port pins
  ConfigSWM(U0_TXD, P0_4);         // Use with USB-to-RS232 break-out cable
  ConfigSWM(U0_RXD, P0_0);         // Use with USB-to-RS232 break-out cable
  //ConfigSWM(U0_TXD, TARGET_TX);  // For MBED serial port (requires board mod.)
  //ConfigSWM(U0_RXD, TARGET_RX);  // For MBED serial port (requires board mod.)

  // Configure UART0 for 9600 baud, 8 data bits, no parity, 1 stop bit.
  // For asynchronous mode (UART mode) the formula is:
  // (BRG + 1) * (1 + (m/256)) * (UARTCLKDIV) * (16 * baudrate Hz.) = MainClock Hz.
  // For this example, we set UARTCLKDIV = 2,
  // We proceed in 2 steps.
  // Step 1: Let m = 0, and round (down) to the nearest integer value of BRG for the desired baudrate.
  // Step 2: Plug in the BRG from step 1, and find the nearest integer value of m, (for the FRG fractional part).
  //
  // Step 1 (with m = 0)
  // BRG = ((MainClock Hz.) / (2 * 16 * baudrate Hz.)) - 1
  //     = (60,000,000/(2 * 16 * 9600)) - 1
  //     = 194.3 -> so let BRG = 194.
  //
  // Step 2.
  // m = 256 * [-1 + {(MainClock Hz.) / (2 * 16 * baudrate Hz.)(BRG + 1)}]
  //   = 256 * [-1 + {(60,000,000) / (2*16*9600)(195)}]
  //   = 0.4 -> so let m = 0


  // Configure the UARTCLKDIV
  LPC_SYSCON->UARTCLKDIV = 2;

  // Configure the FRG (nothing to be done here)
  LPC_SYSCON->UARTFRGMULT = 0;
  LPC_SYSCON->UARTFRGDIV = 255;

  // Setup USART0 ...
  // Give USART0 a reset
  LPC_SYSCON->PRESETCTRL &= (UART0_RST_N);
  LPC_SYSCON->PRESETCTRL |= ~(UART0_RST_N);

  // Configure the USART0 baud rate generator
  LPC_USART0->BRG = 194;

  // Configure the USART0 CFG register:
  // 8 data bits, no parity, one stop bit, no flow control, asynchronous mode
  LPC_USART0->CFG = DATA_LENG_8|PARITY_NONE|STOP_BIT_1;

  // Configure the USART0 CTL register (nothing to be done here)
  // No continuous break, no address detect, no Tx disable, no CC, no CLRCC
  LPC_USART0->CTL = 0;

  // Clear any pending flags (Just to be safe, isn't necessary after the peripheral reset)
  LPC_USART0->STAT = 0xFFFF;

  // Enable the USART0 RX Ready Interrupt
  LPC_USART0->INTENSET = RXRDY;
  NVIC_EnableIRQ(UART0_IRQn);


  // Enable USART0
  LPC_USART0->CFG |= UART_EN;

  while(1) {

    PutTerminalString(LPC_USART0, thestring); // See utilities_lib

    handshake = false;                        // Clear handshake flag, will be set by ISR at end of user input
    while (handshake == false);               // Wait here for handshake from ISR

    PutTerminalString(LPC_USART0, rx_buffer); // Echo received string from the rx_buffer to the terminal if desired. Otherwise comment out.

  };

} // end of main

