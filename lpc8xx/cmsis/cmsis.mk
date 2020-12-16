d      := $(where-am-i)
module := $(notdir $d)
libs-y := $(libs-y) $(module)

drivers         := $(if $(CONFIG_CMSIS_CODE_BUNDLE:y=),drivers,peripherals utilities)
src-$(module)-y := $(wildcard $(foreach s,core $(drivers),$d/$s/src/*.c))
defines-y       += __USE_CMSIS=CMSIS_CORE_LPC8xx
includes-y      := $(includes-y) $(wildcard $(foreach s,core $(drivers),$d/$s/inc))
