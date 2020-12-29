src-y    = $(filter-out $(if $(CONFIG_CMSIS_RTOS_REV:1=),%RTX_Conf_CM.c),$(wildcard src/*.c))
defines-y = __CMSIS_RTOS __CORTEX_M0
includes-y = $(shell pwd)/RTE
lflags   = $(if $(CONFIG_CMSIS_ARCH),,-lRTX_CM0) -lc_nano

include ../../../build.mk
