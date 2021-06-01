/****************************************************************************
 *   $Id:: wdttest.c 7150 2012-10-31 21:33:24Z usb01267                     $
 *   Project: NXP LPC8xx WDT example
 *
 *   Description:
 *     This file contains WDT test modules, main entry, to test WDT APIs.
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
#include "LPC8xx.h"			/* LPC8xx Peripheral Registers */
#include "lpc8xx_mrt.h"
#include "lpc8xx_clkconfig.h"
#include "lpc8xx_wdt.h"
#include "lpc8xx_gpio.h"

extern uint32_t mrt_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
	uint32_t regVal;
	
	SystemCoreClockUpdate();

	GPIOInit();

	/* Config WDT clock */
	WDT_CLK_Setup();
  
  /* Config CLKOUT, mostly used for debugging. */
	regVal = LPC_SWM->PINASSIGN8 & ~( 0xFF << 16 );
	LPC_SWM->PINASSIGN8 = regVal | ( 12 << 16 );	/* P0.12 is CLKOUT, ASSIGN(23:16). */
  CLKOUT_Setup( CLKOUTCLK_SRC_WDT_OSC );

	WDTInit();

	init_mrt(0x20000);

	/* Set port PIO0_7 to output */
	GPIOSetDir( PORT0, 7, 1 );

	/* Set port PIO0_17 to output */
	GPIOSetDir( PORT0, 17, 1 );

	if ((LPC_SYSCON->SYSRSTSTAT & 0x4) == 0x4)
	{
		LPC_SYSCON->SYSRSTSTAT |= 0x4;
		while( 1 ) 
		{
			if ( (mrt_counter > 0) && (mrt_counter <= 200) )
			{
				GPIOSetBitValue( PORT0, 7, 0 );
			}
			if ( (mrt_counter > 200) && (mrt_counter <= 400) )
			{
				GPIOSetBitValue( PORT0, 7, 1 );
			}
			else if ( mrt_counter > 400 )
			{
				mrt_counter = 0;
			}
		}
	}
	else
	{
		while( 1 ) 
		{
			if ( (mrt_counter > 0) && (mrt_counter <= 200) )
			{
				GPIOSetBitValue( PORT0, 17, 0 );
			}
			if ( (mrt_counter > 200) && (mrt_counter <= 400) )
			{
				GPIOSetBitValue( PORT0, 17, 1 );
			}
			else if ( mrt_counter > 400 )
			{
				mrt_counter = 0;
			}
		}
	}
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
