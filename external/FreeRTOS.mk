# The FreeRTOS module is part of the FreeRTOS GitHub repository.
# The module is set up by:
#	git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

portable        := portable/GCC/ARM_CM0
src-mem-alloc   := heap_4.c

d               := $d/FreeRTOS/$(module)
srcdir          := $d/Source \
                   $d/Source/$(portable)
src-$(module)-y := $(wildcard $(patsubst %,%/*.c,$(srcdir))) \
                   $d/Source/portable/MemMang/$(src-mem-alloc)
includes-y      := $(includes-y) \
                   $d/Source/include \
                   $d/Source/$(portable)
