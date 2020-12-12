CONFIG_CHIP      = lpc8xx
CONFIG_FIRMWARE ?= lpcopen
CONFIG_BOARD    ?= nxp_lpcxpresso_812

src-y      = $(wildcard $(CONFIG_FIRMWARE)/*.c)
includes-y = $(shell pwd)/$(CONFIG_FIRMWARE)
extmod-y   = FreeRTOS
lflags     = -lgcc -lc_nano

include ../../../build.mk
