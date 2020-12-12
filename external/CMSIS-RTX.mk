# The CMSIS-RTX module is part of the CMSIS GitHub repository.
# The module is set up by:
#	git clone https://github.com/ARM-software/CMSIS.git
d      := $(where-am-i)
module := $(who-am-i)

d          := $d/CMSIS/CMSIS/RTOS/RTX
includes-y := $(includes-y) $d/INC
ldflags-y  += -L $d/LIB/GCC
