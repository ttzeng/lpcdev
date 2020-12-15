/* LPC812 pin assignments (PinList.txt by the Switch Matrix Tool)
[PinNo.][PinName]               [Signal]    [Function]
 1       PIO0_17                 PIO0_17     LED Segment-C
 2       PIO0_13                 PIO0_13     LED Segment-B
 3       PIO0_12                 PIO0_12     LED Segment-G
 4       RESET/PIO0_5            RESET       Reset
 5       PIO0_4                  U0_TXD      USART0 TxD
 6       SWCLK/PIO0_3            PIO0_3      DHT Sensor
 7       SWDIO/PIO0_2            PIO0_2      User Button
 8       PIO0_11(Open Drain)     PIO0_11     LED Digit-1
 9       PIO0_10(Open Drain)     PIO0_10     LED Digit-0
 10      PIO0_16                 PIO0_16     LED Segment-F
 11      PIO0_15                 PIO0_15     LED Segment-D
 12      PIO0_1/ACMP_I2/CLKIN    PIO0_1      LED Segment-E
 13      PIO0_9/XTALOUT          XTALOUT     XTALOUT
 14      PIO0_8/XTALIN           XTALIN      XTALIN
 15      VDD                     VDD         3.3V
 16      VSS                     VSS         GND
 17      PIO0_7                  PIO0_7      LED Segment-A
 18      PIO0_6/VDDCMP           PIO0_6      LED Digit-2
 19      PIO0_0/ACMP_I1          U0_RXD      USART0 RxD
 20      PIO0_14                 PIO0_14     LED Digit-3
*/
#ifdef __USE_CMSIS
#include "LPC8xx.h"
#include "lpc8xx_gpio.h"
#include "lpc8xx_mrt.h"
#endif
#include "FreeRTOS.h"
#include "task.h"

#ifdef __CODE_RED
#include <cr_section_macros.h>
#endif

