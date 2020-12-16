Overview
========
The IAP project is a simple demonstration program of the SDK IAP
driver. It reads part id, boot code version, unique id and reinvoke ISP. A message
a printed on the UART terminal as various bascial iap operations are performed.

Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- Keil MDK  5.30
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso812MAX board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the CMSIS DAP port(J3 on the board).
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.

Running the demo
================
1.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.

IAP example

PartID:	XXXXX

The major version is:	XXXX

The minor version is:	XXXX

Unique ID:	XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

End of IAP Example 
Customization options
=====================

