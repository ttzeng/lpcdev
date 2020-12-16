/*
 * Example_UART0_Terminal_ISR.c
 *
 *  Created on: Mar 4, 2016
 *      Author:
 */
#include <stdio.h>
#include "LPC8xx.h"
#include "Example_UART0_Terminal.h"

unsigned char rx_buffer[RX_BUFFER_SIZE];
extern volatile enum {false, true} handshake;
static uint32_t rx_char_counter = 0;

#define WaitForUART0txRdy  while(((LPC_USART0->STAT) & (1<<2)) == 0)

/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Description:		    UART0 interrupt service routine.
**                      This ISR reads one received char from the UART0 RXDAT register,
**                      appends it to the rx_buffer array, and echos it back via the
**                      UART0 transmitter. If the char. is 0xD (carriage return),
**                      a new line char (0xA) is appended to the array and echoed,
**                      then a NUL char (0x0) is appended to the array to terminate the string
**                      for future use.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void UART0_IRQHandler() {
  unsigned char temp;

  temp = LPC_USART0->RXDATA;
  rx_buffer[rx_char_counter] = temp;        // Append the current character to the rx_buffer
  WaitForUART0txRdy;                        // Wait for TXREADY
  LPC_USART0->TXDATA = temp;                // Echo it back to the terminal

  if (temp == 0x0D) {                       // CR (carriage return) is current character. End of string.
    rx_buffer[rx_char_counter+1] = 0x0A;    // Append a new line character to rx_buffer.
    rx_buffer[rx_char_counter+2] = 0x00;    // Append a NUL terminator character to rx_buffer to complete the string.
    WaitForUART0txRdy;                      // Wait for TXREADY
    LPC_USART0->TXDATA = 0x0A;              // Echo a NL (new line) character to the terminal.
    handshake = true;                       // Set handshake for main()
    rx_char_counter = 0;                    // Clear array index counter
  }
  else {                                    // Current character is not CR, keep collecting them.
    rx_char_counter++;                      // Increment array index counter.

    if (rx_char_counter == RX_BUFFER_SIZE)  // If the string overruns the buffer, stop here before all hell breaks lose.
      while(1);
  }
  return;
}
