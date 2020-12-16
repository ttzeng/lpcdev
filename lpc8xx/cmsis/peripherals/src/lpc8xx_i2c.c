/*
 * lpc8xx_i2c.c
 *
 *  Created on: Apr 5, 2016
 *
 */

#include "LPC8xx.h"
#include "lpc8xx_i2c.h"

/*****************************************************************************
** Function name:		WaitI2CMasterState
**
** Description:		    Waits for I2C master pending, then compares the master
**                      state to the state parameter. If compare fails, enter
**                      a while(1) loop. If compare passes, return.
**
** parameters:			state: One of the 3-bit Master function state codes of the I2C
** Returned value:		None
**
*****************************************************************************/
void WaitI2CMasterState(uint32_t state) {

  while(!(LPC_I2C->STAT & STAT_MSTPEND));            // Wait for MSTPENDING bit set in STAT register
  if((LPC_I2C->STAT & MASTER_STATE_MASK) != state)   // If master state mismatch ...
    while(1);                                        // die here and debug the code
  return;                                            // If no mismatch, return

}




/*****************************************************************************
** Function name:		WaitI2CSlaveState
**
** Description:		    Waits for I2C slave pending, then compares the slave
**                      state to the state parameter. If compare fails, enter
**                      a while(1) loop. If compare passes, return.
**
** parameters:			state: One of the 2-bit slave function state codes of the I2C
** Returned value:		None
**
*****************************************************************************/
void WaitI2CSlaveState(uint32_t state) {

  while(!(LPC_I2C->STAT & STAT_SLVPEND));         // Wait for SLVPENDING bit in STAT register
  if((LPC_I2C->STAT & SLAVE_STATE_MASK) != state) // If state mismatches
    while(1);                                     // Die here, and debug the problem
  return;                                         // Otherwise, return

}

