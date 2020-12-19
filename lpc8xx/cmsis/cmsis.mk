d      := $(where-am-i)
module := $(notdir $d)
libs-y := $(libs-y) $(module)

drivers         := $(if $(findstring y,$(CONFIG_CMSIS_CODE_BUNDLE)),peripherals utilities,drivers)
src-$(module)-y := $(wildcard $(foreach s,core $(drivers),$d/$s/src/*.c))
defines-y       += __USE_CMSIS=CMSIS_CORE_LPC8xx
includes-y      := $(includes-y) $(wildcard $(foreach s,core $(drivers),$d/$s/inc))
