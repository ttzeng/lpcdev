CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

CONFIG_FREERTOS_PORT    = ARM_CM3
CONFIG_FREERTOS_MEMMANG = 4

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS FreeRTOS-Plus-CLI
lflags     = -lgcc -lc_nano -lnosys

include ../../build.mk
