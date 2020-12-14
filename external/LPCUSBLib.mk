d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

d               := $d/$(module)
src-$(module)-y := $(call find,$d,*.c)
includes-y      := $(includes-y) $d/Drivers/USB
