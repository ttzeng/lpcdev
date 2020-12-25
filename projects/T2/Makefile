CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

CONFIG_FREERTOS_PORT    = ARM_CM3
CONFIG_FREERTOS_MEMMANG = 4

CONFIG_HAS_PRINTF         = y
CONFIG_PRINTF_OUTPUT_HOOK = Board_UARTPutChar

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS FreeRTOS-Plus-CLI utils
lflags     = -lgcc -lc_nano -lnosys

include ../../build.mk
