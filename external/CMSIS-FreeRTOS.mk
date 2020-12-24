# The CMSIS-FreeRTOS module enables CMSIS-RTOS v2 API, a generic RTOS interface
# for Cortex-M processor-based devices, using FreeRTOS under the hood, instead of
# the default RTX5 RTOS kernel (https://www2.keil.com/mdk5/cmsis/rtx/) by Keil.
# The module is set up by:
#	git clone https://github.com/ARM-software/CMSIS-FreeRTOS.git
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

d               := $d/$(module)
cmsis           := $d/../CMSIS_5/CMSIS
rtos2           := $(cmsis)/RTOS2
rtos2-freertos  := $d/CMSIS/RTOS2/FreeRTOS
src-freertos    := $(wildcard $d/Source/*.c) \
                   $d/Source/portable/GCC/ARM_C$(CONFIG_CMSIS_ARCH)/port.c \
                   $d/Source/portable/MemMang/heap_$(CONFIG_FREERTOS_MEMMANG).c
src-rtos2-freertos := $(call find,$(rtos2-freertos)/Source/,*.c)

src-$(module)-y := $(src-freertos) $(src-rtos2-freertos)
includes-y      := $d/Source/include $d/Source/portable/GCC/ARM_C$(CONFIG_CMSIS_ARCH) \
                   $(cmsis)/Core/Include \
                   $(cmsis)/../Device/ARM/ARMC$(CONFIG_CMSIS_ARCH)/Include \
                   $(rtos2)/Include \
                   $(rtos2-freertos)/Include \
                   $(rtos2-freertos)/Include1 \
                   $(includes-y)
