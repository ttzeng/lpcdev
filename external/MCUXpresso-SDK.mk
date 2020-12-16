# The MCUXpresso SDK is a comprehensive software enablement package designed to
# simplify and accelerate application development with Arm® Cortex®-M-based devices
# from NXP.
# Visit the SDK builder at the following link, build and customize the SDK for
# your target board or MCU (e.g. LPC812), then download and unzip the package
# under the 'MCUXpresso-SDK' folder.
#	https://mcuxpresso.nxp.com
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module)

mcusdk            := $d/$(module)
mcusdk-cmsis      := $(mcusdk)/CMSIS
mcusdk-device     := $(mcusdk)/devices/$(CONFIG_MCUXPRESSOSDK_DEVICE)
mcusdk-drivers    := $(mcusdk-device)/drivers
mcusdk-components := $(mcusdk)/components
mcusdk-utils      := $(mcusdk-device)/utilities

cflags-y        += -mcpu=cortex-m0 -mthumb \
                   -ffunction-sections -fdata-sections -ffreestanding -fno-builtin
startup-y       := $(mcusdk-device)/mcuxpresso/startup_lpc812.c
src-$(module)-y := $(mcusdk-device)/system_LPC812.c \
                   $(wildcard $(mcusdk-drivers)/*.c) \
                   $(call find,$(mcusdk-components)/,*.c) \
                   $(call find,$(mcusdk-utils)/,*.c)
includes-y      += $(mcusdk-cmsis)/Include \
                   $(mcusdk-cmsis)/Driver/Include \
                   $(mcusdk-device) \
                   $(mcusdk-drivers) \
                   $(sort $(dir $(call find,$(mcusdk-components)/,*.h))) \
                   $(sort $(dir $(call find,$(mcusdk-utils)/,*.h)))
