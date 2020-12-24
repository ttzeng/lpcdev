CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

CONFIG_CMSIS_ARCH       = M3
CONFIG_FREERTOS_MEMMANG = 4

src-y      = blinky.c
includes-y = $(shell pwd)
extmod-y   = CMSIS-FreeRTOS
lflags     = -lc_nano -lnosys

include ../../../build.mk
