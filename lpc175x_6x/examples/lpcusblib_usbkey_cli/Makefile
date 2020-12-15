CONFIG_CHIP         = lpc175x_6x
CONFIG_FIRMWARE     = lpcopen
CONFIG_BOARD        = 1768_pack

CONFIG_FREERTOS_PORT    = ARM_CM3
CONFIG_FREERTOS_MEMMANG = 3

src-y      = $(wildcard src/*.c)
defines-y  = __LPC175X_6X__ USB_HOST_ONLY
includes-y = $(shell pwd)/inc
extmod-y   = FreeRTOS FreeRTOS-Plus-CLI LPCUSBLib FatFs
lflags     = -lc_nano -lnosys

include ../../../build.mk
