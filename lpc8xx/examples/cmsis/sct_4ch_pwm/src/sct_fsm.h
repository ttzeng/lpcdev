#ifndef __SCT_FSM_H__
#define __SCT_FSM_H__

/* Generated by fzmparser version 2.2 --- DO NOT EDIT! */

#include "sct_user.h"

extern void sct_fsm_init (void);

/* macros for defining the mapping between IRQ and events */
#define SCT_IRQ_EVENT_IRQ_cycle (0)
#define SCT_IRQ_EVENT_IRQ_abort (1)

/* Input assignments */
#define SCT_INPUT_ABORT (0)

/* Output assignments (and their defaults if specified) */
#define SCT_OUTPUT_PWM1 (0)
#define SCT_OUTPUTPRELOAD_PWM1 (0)
#define SCT_OUTPUT_PWM2 (1)
#define SCT_OUTPUTPRELOAD_PWM2 (0)
#define SCT_OUTPUT_PWM3 (2)
#define SCT_OUTPUTPRELOAD_PWM3 (1)
#define SCT_OUTPUT_PWM4 (3)
#define SCT_OUTPUTPRELOAD_PWM4 (1)


/* Match register reload macro definitions */
#define reload_pwm_cycle(value) LPC_SCT->MATCHREL[0].U = value;
#define reload_pwm_val1(value) LPC_SCT->MATCHREL[1].U = value;
#define reload_pwm_val2(value) LPC_SCT->MATCHREL[2].U = value;
#define reload_pwm_val3(value) LPC_SCT->MATCHREL[3].U = value;
#define reload_pwm_val4(value) LPC_SCT->MATCHREL[4].U = value;

#endif
