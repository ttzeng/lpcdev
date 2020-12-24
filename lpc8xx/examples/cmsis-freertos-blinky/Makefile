program ?= cmsis_freertos_blinky

CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis

CONFIG_CMSIS_ARCH       = M0
CONFIG_FREERTOS_MEMMANG = 4

src-y      = src/$(program).c
includes-y = $(shell pwd)
extmod-y   = CMSIS-FreeRTOS
lflags     = -lgcc -lc_nano

include ../../../build.mk
