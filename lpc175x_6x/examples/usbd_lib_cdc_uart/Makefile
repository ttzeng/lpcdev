CONFIG_CHIP         = lpc175x_6x
CONFIG_FIRMWARE     = lpcopen
CONFIG_BOARD        = 1768_pack
CONFIG_LPCOPEN_USBD = y

src-y      = $(wildcard src/*.c)
includes-y = $(shell pwd)/inc
lflags     = -lc_nano

include ../../../build.mk
