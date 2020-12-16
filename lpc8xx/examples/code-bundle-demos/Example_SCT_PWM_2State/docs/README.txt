/*

${ProjName}:
Example_SCT_PWM_2State


Intended purpose:
 * To demostrate the configuration and use of the SCT to generate a PWM with two different duty cycles.


The following will make more sense while refering to Example_SCT_PWM_2State.jpeg in this directory ...

Functional description:
 * External pin P0.0, configured as CTIN_0, selects between states.
 * External pins P0.17 (green LED) and P0.7 (red LED) are configured as CTOUT_0 and CTOUT_1 respectively.
 * Falling edge on CTIN_0 is defined as Event 2, which is enabled only in State 0, and causes State to go from 0 to 1.
   In State 1 the red LED flashes with a small on-time duty cycle (red LED is on when CTOUT_1 is low).
 * Rising edge on CTIN_0 is defined as Event 5, which is enabled only in State 1, and causes State to go from 1 to 0. 
   In State 0 the green LED flashes with a large on-time duty cycle (green LED is on when CTOUT_0 is low).
 * SCT counter is configured for unified, unidirectional mode, with autolimit. I.e., when COUNT = MATCH0, count resets.
   This determines the period of the PWM waveform.
 * COUNT = MATCH[1] is defined as Event 0, which is enabled only in State 0, and clears CTOUT_0 to '0'.
 * COUNT = MATCH[2] is defined as Event 1, which is enabled only in State 0, and sets CTOUT_0 to '1'.
 * COUNT = MATCH[3] is defined as Event 3, which is enabled only in State 1, and clears CTOUT_1 to '0'.
 * COUNT = MATCH[4] is defined as Event 4, which is enabled only in State 1, and sets CTOUT_1 to '1'.
 
 

Pins used in this example:
 * P0.0  [Input]  - CTIN_0  (falling edge = Event 2, rising edge = Event 5). Note: Internal pull-up is on.
 * P0.17 [Output] - CTOUT_0
 * P0.7  [Output] - CTOUT_1
 
External connections:
* P0.0 can be floated or pulled-up externally, and then grounded to cause events 2 and 5. 
* Grounding P0.0 causes Event 2, pulling P0.0 high causes Event 5.
* See accompanying JPEGs.




Detailed Program Flow:
* This example runs at 30 MHz (PLL, running from IRC, generating 60 MHz, divided by 2).
  See function SystemInit() in the startup code.

* main() routine
   1. Configure the SWM as described above.
   2. Configure the SCT
      A. Operate as one unified 32-bit counter, clocked by bus clock, with autolimit, in unidirectional mode.
      B. No events will limit, halt, stop, or start the counter.
      C. Initial COUNT = 0, initial State = 0.
      D. All Match/Capture registers act as match registers.
      E. Initialize CTOUT_0 and CTOUT_1 to '1'.
      F. Initialize the Match registers for the PWM period, and the duty cycles.
      G. EV0 is enabled in State 0. A match in Match1 is associated with this event, no effect on state.
      H. EV1 is enabled in State 0. A match in Match2 is associated with this event, no effect on state.
      I. EV2 is enabled in State 0. A match in Match0 AND CTIN_0=low is associated with this event, changes State from 0 to 1.
      J. EV3 is enabled in State 1. A match in Match3 is associated with this event, no effect on state.
      K. EV4 is enabled in State 1. A match in Match4 is associated with this event, no effect on state.
      L. EV5 is enabled in State 1. A match in Match0 AND CTIN_0=high is associated with this event, changes state from 1 to 0.
      M. Event 0 clears output CTOUT_0 to '0' (green LED on), Event 1 sets output CTOUT_0 to '1' (green LED off).
      N. Event 3 clears output CTOUT_1 to '0' (red LED on), Event 4 sets output CTOUT_1 to '1' (red LED off).
      O. Run the counter (clear the 'Halt' bit), and put the CPU into Sleep mode, since it is no longer needed.
      

 
This example runs from Flash. 


To run this code in LPCXpresso: 1. Build ${ProjName}[Debug]
                                2. Debug ${ProjName}[Debug]
                                3. Click Resume or F8
                            or:
                                1. Program to flash
                                2. Reset the target


                                

*/

 