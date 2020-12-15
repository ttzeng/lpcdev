/****************************************************************************
 *   $Id:: lpc8xx_wkt.c 3635 2012-10-31 00:31:46Z usb00423                     $
 *   Project: NXP LPC8xx Wakeup timer example
 *
 *   Description:
 *     This file contains wakeup timer test modules.
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
#include "lpc8xx_gpio.h"
#include "lpc8xx_mrt.h"
#include "lpc8xx_pmu.h"

extern volatile uint32_t mrt_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
#if !TEST_DEEPPOWERDOWN
  uint32_t regVal;
#endif

  SystemCoreClockUpdate();

  GPIOInit();

  while ( GPIOGetPinValue(0, 1) == 0x1);
	
#if !TEST_DEEPPOWERDOWN
  PMU_Init();
  regVal = LPC_SYSCON->PDSLEEPCFG;
  regVal &= ~(WDT_OSC_PD | BOD_PD);
  PMU_Sleep( MCU_SLEEP, regVal );
//  PMU_Sleep( MCU_DEEP_SLEEP, regVal );
//  PMU_Sleep( MCU_POWER_DOWN, regVal );
#else
  PMU_DeepPowerDown();
#endif

  /* Set port p0.7 to output */
  GPIOSetDir( 0, 7, 1 );

	init_mrt(0x2000);
	/* Only one NMI interrupt can be set at a time. Once it's dedicated to WKT,
	NMI can't be used for MRT. Inside init_mrt(), the MRT interrupt is routed to
	NMI once NMI_ENABLED flag is set. Change it back to use regular MRT interrupt. */
	LPC_SYSCON->NMISRC &= ~0x80000000;
  NVIC_EnableIRQ(MRT_IRQn);	
	
  while (1)                                /* Loop forever */
  {
		/* I/O configuration and LED setting pending. */
		if ( (mrt_counter > 0) && (mrt_counter <= 200) )
		{
			GPIOSetBitValue( 0, 7, 0 );
		}
		if ( (mrt_counter > 200) && (mrt_counter <= 400) )
		{
			GPIOSetBitValue( 0, 7, 1 );
		}
		else if ( mrt_counter > 400 )
		{
			mrt_counter = 0;
		}
  }

}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
