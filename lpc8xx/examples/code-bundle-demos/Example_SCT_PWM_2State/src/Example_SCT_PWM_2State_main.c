/*
===============================================================================
 Name        : Example_SCT_PWM_2State_main.c
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

#include "lpc8xx_syscon.h"
#include "lpc8xx_sct.h"
#include "lpc8xx_swm.h"
#include "utilities.h"

// Pins used in this example:
// P0.17 [Output] - CTOUT_0 (turns on and off the green LED)
// P0.7  [Output] - CTOUT_1 (turns on and off the red LED)
// P0.0  [Input]  - CTIN_0  (High selects state 0 (blink_green), low selects state 1 (blink_red, can you imagine?))

// Macros to define the PWM period and the duty-cycles.
// This example implements 'edge-aligned' PWM.
// The SCT counter will count from 0 up to pwm_period, then start again at 0.
#define pwm_period        10000000
#define C_match_green_ON   1000000
#define C_match_green_OFF  9000000
#define C_match_red_ON     4000000
#define C_match_red_OFF    6000000

enum sct_state {blink_green = 0, blink_red = 1};

int main(void) {

  // Enable clocks to relevant peripherals
  LPC_SYSCON->SYSAHBCLKCTRL |= (SWM|SCT);

  // Configure the SWM (see utilities_lib and lpc8xx_swm.h)
  ConfigSWM(CTIN_0, P0_0);   // SCT input 0 on P0.0, for input transition events (default internal pull-up remains on)
  ConfigSWM(CTOUT_0, P0_17); // SCT output 0 on P0.17, green LED (0=on, 1=off)
  ConfigSWM(CTOUT_1, P0_7);  // SCT output 1 on P0.7, red LED (0=on, 1=off)

  // Configure the SCT ...
  // Give the module a reset
  LPC_SYSCON->PRESETCTRL &= (SCT0_RST_N);
  LPC_SYSCON->PRESETCTRL |= ~(SCT0_RST_N);

  // Configure the CONFIG register
  // UNIFY counter, CLKMODE=busclock, CKSEL=unused(default), NORELOADL/H=unused(default), INSYNC=unused(default), AUTOLIMIT=true
  LPC_SCT->CONFIG |= (1<<UNIFY) |
                     (Bus_clock<<CLKMODE) |
                     (1<<AUTOLIMIT_L);

  // Configure (or should I say control?) the CTRL register
  // Don't run yet, clear the counter, BIDIR=0(default,unidirectional up-count), PRE=0(default,div-by-1)
  LPC_SCT->CTRL_U |= (0<<Stop_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<Halt_L) |       // Stay in halt mode until SCT setup is complete
                     (1<<CLRCTR_L) |     // Clear the counter (good practice)
                     (0<<BIDIR_L) |      // Unidirectional mode (Up-count)
                     (0<<PRE_L);         // Prescaler = div-by-1

  // Setup the LIMIT register
  // No events serve as counter limits because we are using the AUTOLIMIT feature of match0 (see the CONFIG reg. config.)
  LPC_SCT->LIMIT_L = 0;

  // Setup the HALT register
  // No events will set the HALT_L bit in the CTRL reg.
  LPC_SCT->HALT_L = 0;

  // Setup the STOP register
  // No events will set the STOP_L bit in the CTRL reg.
  LPC_SCT->STOP_L = 0;

  // Setup the START register
  // No events will clear the STOP_L bit in the CTRL reg.
  LPC_SCT->START_L = 0;

  // Initialize the COUNT register
  // Start counting at '0'
  LPC_SCT->COUNT_U = 0;

  // Initialize the STATE register
  // Start in state 0
  LPC_SCT->STATE_L = blink_green;

  // Setup the REGMODE register
  // All Match/Capture registers act as match registers
  LPC_SCT->REGMODE_L = 0;

  // Configure the OUTPUT register
  // Initialize CTOUT_0 and CTOUT_1 to '1' so the LEDs are off to begin with
  LPC_SCT->OUTPUT = (1<<0)|(1<<1);

  // Configure the OUTPUTDIRCTRL register
  // The counting direction has no impact on the meaning of set and clear for all outputs
  LPC_SCT->OUTPUTDIRCTRL = 0;

  // Configure the RES (my favorite) register
  // Simultaneous set and clear (which would be a programming error, and won't happen here) has no effect for all outputs
  LPC_SCT->RES = 0;

  // Configure the EVEN register (sadly, there is no ODD register to configure)
  // This example does not use interrupts, so don't enable any event flags to interrupt.
  LPC_SCT->EVEN = 0;

  // Clear any pending event flags by writing '1's to the EVFLAG register
  LPC_SCT->EVFLAG = 0x3F;

  // Configure the CONEN register
  // This example does not use interrupts, so don't enable any 'no-change conflict' event flags to interrupt.
  LPC_SCT->CONEN = 0;

  // Clear any pending 'no-change conflict' event flags, and BUSSERR flags, by writing '1's to the CONLAG register
  LPC_SCT->CONFLAG = 0xFFFFFFFF;

  // Configure the match registers (and their associated match reload registers, which will be the same for this example)
  // for the PWM duty cycles desired
  LPC_SCT->MATCH[0].U =    pwm_period;         // Match0 is the AUTOLIMIT event, determines the period of the PWM
  LPC_SCT->MATCHREL[0].U = pwm_period;         // "
  LPC_SCT->MATCH[1].U =    C_match_green_ON;   // green LED on, CTOUT_0/P0.17 = '0'
  LPC_SCT->MATCHREL[1].U = C_match_green_ON;   // "
  LPC_SCT->MATCH[2].U =    C_match_green_OFF;  // green LED off, CTOUT_0/P0.17 = '1'
  LPC_SCT->MATCHREL[2].U = C_match_green_OFF;  // "
  LPC_SCT->MATCH[3].U =    C_match_red_ON;     // red LED on, CTOUT_1/P0.7 = '0'
  LPC_SCT->MATCHREL[3].U = C_match_red_ON;     // "
  LPC_SCT->MATCH[4].U =    C_match_red_OFF;    // red LED off, CTOUT_1/P0.7 = '1'
  LPC_SCT->MATCHREL[4].U = C_match_red_OFF;    // "

  // Configure the EVENT_STATE and EVENT_CTRL registers for all events
  //
  // Event EVENT_STATE                        EVENT_CTRL
  // ----- ---------------------------------  -------------------------------------------------------------------------------------------
  // EV0   Enabled in State 0 (blink_green).  A match in Match1 is associated with this event (green on), no effect on state.
  // EV1   Enabled in State 0 (blink_green).  A match in Match2 is associated with this event (green off), no effect on state.
  // EV2   Enabled in State 0 (blink_green).  A match in Match0 AND CTIN_0 low is associated with this event, changes state from 0 to 1.
  // EV3   Enabled in State 1 (blink_red).    A match in Match3 is associated with this event (red on), no effect on state.
  // EV4   Enabled in State 1 (blink_red).    A match in Match4 is associated with this event (red off), no effect on state.
  // EV5   Enabled in State 1 (blink red).    A match in Match0 AND CTIN_0 high is associated with this event, changes state from 1 to 0.
  LPC_SCT->EVENT[0].STATE = 1<<blink_green; // Event0 is enabled in state 0
  LPC_SCT->EVENT[1].STATE = 1<<blink_green; // Event1 is enabled in state 0
  LPC_SCT->EVENT[2].STATE = 1<<blink_green; // Event2 is enabled in state 0
  LPC_SCT->EVENT[3].STATE = 1<<blink_red;   // Event3 is enabled in state 1
  LPC_SCT->EVENT[4].STATE = 1<<blink_red;   // Event4 is enabled in state 1
  LPC_SCT->EVENT[5].STATE = 1<<blink_red;   // Event5 is enabled in state 1
  LPC_SCT->EVENT[0].CTRL  = (0x1<<MATCHSEL)|(Match_Only<<COMBMODE)|(0<<STATELD)|(0<<STATEV);
  LPC_SCT->EVENT[1].CTRL  = (0x2<<MATCHSEL)|(Match_Only<<COMBMODE)|(0<<STATELD)|(0<<STATEV);
  LPC_SCT->EVENT[2].CTRL  = (0x0<<MATCHSEL)|(0<<OUTSEL)|(0<<IOSEL)|(LOW<<IOCOND)|(Match_and_IO<<COMBMODE)|(1<<STATELD)|(blink_red<<STATEV);
  LPC_SCT->EVENT[3].CTRL  = (0x3<<MATCHSEL)|(Match_Only<<COMBMODE)|(0<<STATELD)|(0<<STATEV);
  LPC_SCT->EVENT[4].CTRL  = (0x4<<MATCHSEL)|(Match_Only<<COMBMODE)|(0<<STATELD)|(0<<STATEV);
  LPC_SCT->EVENT[5].CTRL  = (0x0<<MATCHSEL)|(0<<OUTSEL)|(0<<IOSEL)|(HIGH<<IOCOND)|(Match_and_IO<<COMBMODE)|(1<<STATELD)|(blink_green<<STATEV);

  // Configure the OUT registers for the 4 SCT outputs
  // Event 0 clears output CTOUT_0 to '0'   (green LED on)
  // Event 1 sets output CTOUT_0 to '1' (green LED off)
  // Event 3 clears output CTOUT_1 to '0'   (red LED on)
  // Event 4 sets output CTOUT_1 to '1' (red LED off)
  LPC_SCT->OUT[0].SET = 1<<event1;
  LPC_SCT->OUT[0].CLR = 1<<event0;
  LPC_SCT->OUT[1].SET = 1<<event4;
  LPC_SCT->OUT[1].CLR = 1<<event3;
  LPC_SCT->OUT[3].SET = 0; // Unused output, set by no event (default)
  LPC_SCT->OUT[3].CLR = 0; // Unused output, cleared by no event (default)

  // FINALLY ... now let's run it. Clearing bit 2 of the CTRL register takes it out of HALT.
  LPC_SCT->CTRL_L &= ~(1<<Halt_L);

  // Go to sleep. No need for the CPU anymore.
  SCB->SCR &= ~(1<<2);  // Clear the SleepDeep bit
  LPC_PMU->PCON = 0x0;  // PCON = 0 selects sleep mode
  __WFI();              // Wait here. No wakeup source has been enabled in this example.

  while(~0) {
  } // end of while(1)

}  // end of main
