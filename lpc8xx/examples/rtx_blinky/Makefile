CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis
CONFIG_CMSIS_ARCH = M0
CONFIG_CMSIS_RTOS_REV  = 2
CONFIG_CMSIS_RTOS_TICK = systick

src-y    = $(filter-out $(if $(CONFIG_CMSIS_RTOS_REV:1=),%RTX_Conf_CM.c),$(wildcard src/*.c))
defines-y = __CMSIS_RTOS __CORTEX_M0
includes-y = $(shell pwd)/RTE
extmod-y = CMSIS-RTX
lflags   = $(if $(CONFIG_CMSIS_ARCH),,-lRTX_CM0) -lgcc -lc_nano

include ../../../build.mk
