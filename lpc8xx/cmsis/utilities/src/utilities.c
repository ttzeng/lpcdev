/*
 * utilities.c
 *
 *  Created on: Feb 15, 2016
 *
 */
#include "LPC8xx.h"
#include "lpc8xx_syscon.h"
#include "lpc8xx_swm.h"
#include "lpc8xx_uart.h"
#include <stdio.h>


/*****************************************************************************
** Function name:		ConfigSWM
**
** Description:			Attaches a pin (designated by its GPIO port and bit numbers)
**                      to a function.
**
** parameters:			func: Any function name that is movable.
**                      port_pin: Any pin which has a GPIO port number and bit number.
**
** Returned value:		None
**
*****************************************************************************/
void ConfigSWM(uint32_t func, uint32_t port_pin) {
  uint32_t temp, i;
  uint32_t pinassign = 0;
  uint32_t shifter = 0;

  for (i=0; i<num_funcs; i++) {
    if (func != i)
      continue;
    else {
      pinassign = i/4;
      shifter = (i%4)*8;
      break;
    }
  }

  temp = LPC_SWM->PINASSIGN[pinassign];
  temp &= ~(0xFF<<(shifter));
  temp |= (port_pin << shifter);
  LPC_SWM->PINASSIGN[pinassign] = temp;

  return;
}

/*****************************************************************************
** Function name:		PutTerminalString
**
** Description:			This function writes a string, one character at a time, to
**                      a USART. The string MUST be terminated by a terminating
**                      NUL ("\0") character. Both the USART instance and the string
**                      are passed into the function by reference using pointers.
**
** parameters:			ptr_LPC_USART: A pointer to any USART on the target.
**                      ptr_string: A pointer to an array of char terminated by NUL.
**
** Returned value:		void
**
*****************************************************************************/
void PutTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string) {
  uint32_t i = 0;
  uint8_t temp;

  do {
    temp = ptr_string[i++];                   // Get the current character
	while (!((ptr_LPC_USART->STAT) & TXRDY)); // Wait for TX Ready
	ptr_LPC_USART->TXDATA = temp;             // Write the character to the TX buffer
  } while (temp != 0);                        // NUL character terminates
  return;
}



/*****************************************************************************
** Function name:		GetTerminalString
**
** Description:			This function accepts a string, one character at a time,
**                      from a USART. Both the USART instance and the array in
**                      which to store the string are passed into the function
**                      by reference using pointers.
**
** parameters:			ptr_LPC_USART: A pointer to any USART on the target.
**                      ptr_string: A pointer to an array of char.
**
** Returned value:		void
**
*****************************************************************************/
void GetTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string) {
  uint32_t i = 0;
  uint8_t temp = 0;

  while (1) {
    while (!((ptr_LPC_USART->STAT) & RXRDY));   // Wait for RX Ready
    temp = ptr_LPC_USART->RXDATA;               // Read the current character from RXDATA
    ptr_string[i++] = temp;                     // Append the current character to the rx buffer
    while (!((ptr_LPC_USART->STAT) & TXRDY));   // Wait for TXREADY
    ptr_LPC_USART->TXDATA = temp;               // Echo it back to the terminal

    if (temp == 0x0D) {                         // CR (carriage return) is current character or buffer full. End of string.
      ptr_string[i] = 0x0A;                     // Append a new line character to rx_buffer.
      ptr_string[i+1] = 0x00;                   // Append a NUL terminator character to rx_buffer to complete the string.
      while (!((ptr_LPC_USART->STAT) & TXRDY)); // Wait for TXREADY
      ptr_LPC_USART->TXDATA = 0x0A;             // Echo a NL (new line) character to the terminal.
      return;
    }
  }
}



/*****************************************************************************
** Function name:		GetConsoleString
**
** Description:			Accepts non-whitespace characters from the console,
**                      terminated by the [Enter] key, and stores them in an array
**                      as a string. The NUL string terminator is added to the end
**                      of the array by the [Enter] key.
**                      The scanf function treats whitespace characters as string
**                      terminators, so please avoid them.
**                      The number of characters is currently limited to 32 (31 plus
**                      NUL terminator).
**
** parameters:			None
**
** Returned value:		void
**
*****************************************************************************/
void GetConsoleString(char * ptr_string) {
  printf("Enter a string (with no white-space characters) terminated by [Enter]: \n");
  scanf("%s", ptr_string);
}



/*****************************************************************************
** Function name:		GetConsoleCharacter
**
** Description:			Returns one non-whitespace character from the console,
**                      terminated by the [Enter] key.
**                      The scanf function treats whitespace characters as string
**                      terminators, so please avoid them.
**
** parameters:			pPromptString: A pointer to a NUL-terminated prompt string.
**
** Returned value:		unsigned char: The character typed before [Enter]
**
*****************************************************************************/
unsigned char GetConsoleCharacter(const char * pPromptString) {
  uint8_t j[10];
  printf(pPromptString);  // Print the prompt to the console
  scanf("%2c", &j);       // Store the last character before CR in j[0]
  return(j[0]);
}



/*****************************************************************************
** Function name:		WaitConsoleEnter
**
** Description:			Waits for the user to type [Enter], then returns.
** parameters:			pPromptString: A pointer to a NUL-terminated prompt string.
**
** Returned value:		void
**
*****************************************************************************/
void WaitConsoleEnter(const char * pPromptString) {
  unsigned char j[10];
  printf(pPromptString);
  scanf("%1c", &j);
  return;
}



