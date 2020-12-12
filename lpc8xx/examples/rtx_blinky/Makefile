CONFIG_CHIP     = lpc8xx
CONFIG_FIRMWARE = cmsis

src-y    = $(wildcard src/*.c)
extmod-y = CMSIS-RTX
lflags   = -lRTX_CM0 -lgcc -lc_nano

include ../../../build.mk
