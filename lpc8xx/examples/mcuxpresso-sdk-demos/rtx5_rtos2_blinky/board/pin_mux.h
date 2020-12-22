/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief
 * Selects function mode (on-chip pull-up/pull-down resistor control).
 * : Inactive.
 * Inactive (no pull-down/pull-up resistor enabled).
 */
#define PIO0_8_MODE_INACTIVE 0x00u
/*!
 * @brief
 * Selects function mode (on-chip pull-up/pull-down resistor control).
 * : Inactive.
 * Inactive (no pull-down/pull-up resistor enabled).
 */
#define PIO0_9_MODE_INACTIVE 0x00u

/*! @name PIO0_7 (number 17), LED_BLUE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_GPIOINIT_LED_BLUE_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_GPIOINIT_LED_BLUE_GPIO_PIN_MASK (1U << 7U) /*!<@brief GPIO pin mask */
#define BOARD_GPIOINIT_LED_BLUE_PORT 0U                  /*!<@brief PORT device index: 0 */
#define BOARD_GPIOINIT_LED_BLUE_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_GPIOINIT_LED_BLUE_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*! @name PIO0_16 (number 10), LED_GREEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_GPIOINIT_LED_GREEN_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_GPIOINIT_LED_GREEN_GPIO_PIN_MASK (1U << 16U) /*!<@brief GPIO pin mask */
#define BOARD_GPIOINIT_LED_GREEN_PORT 0U                   /*!<@brief PORT device index: 0 */
#define BOARD_GPIOINIT_LED_GREEN_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_GPIOINIT_LED_GREEN_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name PIO0_17 (number 1), LED_RED
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_GPIOINIT_LED_RED_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_GPIOINIT_LED_RED_GPIO_PIN_MASK (1U << 17U) /*!<@brief GPIO pin mask */
#define BOARD_GPIOINIT_LED_RED_PORT 0U                   /*!<@brief PORT device index: 0 */
#define BOARD_GPIOINIT_LED_RED_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_GPIOINIT_LED_RED_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_GpioInit(void); /* Function assigned for the Cortex-M0P */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/