static inline void SwitchMatrix_Init()
{	/* Generated by the Switch Matrix Tool */

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
{	/* Generated by the Switch Matrix Tool */

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
{	/* Reconfigure 30MHz system clock derived from System oscillator */
	/* Set up PLL: */
	//   Power up the crystal oscillator & system PLL in the PDRUNCFG register
	LPC_SYSCON->PDRUNCFG &= ~((1 << 5) | (1 << 7));
	//   Select the PLL input in the SYSPLLCLKSEL register
	LPC_SYSCON->SYSPLLCLKSEL = 1;	/* SYSOSC */
	//   Update the PLL clock source in the SYSPLLCLKUEN register
	LPC_SYSCON->SYSPLLCLKUEN = 0;
	LPC_SYSCON->SYSPLLCLKUEN = 1;
	//   Configure the PLL M and N dividers
	LPC_SYSCON->SYSPLLCTRL = (4 | (1 << 5));
	//   Wait for the PLL to lock by monitoring the PLL lock status
	while (!(LPC_SYSCON->SYSPLLSTAT & 1));

	/* Configure the main clock and system clock: */
	//   Select the main clock
	LPC_SYSCON->MAINCLKSEL = 3;		/* PLL output */
	//   Update the main clock source
	LPC_SYSCON->MAINCLKUEN = 0;
	LPC_SYSCON->MAINCLKUEN = 1;
	//   Select the divider value for the system clock to core, memories, and peripherals
	LPC_SYSCON->SYSAHBCLKDIV = 2;

	// Disable the BYPASS bit and select the oscillator frequency range in SYSOSCCTRL register
	LPC_SYSCON->SYSOSCCTRL = 0;
}

enum {
	DISP_TIME,
	DISP_SEC,
	DISP_HUMIDITY,
	DISP_TEMPERATURE,
	MAX_DISP
};
static volatile uint8_t disp_mode = DISP_TIME;
static volatile uint8_t hour = 12, min = 0, sec = 0;
static volatile uint8_t humidity, temperature;

volatile uint16_t gu16LedScratchPad;

static uint8_t bin2bcd(uint8_t v)
{
	return ((v/10)<<4)|(v%10);
}

static void display_update()
{
	uint8_t v;
	switch (disp_mode) {
	case DISP_TIME:
		if ((v = bin2bcd(hour)) < 0x10) v |= 0xf0;	// mask the leading zero
		gu16LedScratchPad = ((uint16_t)v << 8) | (bin2bcd(min));
		break;
	case DISP_SEC:
		gu16LedScratchPad = (0xff << 8) | (bin2bcd(sec));
		break;
	case DISP_HUMIDITY:
		gu16LedScratchPad = (bin2bcd(humidity) << 8) | 0xab;
		break;
	case DISP_TEMPERATURE:
		gu16LedScratchPad = (bin2bcd(temperature) << 8) | 0xdc;
	}
}

static void advance_time_by_min(uint8_t delta)
{
	if ((min += delta) >= 60) {
		min -= 60;
		if (++hour >= 24)
			hour = 0;
	}
	sec = 0;
}

/* Clock thread */
static void vClockTask (void *_unused)
{
	while (1) {
		display_update();
		vTaskDelay(configTICK_RATE_HZ);
		if (++sec >= 60)
			advance_time_by_min(1);
	}
}

#define GPIO_BUTTON		2
#define GPIO_DHTSENSOR	3
#define INPUT			0
#define OUTPUT			1
#define HIGH			1
#define LOW				0

/* User Input thread */
static void vUserInputTask (void *_unused)
{
	GPIOSetDir(PORT0, GPIO_BUTTON, INPUT);

	/* Software debouncer by Jack Ganssle (http://www.ganssle.com/debouncing-pt2.htm) */
	uint8_t state = 0;
	while (1) {
		vTaskDelay(configTICK_RATE_HZ / 100);
		state = (state << 1) | GPIOGetPinValue(PORT0, GPIO_BUTTON) | 0xe0;
		if (state == 0xf0) {	/* debounced */
			#define HZ_LONG_PRESS_CHECK     3
			#define TIME_LONG_PRESS         3
			#define LONG_PRESS_THRESHOLD	(TIME_LONG_PRESS * HZ_LONG_PRESS_CHECK)
			enum {
				EDIT_HOUR,
				EDIT_MIN,
			};
			static uint8_t edit_mode = EDIT_HOUR;
			uint32_t hold_cnt;
			for (hold_cnt = 0; disp_mode == DISP_TIME && !GPIOGetPinValue(PORT0, GPIO_BUTTON); hold_cnt++) {
				if (hold_cnt >= LONG_PRESS_THRESHOLD) {
					switch (edit_mode) {
					case EDIT_HOUR:
						if (++hour >= 24) hour = 0;
						break;
					case EDIT_MIN:
						if (++min >= 60) min = 0;
					}
					display_update();
				}
				vTaskDelay(configTICK_RATE_HZ / HZ_LONG_PRESS_CHECK);
			}
			if (hold_cnt >= LONG_PRESS_THRESHOLD) {
				edit_mode = (edit_mode == EDIT_HOUR)? EDIT_MIN : EDIT_HOUR;
			} else if (++disp_mode >= MAX_DISP)
				disp_mode = DISP_TIME;
		}
	}
}

static void delayUs(uint32_t delayInUs)
{
	uint32_t ivalue;
	LPC_MRT->Channel[0].INTVAL = (delayInUs * 30) | (1 << 31);
	while ((ivalue = LPC_MRT->Channel[0].TIMER & 0x7fffffff) != 0x7fffffff);
}

#define DHT_DATA_LEN	5
#define DHT_SUCCESS		0
#define DHT_ENOSPACE	-1
#define DHT_ECHECKSUM	-2

static int dht11_read(uint8_t port, uint8_t pin, uint8_t *buf, uint8_t buflen)
{
	if (buflen < DHT_DATA_LEN)
		return DHT_ENOSPACE;
	__disable_irq();

	/* Send START signal by pulling low the Data Signal bus at least 18ms */
	GPIOSetBitValue(port, pin, LOW);
	GPIOSetDir(port, pin, OUTPUT);
	delayUs(20000);

	/* Release the Data Signal bus and wait 20~40us for DHT's response */
	GPIOSetBitValue(port, pin, HIGH);
	GPIOSetDir(port, pin, INPUT);
	delayUs(20);

	/* DHT responds by pulling the Data Signal bus for 80us */
	while (GPIOGetPinValue(port, pin) != LOW);
	while (GPIOGetPinValue(port, pin) == LOW);

	/* DHT pulls high the Data Signal bus for 80us and prepares the sending data */
	while (GPIOGetPinValue(port, pin) != LOW);

	uint8_t index, checksum = 0;
	for (index = 0; index < DHT_DATA_LEN; index++) {
		uint8_t bit, value = 0;
		for (bit = 0x80; bit; bit >>= 1) {
			/* DHT pulls low the Data Signal bus for 50us to start sending each bit */
			while (GPIOGetPinValue(port, pin) == LOW);

			/* DHT pulls high the Data Signal bus, the length of duration is
			   determined by value of the sending bit. 0: 26~28us, 1: 70us */
			delayUs(35);
			if (GPIOGetPinValue(port, pin) != LOW) {
				while (GPIOGetPinValue(port, pin) != LOW);
				value |= bit;
			}
		}
		buf[index] = value;
	}

	__enable_irq();
	for (index = 0; index < DHT_DATA_LEN-1; index++)
		checksum += buf[index];
	return (buf[4] == checksum)? DHT_SUCCESS : DHT_ECHECKSUM;
}

/* DHT sensor thread */
static void vDHTSensorTask (void *_unused)
{
	static uint8_t data[DHT_DATA_LEN];
	GPIOSetDir(PORT0, GPIO_DHTSENSOR, INPUT);
	/* Enable clock to MRT and reset the MRT peripheral */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);
	LPC_SYSCON->PRESETCTRL &= ~(1 << 7);
	LPC_SYSCON->PRESETCTRL |=  (1 << 7);
	LPC_MRT->Channel[0].CTRL = MRT_ONE_SHOT_INT;

	/* do not send instructions within 1S after power on to pass the unstable state */
	while (1) {
		static uint8_t refresh_time = 5;
		vTaskDelay(configTICK_RATE_HZ * refresh_time);
		if (dht11_read(PORT0, GPIO_DHTSENSOR, data, sizeof(data)) == DHT_SUCCESS) {
			humidity = data[0], temperature = data[2];
			refresh_time = 60;
		} else refresh_time = 5;
	}
}

/* LED display thread */
static void vLEDDisplayTask (void *_unused)
{
	static uint8_t cathode[] = { 14, 6, 11, 10 };
	enum {
		_A = (1 << 7),
		_B = (1 << 13),
		_C = (1 << 17),
		_D = (1 << 15),
		_E = (1 << 1),
		_F = (1 << 16),
		_G = (1 << 12)
	};
	#define digit0	(_A|_B|_C|_D|_E|_F)
	#define digit1	(_B|_C)
	#define digit2	(_A|_B|_G|_E|_D)
	#define digit3	(_A|_B|_G|_C|_D)
	#define digit4	(_F|_G|_B|_C)
	#define digit5	(_A|_F|_G|_C|_D)
	#define digit6	(_A|_F|_E|_D|_C|_G)
	#define digit7	(_A|_B|_C)
	#define digit8	(_A|_B|_C|_D|_E|_F|_G)
	#define digit9	(_G|_F|_A|_B|_C|_D)
//	#define digitA	(_A|_B|_C|_E|_F|_G)
//	#define digitB	(_F|_E|_D|_C|_G)
	#define digitA	(_E|_G)			/* letter 'r' */
	#define digitB	(_B|_C|_F|_E|_G)/* letter 'H' */
	#define digitC	(_A|_F|_E|_D)
//	#define digitD	(_B|_C|_D|_E|_G)
	#define digitD	(_A|_B|_F|_G)	/* symbol degree */
	#define digitE	(_A|_D|_E|_F|_G)
//	#define digitF	(_A|_E|_F|_G)
	#define digitF	0		/* space for leading zeros */
	static uint32_t pattern[] = { digit0, digit1, digit2, digit3,
	                              digit4, digit5, digit6, digit7,
	                              digit8, digit9, digitA, digitB,
	                              digitC, digitD, digitE, digitF
	};

	uint8_t i, bit;
	for (i = 0; i < sizeof(cathode); i++) {
		GPIOSetDir(PORT0, cathode[i], OUTPUT);
		GPIOSetBitValue(PORT0, cathode[i], HIGH);
	}
	for (bit = 1; bit <= 17; bit++) {
		if (digit8 & (1 << bit))
			GPIOSetDir(PORT0, bit, OUTPUT);
	}

	while (1) {
		if (++i >= sizeof(cathode)) i = 0;
		uint32_t bitmaps = pattern[(gu16LedScratchPad >> (i<<2)) & 0xf];
		for (bit = 1; bit <= 17; bit++) {
			if (digit8 & (1 << bit))
				GPIOSetBitValue(PORT0, bit, bitmaps & (1 << bit));
		}
		GPIOSetBitValue(PORT0, cathode[i], LOW);
		vTaskDelay(configTICK_RATE_HZ/150);
		GPIOSetBitValue(PORT0, cathode[i], HIGH);
	}
}

int main(void)
{
	SwitchMatrix_Init();
	IOCON_Init();
	Clock_Setup();
	SystemCoreClockUpdate();
	GPIOInit();

	/* LED display thread */
	xTaskCreate(vLEDDisplayTask, "LED Display",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(TaskHandle_t *) NULL);
	/* Clock thread */
	xTaskCreate(vClockTask, "Clock",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(TaskHandle_t *) NULL);
	/* User input thread */
	xTaskCreate(vUserInputTask, "User Input",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(TaskHandle_t *) NULL);
	/* DHT sensor thread */
	xTaskCreate(vDHTSensorTask, "DHT Sensor",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(TaskHandle_t *) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();
	return 0;
}