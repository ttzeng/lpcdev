CONFIG_CHIP         = lpc175x_6x
CONFIG_FIRMWARE     = lpcopen
CONFIG_BOARD        = 1768_pack

src-y      = $(wildcard src/*.c)
defines-y  = __CODE_RED __LPC175X_6X__ USB_HOST_ONLY
includes-y = $(shell pwd)/inc
extmod-y   = LPCUSBLib FatFs
lflags     = -lc_nano -lnosys

include ../../../build.mk
