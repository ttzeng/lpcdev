CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis

CONFIG_FREERTOS_PORT    = ARM_CM0
CONFIG_FREERTOS_MEMMANG = 4

src-y      = lpc812_led_clock.c
includes-y = $(shell pwd)
extmod-y   = FreeRTOS
lflags     = -lgcc -lc_nano

include ../../build.mk
