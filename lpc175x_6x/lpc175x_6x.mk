d      := $(where-am-i)
module := $(notdir $d)

cflags-y   += -mcpu=cortex-m3 -mthumb \
              -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections
asflags-y  += -x assembler-with-cpp
startup-y  := $(if $(startup-y),$(startup-y),$d/startup/cr_startup_lpc175x_6x.c)
startup-y  += $(wildcard $d/$(CONFIG_FIRMWARE)/startup/*.c)
ldscript-y := $d/ldscripts/flash.ld

# Code Read Protection support
startup-$(CONFIG_ENABLE_CRP) += $d/startup/crp.c

subdir-$(module)-y += $d/$(CONFIG_FIRMWARE)/$(CONFIG_FIRMWARE).mk

$(foreach s,$(subdir-$(module)-y),$(eval $(call include-mkfile,$s)))
