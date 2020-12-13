d      := $(where-am-i)
module := $(notdir $d)
libs-y := $(libs-y) $(module)

src-$(module)-y := $(wildcard $d/core/src/*.c \
                              $d/drivers/src/*.c)
defines-y       += __USE_CMSIS=CMSIS_CORE_LPC17xx
includes-y      := $(includes-y) $d/core/inc \
                                 $d/drivers/inc
