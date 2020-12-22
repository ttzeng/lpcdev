CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

CONFIG_CMSIS_ARCH      = M3
CONFIG_CMSIS_RTOS_REV  = 2
CONFIG_CMSIS_RTOS_TICK = systick

src-y      = blinky.c
defines-y  = __CMSIS_RTOS __CORTEX_M3
includes-y = $(shell pwd)/RTE
extmod-y   = CMSIS-RTX
lflags     = -lc_nano

include ../../../build.mk
