#include "cmsis_os2.h"                  // ARM::CMSIS:RTOS2:Keil RTX5
#include "board.h"

static void vLEDTask (void *argument)
{
    volatile bool LedState = false;

    while (1) {
        osThreadFlagsWait(0x0001, osFlagsWaitAny ,osWaitForever);
        Board_LED_Set(0, LedState);
        LedState = (bool) !LedState;
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
    SystemCoreClockUpdate();
    Board_Init();

    /* Initial LED0 state is off */
    Board_LED_Set(0, false);

    osKernelInitialize();               // Initialize CMSIS-RTOS
    osThreadNew(app_main, NULL, NULL);  // Create application main thread
    if (osKernelGetState() == osKernelReady) {
        osKernelStart();                // Start thread execution
    }
    return 1;
}
