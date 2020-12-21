# The CMSIS-RTX module is part of the CMSIS GitHub repository.
# The module is set up to build against CMSIS Version 4 by:
#	git clone https://github.com/ARM-software/CMSIS.git
# or CMSIS Version 5 by:
#	git clone https://github.com/ARM-software/CMSIS_5.git
d      := $(where-am-i)
module := $(who-am-i)

cmsis      := $d/CMSIS_5/CMSIS
rtos       := $(cmsis)/RTOS/RTX
includes-y := $(includes-y) $(rtos)/INC

ifdef CONFIG_CMSIS_ARCH
libs-y := $(libs-y) $(module)

src-$(module)-y := $(rtos)/SRC/GCC/SVC_Table.S \
                   $(rtos)/SRC/GCC/HAL_C$(CONFIG_CMSIS_ARCH).s \
                   $(wildcard $(rtos)/SRC/*.c)
$(eval $(call add-local-cflags,$(rtos)/SRC/rt_CMSIS.c,-I $(cmsis)/Include -I $(cmsis)/Core/Include))
else
ldflags-y += -L $(rtos)/LIB/GCC
endif
