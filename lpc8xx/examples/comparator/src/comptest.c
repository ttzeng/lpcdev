/****************************************************************************
 *   $Id:: comptest.c 8974 2012-10-31 00:14:35Z usb00423                    $
 *   Project: NXP LPC8xx CMP example
 *
 *   Description:
 *     This file contains Comparator test code example to test
 *     Comparator initialization, COMP interrupt handler, and APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers. This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
****************************************************************************/
#include "LPC8xx.h"                        /* LPC8xx definitions */
#include "lpc8xx_comp.h"
#include "lpc8xx_gpio.h"

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{ 
  SystemCoreClockUpdate();

  GPIOInit();	
  /* Set port p0.7 to output */
  GPIOSetDir( PORT0, 7, 0 );

  COMP_Init();			

  COMP_SetOutput( 0 ); // COMP output async

  COMP_SetHysteresis( 0 ); // hysteresis : none

//  COMP_SelectInput( COMP_VP, 0 ); // COMP_VP -> voltage ladder output
  COMP_SelectInput( COMP_VP, 1 ); // COMP_VP -> COMP_0a
  COMP_SelectInput( COMP_VM, 2 ); // COMP_VM -> COMP_0b
//  COMP_SelectInput( COMP_VM, 6 ); // COMP_VM -> BOD 0.9 V band gap

  COMP_SetInterrupt( 0, 1 ); // single edge, rising edge  						

  while ( 1 )
  {
		if (COMP_GetOutputStatus())
		{
			GPIOSetBitValue( PORT0, 7, 0 );
		}
		else
		{
			GPIOSetBitValue( PORT0, 7, 1 );
		}
  };
}

/******************************************************************************
**                            End Of File
******************************************************************************/

