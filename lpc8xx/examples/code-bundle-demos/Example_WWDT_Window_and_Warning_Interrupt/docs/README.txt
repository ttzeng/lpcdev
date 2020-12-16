/*

${ProjName}:
Example_WWDT_Window_and_Warning_Interrupt


Intended purpose:
* To demonstrate the configuration and use of the Windowed Watchdog Timer using both its windowing and
  warning interrupt features.


Functional description / configuration:
 * The watchdog oscillator and wdt_osc_clk are configured, the WWDT is configured to count down from
   0x800 with the WINDOW set to 0x400 and the WARNINT set to 0x3FF.
 * The feed sequence is done inside the WARNINT ISR.
 * The green LED is lit, and the main while(1) loop (which does nothing) is entered.
 * If a WWDT reset occurs, the re-entrant code will illuminate the red LED and stop.


External connections:
* None


Detailed Program Flow:
* This example runs at 30 MHz (PLL, running from IRC, generating 60 MHz, divided by 2).
  See function SystemInit() in the startup code.

* main() routine
  1. Turn on peripheral clocks to relevant peripherals.
  2. Examine the WDTOF flag. If set, this current reset is due to a watchdog event. Stop here.
  3. Apply power to the watchdog oscillator.
  4. Configure the WDCLK frequency in the SYSCON module.
     A. WDTOSCCTRL FREQSEL = 0x1 ... Fclkana ~= 0.6 Mhz
     B. WDTOSCCTRL DIVSEL = 0x1F ... wdt_osc_clk = Fclkana/(2*(1 + DIVSEL)) = Fclkana/64 ~= 9,375 Hz
  5. Configure the WWDT
     A. WARNINT = 0x3FF
     B. WINDOW = 0x400
     C. TC = 0x800
     D. MOD = (WDEN=1) | (WDRESET=1)
  6. Enable the WWDT interrupt in the NVIC.
  7. Turn on the green LED.
  8. Do a feed sequence to enable the WWDT.
  9. Enter the main while(1) loop.
     A. WWDT interrupts will feed the WWDT as long as they occur inside the window.
     B. Users can try lowering the WINDOW value so that the WARNINT interrupt, and the feed sequence
        in the ISR, happen while the WDTV register is greater-than-or-equal-to the WINDOW value,
        to verify that the WWDT reset will occur. In this case, the reentrant code will become stuck in the
        while(1) loop of step 2, with the red LED illuminated.

  
  
* This example runs from flash, but does not lend itself to running under a debug session,
  if the watchdog reset is going to be triggerred (by modifying the code as explained above).
  
* It is recommended that the code is compiled (built), then loaded to flash.
  Once loaded, reset the target using the reset button on the board.


To run this code in LPCXpresso: 1. Build ${ProjName}[Debug]
                                2. Debug ${ProjName}[Debug] 
                                OR ...
                                1. Build ${ProjName}[Debug]
                                2. Program the image (${ProjName}/Debug/${ProjName}.axf in LPCXpresso) 
                                3. Press the reset button on the lpc812 Max board
                                

*/


