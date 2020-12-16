d      := $(where-am-i)
module := $(notdir $d)

ifeq ($(CONFIG_FIRMWARE),MCUXpresso-SDK)
subdir-$(module)-y += $(TOP)/external/$(CONFIG_FIRMWARE).mk
else
cflags-y   += -mcpu=cortex-m0 -mthumb \
              -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections
startup-y  := $d/startup/cr_startup_lpc8xx.c
startup-y  += $(wildcard $d/$(CONFIG_FIRMWARE)/startup/*.c)

# Code Read Protection support
startup-$(CONFIG_ENABLE_CRP) += $d/startup/crp.c
# Micro Trace Buffer support
startup-$(CONFIG_ENABLE_MTB) += $d/startup/mtb.c

subdir-$(module)-y += $d/$(CONFIG_FIRMWARE)/$(CONFIG_FIRMWARE).mk
endif
ldscript-y := $d/ldscripts/flash.ld

$(foreach s,$(subdir-$(module)-y),$(eval $(call include-mkfile,$s)))
