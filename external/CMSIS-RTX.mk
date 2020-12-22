# The CMSIS-RTX module is part of the CMSIS GitHub repository.
# The module is set up to build against CMSIS Version 4 by:
#	git clone https://github.com/ARM-software/CMSIS.git
# or CMSIS Version 5 by:
#	git clone https://github.com/ARM-software/CMSIS_5.git
d      := $(where-am-i)
module := $(who-am-i)

cmsis      := $d/CMSIS_5/CMSIS
rtos       := $(cmsis)/RTOS$(CONFIG_CMSIS_RTOS_REV:1=)/RTX
includes-y := $(includes-y) $(rtos)/$(if $(CONFIG_CMSIS_RTOS_REV:1=),Include1,INC)

ifdef CONFIG_CMSIS_ARCH
libs-y := $(libs-y) $(module)

ifneq ($(CONFIG_CMSIS_RTOS_REV:1=),)
src-$(module)-y := $(rtos)/Source/GCC/irq_c$(shell echo $(CONFIG_CMSIS_ARCH) | tr A-Z a-z).S \
                   $(rtos)/../Source/os_$(CONFIG_CMSIS_RTOS_TICK).c \
                   $(wildcard $(foreach s,Config Source,$(rtos)/$s/*.c)) \
                   $(rtos)/../Template/cmsis_os1.c
includes-y      := $(includes-y) \
                   $(cmsis)/RTOS$(CONFIG_CMSIS_RTOS_REV)/Include \
                   $(cmsis)/Core/Include \
                   $(cmsis)/../Device/ARM/ARMC$(CONFIG_CMSIS_ARCH)/Include \
                   $(rtos)/Include
else
src-$(module)-y := $(rtos)/SRC/GCC/SVC_Table.S \
                   $(rtos)/SRC/GCC/HAL_C$(CONFIG_CMSIS_ARCH).s \
                   $(wildcard $(rtos)/SRC/*.c)
$(eval $(call add-local-cflags,$(rtos)/SRC/rt_CMSIS.c,-I $(cmsis)/Include -I $(cmsis)/Core/Include))
endif
else
ldflags-y += -L $(rtos)/LIB/GCC
endif
