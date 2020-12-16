/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_iap.h"

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    status_t status;
    uint32_t partID;
    uint32_t bootCodeVersion[2];
    uint32_t uniqueID[4];

    /* Board pin, clock, debug console init */
    /* Enable clock of uart0. */
    CLOCK_EnableClock(kCLOCK_Uart0);
    /* Ser DIV of uart0. */
    CLOCK_SetClkDivider(kCLOCK_DivUsartClk, 1U);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF("\r\nIAP example\r\n");

    /* Read part identification number */
    status = IAP_ReadPartID(&partID);
    if (status != kStatus_IAP_Success)
    {
        PRINTF("\r\nRead part identification number failed\r\n");
    }
    else
    {
        PRINTF("\r\nPartID:\t%X\r\n", partID);
    }

    /* Read boot code version */
    status = IAP_ReadBootCodeVersion(bootCodeVersion);
    if (status != kStatus_IAP_Success)
    {
        PRINTF("\r\nRead boot code version failed\r\n");
    }
    else
    {
        PRINTF("\r\nThe major version is:\t%X\r\n", bootCodeVersion[0]);
        PRINTF("\r\nThe minor version is:\t%X\r\n", bootCodeVersion[1]);
    }

    /* Read unique identification number */
    status = IAP_ReadUniqueID(uniqueID);
    if (status != kStatus_IAP_Success)
    {
        PRINTF("\r\nRead unique identification number failed\r\n");
    }
    else
    {
        PRINTF("\r\nUnique ID:\t%X%X%X%X\r\n", uniqueID[0], uniqueID[1], uniqueID[2], uniqueID[3]);
    }

    PRINTF("\r\nEnd of IAP Example\r\n");
    while (1)
    {
    }
}
