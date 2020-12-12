d      := $(where-am-i)
module := $(notdir $d)
libs-y := $(libs-y) $(module)

src-$(module)-y := $(wildcard $d/src/*.c \
                              $d/board/$(CONFIG_BOARD)/src/*.c)
defines-y       += __USE_LPCOPEN CORE_M0PLUS
includes-y      := $(includes-y) $d/inc \
                                 $d/board/$(CONFIG_BOARD)/inc

# Suppress compiler warnings in iap.c
$(eval $(call add-local-cflags,iap.c,-w))
