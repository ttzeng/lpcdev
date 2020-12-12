CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS FreeRTOS-Plus-CLI
lflags     = -lgcc -lc_nano

include ../../../build.mk
