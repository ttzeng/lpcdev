CONFIG_CHIP     = lpc175x_6x
CONFIG_FIRMWARE = lpcopen
CONFIG_BOARD    = 1768_pack

startup-y   = src/cr_ucos_iii_startup_lpc17xx40xx.c
src-y       = src/ucos_iii_blinky.c
includes-y += ./inc
extmod-y    = uCOS-III
lflags      = -lc_nano -lnosys

include ../../../build.mk
