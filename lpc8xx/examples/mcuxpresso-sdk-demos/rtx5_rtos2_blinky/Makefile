CONFIG_CHIP                 = lpc8xx
CONFIG_FIRMWARE             = MCUXpresso-SDK
CONFIG_MCUXPRESSOSDK_DEVICE = LPC812

CONFIG_CMSIS_ARCH      = M0
CONFIG_CMSIS_RTOS_REV  = 2
CONFIG_CMSIS_RTOS_TICK = systick

src-y      = $(call find,,*.c)
defines-y  = CPU_LPC812M101JD20 \
             SDK_DEBUGCONSOLE=DEBUGCONSOLE_REDIRECT_TO_SDK \
             __CMSIS_RTOS __CORTEX_M0
includes-y = $(foreach s,board RTE,$(shell pwd)/$s)
extmod-y   = CMSIS-RTX
lflags     = -lgcc -lc_nano

include ../../../../build.mk
