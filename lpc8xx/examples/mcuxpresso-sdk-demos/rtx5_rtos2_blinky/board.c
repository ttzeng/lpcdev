#include "board.h"

status_t BOARD_InitDebugConsole(void)
{
    status_t result;
    CLOCK_EnableClock(kCLOCK_Uart0);
    CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);
    RESET_PeripheralReset(kUART0_RST_N_SHIFT_RSTn);
    result = DbgConsole_Init(0,
                             BOARD_DEBUG_USART_BAUDRATE, kSerialPort_Uart,
                             CLOCK_GetMainClkFreq());
    assert(kStatus_Success == result);
}
