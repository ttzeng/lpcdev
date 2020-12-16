/*

${ProjName}:
Example_UART0_Terminal


Intended purpose:
To demonstrate the configuration and use of the USART module, in asynchronous
mode, while communicating with a terminal emulator.


Functional description:
* This example does not use semihosting, so there is no input/output with the
  console. All input/output is accomplished by UART0 communicating with a
  terminal emulator running on the PC, via a USB-to-RS232 breakout cable or
  via the MBED serial port (requires board modifications).
* The user is prompted to enter a string (not to exceed 32 characters, white
  space OK) terminated by [Enter].
* Each character that is entered is echoed back to the terminal by the UART0
  ISR, and the characters are stored in an array for future use.
* When the user types [Enter], a NUL character is appended to the array, and
  a handshake flag is set by the ISR for the main routine.
* Upon seeing the handshake flag, the main routine prints the stored string to
  the terminal, and the process repeats.


External connections:
* To use the MBED Serial Port:
    1. No external connections are necessary.
    2. The Max board must have the necessary solder-bump jumper modifications:
      A. For LPC812 Max, short pins 1 and 2 of both SJ1 and SJ4
      B. For LPC824 Max, short pins 2 and 3 of SJ9
    3. The Windows MBED Serial Port Driver must be installed on the PC.
       https://developer.mbed.org/handbook/Windows-serial-configuration
    4. The terminal emulator can then be connected to the MBED serial port which
       enumerates when the board is connected. It appears something like this: 
       COM13: mbed Serial Port (COM13)
    5. Uncomment the appropriate ConfigSWM() calls in the beginning of main().

* To use a USB-to-RS232 breakout cable:
    1. There are three external connections necessary: RXD and TXD based on the SWM
       settings, plus a ground connection, as shown in the accompanying JPEG (described below).
       P0.4 = U0_TXD = breakout cable RXD
       P0.0 = U0_RXD = breakout cable TXD
       Board GND = breakout cable GND
    2. Uncomment the appropriate ConfigSWM() calls in the beginning of main().



* Notes on the baud rate and FRG:
  The BRG and FRG settings are calculated in two steps, as follows:
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



Program Flow:
* This example runs at 30 MHz (PLL, running from IRC, generating 60 MHz, divided by 2).
  See function SystemInit() in the startup code.

* main() routine
  1. Clocks to USART0 and the SWM are enabled. 

  2. The SWM is configured as per above.

  3. The UARTCLKDIV and the FRG (both in SYSCON) are configured as per above
     (for 9600 b.p.s.).

  4. USART0 is given a peripheral reset.

  5. USART0 BRG is configured as per above (for 9600 b.p.s.). 

  6. USART0 CFG register is configured for:
     8 data bits
     no parity
     one stop bit
     no flow control
     asynchronous mode
     no loop-back

  7. USART0 CTL register is configured for:
     no continuous break
     no address detect
     no Tx disable
     no CC
     no CLRCC

  8. USART0 Rx Ready interrupt is enabled, and code execution enters the main while(1)
     loop.
 
  9. The user is prompted to enter a string, and each character is echoed by the
     ISR back to the terminal until the NUL terminator is encountered.

  10. When the NUL terminator is encounterd, the ISR sets a handshake flag for
      main, which prints the entire received string to the terminal, and the process repeats.
  
 
This example runs from Flash. 


To run this code in LPCXpresso: 1. Build ${ProjName}[Debug]
                                2. Program memory, then reset target
                                or
                                2. Debug ${ProjName}[Debug]
                                3. Click Resume (F8)

                                

*/

