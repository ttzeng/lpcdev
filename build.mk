where-am-i = $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
who-am-i   = $(basename $(notdir $(lastword $(MAKEFILE_LIST))))

TOP := $(where-am-i)

# Do not use make's built-in rules and variables
# (this increases performance and avoids hard-to-debug behaviour)
MAKEFLAGS += -rR

# Do not print "Entering directory ..." at all for in-tree build.
MAKEFLAGS += --no-print-directory

# Beautify output
# ---------------------------------------------------------------------------
#
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing will be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If KBUILD_VERBOSE equals 0 then the above command will be hidden.
# If KBUILD_VERBOSE equals 1 then the above command is displayed.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands
ifeq ("$(origin V)", "command line")
  KBUILD_VERBOSE = $(V)
endif
ifndef KBUILD_VERBOSE
  KBUILD_VERBOSE = 0
endif
quiet := $(if $(KBUILD_VERBOSE:1=),quiet_)
Q     := $(if $(KBUILD_VERBOSE:1=),@)

# If the user is running make -s (silent mode), suppress echoing of
# commands
ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  quiet=silent_
endif

export quiet Q KBUILD_VERBOSE

# Print a command before executing it.
# Short version is used, if $(quiet) equals `quiet_', otherwise full one.
cmd = @set -e; $(if $($(quiet)cmd_$(1)),echo '  $($(quiet)cmd_$(1))';) $(cmd_$(1))

# Use make M=dir or set the environment variable KBUILD_EXTMOD to specify the
# directory of external module to build. Setting M= takes precedence.
ifeq ("$(origin M)", "command line")
  KBUILD_EXTMOD := $(M)
endif

# To append cflags to a particular source file, do this:
#	$(eval $(call add-local-cflags,src/foo.c,-I./inc))
define add-local-cflags =
cflags-$(notdir $(1))-y += $(2)
endef

# To include makefiles listed in a variable, do this:
#	$(foreach s,$(foo),$(eval $(call include-mkfile,$s)))
define include-mkfile =
include $(1)
endef

PHONY := default all clean lib

default: all

HOSTARCH := $(shell uname -m | sed -e s/i.86/x86/ -e s/x86_64/x86/ \
                                   -e s/sun4u/sparc64/ \
                                   -e s/arm.*/arm/ -e s/sa110/arm/ \
                                   -e s/s390x/s390/ -e s/parisc64/parisc/ \
                                   -e s/ppc.*/powerpc/ -e s/mips.*/mips/ \
                                   -e s/sh[234].*/sh/ -e s/aarch64.*/arm64/ \
                                   -e s/riscv.*/riscv/)

-include $(TOP)/.config
-include $(TOP)/$(CONFIG_CHIP)/$(CONFIG_CHIP).mk

$(foreach s,$(extmod-y) $(KBUILD_EXTMOD),$(eval $(call include-mkfile,$(TOP)/external/$s.mk)))

ARCH ?= $(HOSTARCH)

# CROSS_COMPILE specify the prefix used for all executables used
# during compilation. Only gcc and related bin-utils executables
# are prefixed with $(CROSS_COMPILE).
# CROSS_COMPILE can be set on the command line
# make CROSS_COMPILE=ia64-linux-
# Alternatively CROSS_COMPILE can be set in the environment.
# Default value for CROSS_COMPILE is not to prefix executables

# Make variables (CC, etc...)
AS          = $(CROSS_COMPILE)as
LD          = $(CROSS_COMPILE)ld
CC          = $(CROSS_COMPILE)gcc
CPP         = $(CC) -E
AR          = $(CROSS_COMPILE)ar
NM          = $(CROSS_COMPILE)nm
STRIP       = $(CROSS_COMPILE)strip
OBJCOPY     = $(CROSS_COMPILE)objcopy
OBJDUMP     = $(CROSS_COMPILE)objdump

export HOSTARCH ARCH CROSS_COMPILE AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

# Variable 'defines-y' defines symbols for all source files compilation.
cflags-y += $(foreach s,$(defines-y),-D$s)

# Variable 'includes-y' lists header search folders for all source files.
cflags-y += -I. $(foreach s,$(includes-y),-I$s)

# Use 'make D=0' to disable source level debugging.
ifeq ("$(origin D)", "command line")
  KBUILD_DEBUG = $(D)
endif
ifndef KBUILD_DEBUG
  KBUILD_DEBUG = 1
endif
cflags-y += $(if $(KBUILD_DEBUG:1=),-DNDEBUG,-g -DDEBUG)

# Use 'make O=n' to pass optimization option to gcc.
ifeq ("$(origin O)", "command line")
  KBUILD_OPTIMIZATION = $(O)
endif
ifndef KBUILD_OPTIMIZATION
  KBUILD_OPTIMIZATION = s
endif
cflags-y += -O$(KBUILD_OPTIMIZATION)

# Utility functions
mkdir      = mkdir -p $(abspath $(dir $(1)))
localpath  = $(subst $(TOP)/,,$(1))

%.o: %.c
	$(call cmd,cc_o_c)

%.o: %.cpp
	$(call cmd,cc_o_c)

quiet_cmd_cc_o_c = CC $(call localpath,$<)
      cmd_cc_o_c = $(CC) -c $(cflags-y) $(cflags-$(notdir $<)-y) $< -o $@

outdir  ?= out/
program ?= $(notdir $(shell pwd))
crt     := $(addsuffix .o,$(basename $(startup-y)))
obj-c   := $(addsuffix .o,$(basename $(src-y)))

all: $(outdir)$(program)

$(outdir)$(program): lib $(crt) $(obj-c) $(outdir)$(program).ld
	$(call cmd,gen_elf)
	$(call cmd,gen_hex)
	$(call cmd,gen_bin)

ifneq ($(libs-y),)
lib: $(foreach s,$(libs-y),$(outdir)lib$s.a)

define add-lib-rule =
$(outdir)lib$(1).a: $(addsuffix .o,$(basename $(src-$(1)-y)))
	$$(call cmd,mklib)
endef

$(foreach s,$(libs-y),$(eval $(call add-lib-rule,$s)))

obj-a  := $(addsuffix .o,$(basename $(foreach s,$(libs-y),$(src-$s-y))))
lflags := $(foreach s,$(libs-y),-l$s) $(lflags)

quiet_cmd_mklib = AR $(call localpath,$@)
      cmd_mklib = $(call mkdir,$@); $(AR) -rcs $@ $^
endif

$(outdir)$(program).ld: $(ldscript-y)
	$(Q)$(call mkdir,$@)
	$(Q)cp $^ $@

quiet_cmd_gen_elf = LD $(call localpath,$@)
      cmd_gen_elf = $(CC) -o $@ -nostdlib $(cflags-y) \
                          $(ldflags-y) -L $(outdir) -T $@.ld \
                          -Xlinker -Map=$@.map -Xlinker --gc-sections \
                          $(crt) $(obj-c) $(lflags)

quiet_cmd_gen_hex = OBJCOPY $(call localpath,$@.hex)
      cmd_gen_hex = $(OBJCOPY) -O ihex $@ $@.hex

quiet_cmd_gen_bin = OBJCOPY $(call localpath,$@.bin)
      cmd_gen_bin = $(OBJCOPY) -O binary $@ $@.bin

clean:
	$(call cmd,cleanup)

quiet_cmd_cleanup = CLEANUP
      cmd_cleanup = rm -rf $(crt) $(obj-c) $(obj-a) $(outdir)

.PHONY: $(PHONY)
