CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis

CONFIG_FREERTOS_PORT    = ARM_CM0
CONFIG_FREERTOS_MEMMANG = 4

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS FreeRTOS-Plus-CLI
lflags     = -lgcc -lc_nano

include ../../../build.mk
