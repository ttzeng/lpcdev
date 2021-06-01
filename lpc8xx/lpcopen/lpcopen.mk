d      := $(where-am-i)
module := $(notdir $d)
libs-y := $(libs-y) $(module)

src-$(module)-y := $(wildcard $d/src/*.c)
defines-y       += __USE_LPCOPEN CORE_M0PLUS
includes-y      := $(includes-y) $d/inc

ifneq ($(CONFIG_BOARD),)
src-$(module)-y += $(wildcard $d/board/$(CONFIG_BOARD)/src/*.c)
includes-y      := $(includes-y) $d/board/$(CONFIG_BOARD)/inc
else
defines-y       += NO_BOARD_LIB
endif

# Suppress compiler warnings in iap.c
$(eval $(call add-local-cflags,iap.c,-w))
