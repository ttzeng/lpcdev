CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

CONFIG_FREERTOS_PORT    = ARM_CM3
CONFIG_FREERTOS_MEMMANG = 3

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS
lflags     = -lc_nano -lnosys

include ../../../build.mk
