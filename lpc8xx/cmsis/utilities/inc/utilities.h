/*
 * utilities.h
 *
 *  Created on: Feb 16, 2016
 *      Author:
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "board.h"


void ConfigSWM(uint32_t func, uint32_t port_pin);
void PutTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string);
void GetTerminalString(LPC_USART_TypeDef * ptr_LPC_USART, uint8_t * ptr_string);
void GetConsoleString(char * ptr_string);
uint32_t GetConsoleInput(uint32_t number_of_digits);
unsigned char GetConsoleCharacter(const char * pPromptString);
void WaitConsoleEnter(const char * pPromptString);
void DebugWaitEsc(void);
void Config_LEDs(uint32_t bits);
void LEDs_Off(uint32_t bits);
void LEDs_On(uint32_t bits);
void Setup_LPC8xx_Low_Power();

#endif /* UTILITIES_H_ */
