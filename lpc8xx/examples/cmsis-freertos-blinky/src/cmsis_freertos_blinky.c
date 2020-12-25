#include "LPC8xx.h"
#include "lpc8xx_gpio.h"

#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5

#define GPIO_LED_BLUE       7

static inline void SwitchMatrix_Init()
{
    /* Generated by the Switch Matrix Tool */

    /* Enable SWM clock (already enabled in system_LPC8xx.c */
    //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);

    /* Pin Assign 8 bit Configuration */
    /* U0_TXD @ PIO0_4 */
    /* U0_RXD @ PIO0_0 */
    LPC_SWM->PINASSIGN0 = 0xffff0004UL;

    /* Pin Assign 1 bit Configuration */
    /* XTALIN  @ PIO0_8 */
    /* XTALOUT @ PIO0_9 */
    /* RESET   @ PIO0_5 */
    LPC_SWM->PINENABLE0 = 0xffffff8fUL;

    /* Disable the clock to the Switch Matrix to save power */
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<7);
}

static inline void IOCON_Init()
{
    /* Generated by the Switch Matrix Tool */

    /* Enable IOCON clock (already enabled in system_LPC8xx.c) */
    //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<18);

    /* Pin I/O Configuration */
    // Enable XTALIN and XTALOUT on PIO0_8 and PIO0_9 & remove the pull-up/down resistors
    LPC_IOCON->PIO0_8 = 0x80;
    LPC_IOCON->PIO0_9 = 0x80;

    /* Disable the clock to the IOCON to save power */
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<18);
}

static inline void Clock_Setup()
{
    /* Reconfigure 30MHz system clock derived from System oscillator */
    /* Set up PLL: */
    //   Power up the crystal oscillator & system PLL in the PDRUNCFG register
    LPC_SYSCON->PDRUNCFG &= ~((1 << 5) | (1 << 7));
    //   Select the PLL input in the SYSPLLCLKSEL register
    LPC_SYSCON->SYSPLLCLKSEL = 1;   /* SYSOSC */
    //   Update the PLL clock source in the SYSPLLCLKUEN register
    LPC_SYSCON->SYSPLLCLKUEN = 0;
    LPC_SYSCON->SYSPLLCLKUEN = 1;
    //   Configure the PLL M and N dividers
    LPC_SYSCON->SYSPLLCTRL = (4 | (1 << 5));
    //   Wait for the PLL to lock by monitoring the PLL lock status
    while (!(LPC_SYSCON->SYSPLLSTAT & 1));

    /* Configure the main clock and system clock: */
    //   Select the main clock
    LPC_SYSCON->MAINCLKSEL = 3;     /* PLL output */
    //   Update the main clock source
    LPC_SYSCON->MAINCLKUEN = 0;
    LPC_SYSCON->MAINCLKUEN = 1;
    //   Select the divider value for the system clock to core, memories, and peripherals
    LPC_SYSCON->SYSAHBCLKDIV = 2;

    // Disable the BYPASS bit and select the oscillator frequency range in SYSOSCCTRL register
    LPC_SYSCON->SYSOSCCTRL = 0;
}

static void vLEDTask (void *argument)
{
    GPIOSetDir(0, GPIO_LED_BLUE, 1);
    while (1) {
        osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);   /* wait for an event flag 0x0001 */
        GPIOSetBitValue(0, GPIO_LED_BLUE, GPIOGetPinValue(0, GPIO_LED_BLUE) ^ 1);
    }
}

static void app_main (void *argument)
{
    osThreadId_t tid = osThreadNew(vLEDTask, NULL, NULL);
    while (1) {
        osThreadFlagsSet(tid, 0x0001);
        osDelay(1000);
    }
}

int main(void)
{
    SwitchMatrix_Init();
    IOCON_Init();
    Clock_Setup();
    SystemCoreClockUpdate();
    GPIOInit();

    osKernelInitialize();               // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL);  // Create application main thread
    if (osKernelGetState() == osKernelReady) {
        osKernelStart();                // Start thread execution
    }
    return 0;
}