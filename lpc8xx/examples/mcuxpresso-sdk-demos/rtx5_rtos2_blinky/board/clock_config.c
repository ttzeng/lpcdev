/*
 * How to set up clock using clock driver functions:
 *
 * 1. Setup clock sources.
 *
 * 2. Set up all dividers.
 *
 * 3. Set up all selectors to provide selected clocks.
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v7.0
processor: LPC812
package_id: LPC812M101JDH20
mcu_data: ksdk2_0
processor_version: 8.0.2
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

#include "fsl_power.h"
#include "fsl_clock.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_ClockInitPLL30MHz();
}

/*******************************************************************************
 ******************* Configuration BOARD_ClockInitPLL30MHz *********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_ClockInitPLL30MHz
called_from_default_init: true
outputs:
- {id: I2C0_clock.outFreq, value: 30 MHz}
- {id: LowPower_clock.outFreq, value: 10 kHz}
- {id: SPI0_clock.outFreq, value: 30 MHz}
- {id: SPI1_clock.outFreq, value: 30 MHz}
- {id: SYSPLL_clock.outFreq, value: 60 MHz}
- {id: System_clock.outFreq, value: 30 MHz}
- {id: divto750k_clock.outFreq, value: 750 kHz}
settings:
- {id: SYSCON.MAINCLKSEL.sel, value: SYSCON.PLL}
- {id: SYSCON.M_MULT.scale, value: '5', locked: true}
- {id: SYSCON.N_DIV.scale, value: '1', locked: true}
- {id: SYSCON.SYSAHBCLKDIV.scale, value: '2'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Variables for BOARD_ClockInitPLL30MHz configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_ClockInitPLL30MHz configuration
 ******************************************************************************/
void BOARD_ClockInitPLL30MHz(void)
{
    /*!< Set up the clock sources */
    /*!< Set up IRC */
    POWER_DisablePD(kPDRUNCFG_PD_IRC_OUT);                   /*!< Ensure IRC_OUT is on  */
    POWER_DisablePD(kPDRUNCFG_PD_IRC);                      /*!< Ensure IRC is on  */
    POWER_DisablePD(kPDRUNCFG_PD_SYSOSC);                  /*!< Ensure Main osc is on */
    CLOCK_Select(kSYSPLL_From_Irc);                         /*!< set IRC to pll select */
    clock_sys_pll_t config;
    config.src = kCLOCK_SysPllSrcIrc;                           /*!< set pll src  */
    config.targetFreq = 60000000U;                     /*!< set pll target freq */
    CLOCK_InitSystemPll(&config);                           /*!< set parameters */
    CLOCK_SetMainClkSrc(kCLOCK_MainClkSrcSysPll);         /*!< select pll clock for main clock */
    CLOCK_Select(kCLKOUT_From_Irc);                        /*!< select IRC for CLKOUT */
    CLOCK_UpdateClkOUTsrc();                                   /*!< update CLKOUT src */
    CLOCK_SetCoreSysClkDiv(2U);
    /*!< Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_CLOCKINITPLL30MHZ_CORE_CLOCK;
}

