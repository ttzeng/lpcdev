/*
 * board.h
 *
 *  Created on:
 *      Author:
 */

#ifndef BOARD_H_
#define BOARD_H_

#define TARGET_BOARD 812   // For the LPC812 Max board
//#define TARGET_BOARD 824 // For the LPC824 Max board

// LPC812 Max board
#if TARGET_BOARD == 812
#define RED    (1<<7)
#define BLUE   (1<<16)
#define GREEN  (1<<17)
#define PCF8591_address  0x4F      // The on-board ADC/DAC default I2C address
#define PCA9672_address  0x23      // The on-board GPIO Expander default I2C address
#define TARGET_TX P0_6             // For the MBED serial port
#define TARGET_RX P0_1             // For the MBED serial port
#define NUM_IOCON_P0_SLOTS 19      // For looping through the pad controls

// LPC824 Max board
#elif TARGET_BOARD == 824
#define RED    (1<<12)
#define BLUE   (1<<27)
#define GREEN  (1<<16)
#define TARGET_TX P0_7             // For the MBED serial port
#define TARGET_RX P0_18            // For the MBED serial port
#define NUM_IOCON_P0_SLOTS 31      // For looping through the pad controls

#endif

#endif /* BOARD_H_ */
