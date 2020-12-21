CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis
CONFIG_CMSIS_ARCH = M0

src-y    = $(wildcard src/*.c)
defines-y = __CMSIS_RTOS __CORTEX_M0
extmod-y = CMSIS-RTX
lflags   = $(if $(CONFIG_CMSIS_ARCH),,-lRTX_CM0) -lgcc -lc_nano

include ../../../build.mk
