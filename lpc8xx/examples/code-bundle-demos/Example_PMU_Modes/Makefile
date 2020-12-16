CONFIG_CHIP              = lpc8xx
CONFIG_FIRMWARE          = cmsis
CONFIG_CMSIS_CODE_BUNDLE = y

src-y      = $(wildcard src/*.c)
defines-y  = CLOCK_IRC_ONLY
includes-y = $(shell pwd)/inc

include ../../../../build.mk
