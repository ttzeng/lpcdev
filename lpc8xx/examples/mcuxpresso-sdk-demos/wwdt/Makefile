CONFIG_CHIP                 = lpc8xx
CONFIG_FIRMWARE             = MCUXpresso-SDK
CONFIG_MCUXPRESSOSDK_DEVICE = LPC812

src-y     = $(wildcard *.c)
defines-y = CPU_LPC812M101JD20
lflags    = -lgcc -lc_nano

include ../../../../build.mk
