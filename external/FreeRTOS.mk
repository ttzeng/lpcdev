# The FreeRTOS module is part of the FreeRTOS GitHub repository.
# The module is set up by:
#	git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

d               := $d/FreeRTOS/$(module)
portable        := portable/GCC/$(CONFIG_FREERTOS_PORT)
srcdir          := $d/Source \
                   $d/Source/$(portable)
src-$(module)-y := $(wildcard $(patsubst %,%/*.c,$(srcdir))) \
                   $d/Source/portable/MemMang/heap_$(CONFIG_FREERTOS_MEMMANG).c
includes-y      := $(includes-y) \
                   $d/Source/include \
                   $d/Source/$(portable)
