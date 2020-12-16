/*

${ProjName}:
Example_PMU_Modes


Intended purpose:
* To demonstrate the entry into, and wakeup from, the four low-power modes, 
  while allowing the user to measure current consumption and wakeup time.


Functional description:
* This example does not use semihosting, so there is no input/output with the
  console. All input/output is accomplished by UART0 communicating with a
  terminal emulator running on the PC, via a USB-to-RS232 breakout cable.

* In order to measure the target's current consumption, the lpc812 Max board
  must be modified as follows (please see sheet 2 of the LPC812 Max Schematic Rev. 3):
  1. Remove the zero-Ohm resistor R23.
  2. Install a male header (HDR-1x02) across JP2 to accept the multimeter clip leads.
     When not using a meter across JP2, don't forget to insert a jumper there instead.

* Port pin P0.4 (a.k.a. the wakeup pin) is used to wakeup the target from the low power modes.

* A falling edge on P0.4 is used to trigger the wakeup interrupt from Sleep, Deep-sleep,
  and Power-down modes.=

* Grounding an EXTERNALLY pulled up P0.4 will wakeup the target from Deep power-down mode.
  P0.4 MUST be pulled high externally to enter Deep power-down mode.

* If the P0.4 internal pull-up is configured active during Sleep, Deep-sleep, and
  Power-down modes, then P0.4 can remain floating during those modes.

* The user is prompted to enter '0', '1', '2', or '3' (terminated by [Enter]) to enter
  Sleep, Deep-sleep, Power-down, or Deep power-down modes, respectively.

* The target enters the low power mode selected, and is awakened by grounding P0.4.


External connections:
* RS232 breakout cable RXD and TXD based on the SWM settings, plus a ground connection.
  Also, an external pullup on P0.4, in which the P0.4 side can be momentarliy grounded.
  Please see the accompanying JPEG.

* // Connect UART0 TXD, RXD signals to port pins P0.6, P0.0
* P0.6 = U0_TXD = breakout cable RXD
* P0.0 = U0_RXD = breakout cable TXD
* Board GND = breakout cable GND


Notes on the baud rate and FRG:
* For this example, only one of the three clock dividers available to the
  USART are used (FRG and UARTCLKDIV are unnecessary). The UARTCLKDIV in 
  SYSCON is programmed for divide-by-1. This is sufficient to produce a 
  baud rate close enough to 9,600 b.p.s. based on the 12 MHz IRC.
  Similarly for the FRG, it is programmed to divide-by-1.
* The BRG and FRG settings are calculated in two steps, as follows:
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
  //     = 77.25 -> so let BRG = 77.
  //
  // Step 2.
  // m = 256 * [-1 + {(MainClock Hz.) / (16 * baudrate Hz.)(BRG + 1)}]
  //   = 256 * [-1 + {(12,000,000) / (16*9600)(78)}]
  //   = 0.4 -> so let m = 0


Program Flow:
* This example runs using the 12MHz IRC and no PLL.
  See funtion IRC_Only_SystemInit() (in utilities_lib), and the startup code.

* main() routine
  1. Clocks are enabled. 
  
  2. The BOD is powered down.

  3. Internal pull-ups (and pull-downs) are disabled in IOCON.
  
  4. GPIO ports are configured.
  
  5. P0.4 is configured as PININT7, falling edge sensitive, made the wakeup source, and interrupt enabled.
  
  6. The SWM is configured for UART0 TxD on P0.6 and RxD on P0.0.
  
  7. UART0 and its clocking are configured for 9600, 8 bits, no parity, one stop bit.

  8. USART0 Rx Ready interrupt is enabled, some clocks are disabled,
     and code execution enters the main while(1) loop.
 
  9. The user is prompted to enter '0', '1', '2', or '3' followed by [Enter], to enter one of the low power modes
     (Sleep, Deep-sleep, Power-down, or Deep power-down respectively).

  10. A logic low on P0.4 will wake up the target and the process repeats.
  
  Please note: Spurious characters often appear on the terminal when a low power mode is entered.
  You may want to clear the terminal's buffer when this happens, so that subsequent output is legible
  if another low power mode entry cycle is intended. The terminal may be confused, but the UART on the
  target is not, and will continue to respond correctly.
  
  
 
This example runs from Flash. 


To run this code in LPCXpresso: 1. Build ${ProjName}[Debug]
                                2. Program the image to flash (${ProjName}/Debug/${ProjName}.axf in LPCXpresso) 
                                3. Launch your terminal emulator and connect to the appropriate USB COM port (using 9600, 8, no, 1)
                                4. Press the reset button on the lpc812 Max board and follow the prompts.

                                

*/

