src-y      = $(call find,,*.c)
defines-y  = CPU_$(CONFIG_MCUXPRESSOSDK_CPU_FEATURES) \
             SDK_DEBUGCONSOLE=DEBUGCONSOLE_REDIRECT_TO_SDK \
             __CMSIS_RTOS __CORTEX_M0
includes-y = $(foreach s,board RTE,$(shell pwd)/$s)
lflags     = -lc_nano

include ../../../../build.mk
