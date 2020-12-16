/*
===============================================================================
 Name        : Example_PMU_Modes.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC8xx.h"
#endif

#ifdef __CODE_RED
#include <cr_section_macros.h>
#endif


#include "Example_PMU_Modes.h"
#include "lpc8xx_swm.h"
#include "lpc8xx_syscon.h"
#include "utilities.h"
#include "lpc8xx_uart.h"

unsigned char promptstring[] = "Choose a low power mode:\n\r0 for Sleep\n\r1 for Deep-sleep\n\r2 for Power-down\n\r3 for Deep power-down\n\r";
unsigned char sleepstring[] = "Entering Sleep ... Use pin interrupt (P0.4) to wake up\n\n\r";
unsigned char dsleepstring[] = "Entering Deep-sleep ... Use pin interrupt (P0.4) to wake up\n\n\r";
unsigned char pdstring[] = "Entering Power-down ... Use pin interrupt (P0.4) to wake up\n\n\r";
unsigned char dpdstring[] = "Entering Deep power-down ... Use wakeup pin (P0.4) to wake up\n\n\r";

extern unsigned char rx_buffer[RX_BUFFER_SIZE];
volatile enum {false, true} handshake;


int main(void) {

  int k;
  uint32_t * addr = (uint32_t *)LPC_IOCON_BASE;

  // Enable clocks to UART0, GPIO, IOCON, SWM
  LPC_SYSCON->SYSAHBCLKCTRL |= (UART0 | GPIO | IOCON | SWM);

  // Power down the BOD since we are not using it
  LPC_SYSCON->PDRUNCFG |= BOD_PD;

  // Disable pull-ups and pull-downs.
  // We may leave P0.4 pull-up on, if P0.4 is floated,
  // BUT WE CAN'T FLOAT IT FOR DPD mode, it MUST be pulled high externally!
  //
  // Note: On some LPC8xx parts, there are more slots in the iocon register map than there are GPIO port bits.
  // This is because there is at least one reserved address sitting in the middle of the
  // register address map for some reason. This is why the macro NUM_IOCON_P0_SLOTS,
  // which is defined in board.h, may be greater than the number of GPIO port bits on the chip.
  for (k = 0; k < NUM_IOCON_P0_SLOTS; k++) {
    if  (k == 4) {             // Optionally, leave P0.4 (k = 4) pull-up on
      continue;
    }
    else {
      *(addr + k) &= ~(0x3<<3);
    }
  }

  // Make all ports outputs driving '1', except P0.4 is input
  LPC_GPIO_PORT->DIR0 = 0xFFFFFFBF;
  LPC_GPIO_PORT->PIN0 = 0xFFFFFFFF;

  // Configure P0.4 as PININT7, falling edge sensitive
  LPC_PIN_INT->IST = 0xFF;     // Clear any pending edge-detect interrupt flags
  LPC_SYSCON->PINTSEL[7] = 6;  // P0.4 assigned to PININT7
  LPC_PIN_INT->ISEL = 0x0;     // Edge-sensitive
  LPC_PIN_INT->SIENF = 1<<7;   // Falling-edge interrupt enabled on PININT7

  // Configure PININT7 as wake up interrupt in SysCon block
  LPC_SYSCON->STARTERP0 = 1<<7;

  // Enable pin interrupt 7 in the NVIC
  NVIC_EnableIRQ((IRQn_Type) (PININT7_IRQn));

  // Connect UART0 TXD, RXD signals to port pins P0.6, P0.0
  ConfigSWM(U0_TXD, P0_4);
  ConfigSWM(U0_RXD, P0_0);

  // Configure USART0 as the UART (see Example_PMU_Modes_Funcs.c)
  Setup_UART0_9600();

  // Enable the USART0 RX Ready Interrupt
  LPC_USART0->INTENSET = RXRDY;
  NVIC_EnableIRQ(UART0_IRQn);

  // Enable USART0
  LPC_USART0->CFG |= UART_EN;

  Config_LEDs(GREEN);

  // Turn clocks off to IOCON and SWM since we are done with them
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(IOCON|SWM);

  while(1) {

    LEDs_On(GREEN);

    PutTerminalString(LPC_USART0, promptstring); // See utilities_lib

    handshake = false;                           // Clear handshake flag, will be set by ISR at end of user input
    while (handshake == false);                  // Wait here for handshake from ISR

    LEDs_Off(GREEN);

    switch(rx_buffer[0]) {
      default:
      case(0x30):
        PutTerminalString(LPC_USART0, sleepstring);
        // Prepare for low-power mode
        Setup_Low_Power_Example_PMU_Modes();
        // Clear the SleepDeep bit
        SCB->SCR &= ~(1<<2);
        // PCON = 0 selects sleep mode
        LPC_PMU->PCON = 0x0;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x31):
        PutTerminalString(LPC_USART0, dsleepstring);
        // Prepare for low-power mode
        Setup_Low_Power_Example_PMU_Modes();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 1 selects Deep-sleep mode
        LPC_PMU->PCON = 0x1;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x32):
		PutTerminalString(LPC_USART0, pdstring);
        // Prepare for low-power mode
        Setup_Low_Power_Example_PMU_Modes();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 2 selects Power-down mode mode
        LPC_PMU->PCON = 0x2;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;

      case(0x33):
		PutTerminalString(LPC_USART0, dpdstring);
        // Prepare for low-power mode
        Setup_Low_Power_Example_PMU_Modes();
        // Set the SleepDeep bit
        SCB->SCR |= (1<<2);
        // PCON = 3 selects Deep power-down mode
        LPC_PMU->PCON = 0x3;
        // Wait here for pin interrupt. Startup time begins on falling edge of P0.4
        __WFI();

        break;
    } // end of switch
  } // end of while(1)

  return 0;
} // end of main

