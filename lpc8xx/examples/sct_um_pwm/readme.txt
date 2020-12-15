Purpose:
SCT program that implements a PWM with two different duty cycles.
CTIN_0 selects between these two states.
CTIN_0 = 1: Green LED flashes with large duty cycle
CTIN_0 = 0: Red LED flashes with small duty cycle.

Running mode:
* Compile, Flash the program and reset.
* Default project target set to Blinky_Release (executing from flash)

Note:
Tested on LPC800 LPCXpresso Board
LPC800 running at 24 MHz

Input:
PIO0_1 configured as CTIN_0 (available on UART connector)

Output:
PIO0_17 configured as CTOUT_0, Green LED
PIO0_7 configured as CTOUT_1, Red LED


 