# The FreeRTOS-Plus-CLI module is part of the FreeRTOS GitHub repository.
# The module is set up by:
#	git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

d               := $d/FreeRTOS/FreeRTOS-Plus
src-$(module)-y := $(wildcard $d/Source/$(module)/*.c)
includes-y      := $(includes-y) $d/Source/$(module)
