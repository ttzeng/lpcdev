#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

static void blinky (void *argument) {
    while (1) {
        osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);   /* wait for an event flag 0x0001 */
        LED_BLUE_TOGGLE();
    }
}

static void app_main (void *argument) {
    osThreadId_t tid = osThreadNew(blinky, NULL, NULL);
    while (1) {
        osThreadFlagsSet(tid, 0x0001);  /* set signal to the blinky thread */
        osDelay(1000);
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    LED_BLUE_INIT(1);

    osKernelInitialize();               // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL);  // Create application main thread
    if (osKernelGetState() == osKernelReady) {
        osKernelStart();                // Start thread execution
    }
}
