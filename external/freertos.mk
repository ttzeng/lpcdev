d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

d               := $d/$(module)
portable        := portable/GCC/$(CONFIG_FREERTOS_PORT)
srcdir          := $d/src $d/$(portable)
src-$(module)-y := $(wildcard $(patsubst %,%/*.c,$(srcdir))) \
                   $d/portable/MemMang/heap_$(CONFIG_FREERTOS_MEMMANG).c
includes-y      := $(includes-y) $d/inc $d/$(portable)
