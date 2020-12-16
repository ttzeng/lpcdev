/*
 * lpc8xx_sct.h
 *
 *  Created on: Apr 14, 2016
 *      Author:
 */

#ifndef LPC8XX_SCT_H_
#define LPC8XX_SCT_H_

// CONFIG register bit-field shifters
#define UNIFY       0
#define CLKMODE     1
#define CKSEL       3
#define NORELOAD_L  7
#define NORELOAD_H  8
#define INSYNC      9
#define AUTOLIMIT_L 17
#define AUTOLIMIT_H 18

// CONFIG register values
#define Bus_clock           0
#define Prescaled_bus_clock 1
#define Input               2

#define In0re               0
#define In0fe               1
#define In1re               2
#define In1fe               3
#define In2re               4
#define In2fe               5
#define In3re               6
#define In3fe               7

// CTRL register bit-field shifters
#define DOWN_L   0
#define Stop_L   1
#define Halt_L   2
#define CLRCTR_L 3
#define BIDIR_L  4
#define PRE_L    5
#define DOWN_H   16
#define Stop_H   17
#define Halt_H   18
#define CLRCTR_H 19
#define BIDIR_H  20
#define PRE_H    21

// EVENT_STATE register values
#define state0 0
#define state1 1

// EVENT_CTRL register bit field shifters
#define MATCHSEL  0
#define HEVENT    4
#define OUTSEL    5
#define IOSEL     6
#define IOCOND    10
#define COMBMODE  12
#define STATELD   14
#define STATEV    15
#define MATCHMEM  20
#define DIRECTION 21

//  EVENT_CTRL register bit field values
#define LOW       0
#define Rise      1
#define Fall      2
#define HIGH      3
#define Match_or_IO  0
#define Match_Only   1
#define IO_Only      2
#define Match_and_IO 3
#define Direction_Independent 0
#define Counting_Up           1
#define Counting_Down         2

// OUT register bit field shifters
#define event0 0
#define event1 1
#define event2 2
#define event3 3
#define event4 4
#define event5 5

// Event state mask register
#define state0 0
#define state1 1



#endif /* LPC8XX_SCT_H_ */