/*****************************************************************************
** Function name:   DebugWaitEsc
** Description:     Waits for 'Esc' char in the debug UART's RX data register
**                  then returns.
** parameters:      None
** Returned value:  void
*****************************************************************************/
void DebugWaitEsc(void) {
  printf("Type 'Esc' to continue\n\r");
  while (1) {
    while (!((LPC_USART0->STAT) & RXRDY));   // Wait for RX Ready
		if (LPC_USART0->RXDATA == 0x1B) {        // Read the current character from RXDATA
			return;                                // If it's Esc, return, else loop again
		}
	}
}



/*****************************************************************************
** Function name:		GetConsoleInput
**
** Description:			Accepts characters from the console and concatenates them
** 						into an unsigned hex integer. Each number is terminated
** 						by the Enter key. The correct number of digits for the
** 						size of the number must be entered (one digit for 4-bits, two
** 						for 8-bits, etc.) including leading zeros if any.
** 						Only characters 0 - 9, a - f, A - F are processed.
** 						All others are treated as '0'.
**
** parameters:			number_of_digits (currently 1 and 2 are supported).
**
** Returned value:		The resulting unsigned integer
**
*****************************************************************************/
uint32_t GetConsoleInput(uint32_t number_of_digits) {
  uint32_t temp;
  uint32_t num[10];
  unsigned char j[10];
  uint32_t n;

  printf("Enter a %d-digit hex number (with no prefix): \n", number_of_digits);
  switch (number_of_digits) {
    default:
    case 1: scanf("%2c", &j); break;   // NUM_DIGITS = 1
    case 2: scanf("%3c", &j); break;   // NUM_DIGITS = 2
  //case 4: scanf("%5c", &j); break;   // NUM_DIGITS = 4
  //case 8: scanf("%9c", &j); break;   // NUM_DIGITS = 8
  }

  for (n = 0; n < number_of_digits; n++) {

    if ( ((j[n]<0x30) || (j[n]>0x66)) || ((j[n] > 0x39) && (j[n] < 0x41)) || ((j[n] > 0x46) && (j[n] < 0x61)) ) {
      printf("Invalid hex digit, ascii code 0x%x. Using '0' instead.\n", j[n]);
  	  j[n] = '0';
    }

    if (j[n] >= 0x61) {
      num[n] = j[n] - 0x57;
    }
    else {
      if (j[n] >= 0x41) {
  	  num[n] = j[n] - 0x37;
      }
      else {
  	  num[n] = j[n] - 0x30;
      }
    }

  } // end of for loop

  temp = 0;
  while(n--)
  {
    temp |= num[n]<<(4*((number_of_digits-1)-n)); // Reverse the order in which digits are received
  }

  return(temp);

} // End of function GetConsoleInput



/*****************************************************************************
** Function name:		Config_LEDs
**
** Description:			Makes port bit(s) an output driving '1' (for LED off).
** 						Typically called with '1' in any port bit positions
** 						which have a board LED attached.
**
** parameters:			bits ('1' in a bit position corresponding to an LED)
**
** Returned value:		None
**
*****************************************************************************/
void Config_LEDs(uint32_t bits) {
  LPC_GPIO_PORT->SET0  = bits;           // Write '1's to bits corresponding to requested LEDs
  LPC_GPIO_PORT->DIR0 |= bits;           // Make those bits outputs
}



/*****************************************************************************
** Function name:		LEDs_Off
**
** Description:			Makes port bit(s) drive '1' (for LED off).
** 						Typically called with '1' in any port bit positions
** 						which have a board LED attached.
**
** parameters:			bits ('1' in a bit position corresponding to an LED)
**
** Returned value:		None
**
*****************************************************************************/
void LEDs_Off(uint32_t bits) {
  LPC_GPIO_PORT->SET0  = bits;           // Write '1's to bits corresponding to requested LEDs
}



/*****************************************************************************
** Function name:		LEDs_On
**
** Description:			Makes port bit(s) drive '0' (for LED on).
** 						Typically called with '1' in any port bit positions
** 						which have a board LED attached.
**
** parameters:			bits ('1' in a bit position corresponding to an LED)
**
** Returned value:		None
**
*****************************************************************************/
void LEDs_On(uint32_t bits) {
  LPC_GPIO_PORT->CLR0  = bits;           // Write '0's to bits corresponding to requested LEDs
}



/*****************************************************************************
** Function name:		Setup_LPC8xx_Low_Power
**
** Description:		    Sets PDAWAKECFG = PDRUNCFG and turns off some clocks.
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void Setup_LPC8xx_Low_Power() {

  // Configure PDAWAKECFG
  LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;

  // Turn clocks off to unused peripherals
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(SWM | GPIO | IOCON);

  return;
}



/*****************************************************************************
** Function name:		IRC_Only_SystemInit
**
** Description:		    Configures the clocking to run directly from the 12 MHz IRC
**
** Parameters:			None
** Returned value:		void
**
*****************************************************************************/
void IRC_Only_SystemInit(void) {
  LPC_SYSCON->SYSPLLCLKSEL  = 0;                  // Selects IRC as input to the PLL which we won't be using
  LPC_SYSCON->SYSPLLCLKUEN  = 1;                  // Update Clock Source
  while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));     // Wait Until Updated

  LPC_SYSCON->MAINCLKSEL    = 0;                  // Select IRC as input to main clock select mux
  LPC_SYSCON->MAINCLKUEN    = 1;	              // Update MCLK Clock Source
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01));       // Wait Until Updated

  LPC_SYSCON->SYSAHBCLKDIV  = 1;                  // System clock = main clock div-by-1

  LPC_FLASHCTRL->FLASHCFG = 0;                    // FLASH access time is always 1 system clock
}


