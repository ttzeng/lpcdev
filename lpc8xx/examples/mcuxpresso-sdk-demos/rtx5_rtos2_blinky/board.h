#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

#define LED_RED_INIT(output) \
        GPIO_PortInit(GPIO, BOARD_GPIOINIT_LED_RED_PORT); \
        GPIO_PinInit(GPIO, BOARD_GPIOINIT_LED_RED_PORT, \
                     BOARD_GPIOINIT_LED_RED_PIN, \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});
#define LED_RED_ON() \
        GPIO_PortClear(GPIO, BOARD_GPIOINIT_LED_RED_PORT, \
                       1U << BOARD_GPIOINIT_LED_RED_PIN)
#define LED_RED_OFF() \
        GPIO_PortSet(GPIO, BOARD_GPIOINIT_LED_RED_PORT, \
                     1U << BOARD_GPIOINIT_LED_RED_PIN)
#define LED_RED_TOGGLE() \
        GPIO_PortToggle(GPIO, BOARD_GPIOINIT_LED_RED_PORT, BOARD_GPIOINIT_LED_RED_PIN_MASK);

#define LED_GREEN_INIT(output) \
        GPIO_PortInit(GPIO, BOARD_GPIOINIT_LED_GREEN_PORT); \
        GPIO_PinInit(GPIO, BOARD_GPIOINIT_LED_GREEN_PORT, \
                     BOARD_GPIOINIT_LED_GREEN_PIN, \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});
#define LED_GREEN_ON() \
        GPIO_PortClear(GPIO, BOARD_GPIOINIT_LED_GREEN_PORT, \
                       1U << BOARD_GPIOINIT_LED_GREEN_PIN)
#define LED_GREEN_OFF() \
        GPIO_PortSet(GPIO, BOARD_GPIOINIT_LED_GREEN_PORT, \
                     1U << BOARD_GPIOINIT_LED_GREEN_PIN)
#define LED_GREEN_TOGGLE() \
        GPIO_PortToggle(GPIO, BOARD_GPIOINIT_LED_GREEN_PORT, BOARD_GPIOINIT_LED_GREEN_PIN_MASK);

#define LED_BLUE_INIT(output) \
        GPIO_PortInit(GPIO, BOARD_GPIOINIT_LED_BLUE_PORT); \
        GPIO_PinInit(GPIO, BOARD_GPIOINIT_LED_BLUE_PORT, \
                     BOARD_GPIOINIT_LED_BLUE_PIN, \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});
#define LED_BLUE_ON() \
        GPIO_PortClear(GPIO, BOARD_GPIOINIT_LED_BLUE_PORT, \
                       1U << BOARD_GPIOINIT_LED_BLUE_PIN)
#define LED_BLUE_OFF() \
        GPIO_PortSet(GPIO, BOARD_GPIOINIT_LED_BLUE_PORT, \
                     1U << BOARD_GPIOINIT_LED_BLUE_PIN)
#define LED_BLUE_TOGGLE() \
        GPIO_PortToggle(GPIO, BOARD_GPIOINIT_LED_BLUE_PORT, BOARD_GPIOINIT_LED_BLUE_PIN_MASK);

#define BOARD_DEBUG_USART_BAUDRATE 115200

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
