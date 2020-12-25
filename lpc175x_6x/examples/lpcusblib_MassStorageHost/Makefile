CONFIG_CHIP         = lpc175x_6x
CONFIG_FIRMWARE     = lpcopen
CONFIG_BOARD        = 1768_pack

CONFIG_HAS_PRINTF         = y
CONFIG_PRINTF_OUTPUT_HOOK = Board_UARTPutChar

src-y      = $(wildcard src/*.c)
defines-y  = __LPC175X_6X__ USB_HOST_ONLY
includes-y = $(shell pwd)/inc
extmod-y   = LPCUSBLib FatFs utils
lflags     = -lgcc -lc_nano -lnosys

include ../../../build.mk
