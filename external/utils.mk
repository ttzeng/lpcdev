d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(module) $(libs-y)

# (https://github.com/mpaland/printf.git) library is a tiny but fully loaded
# printf, sprintf and (v)snprintf implementation designed for embedded systems.
# See https://embeddedartistry.com/blog/2019/11/06/an-embedded-friendly-printf-implementation/
printf       := $d/printf
src-printf-y := $(printf)/printf.c
$(eval $(call add-local-cflags,$(src-printf-y),\
         $(if $(CONFIG_PRINTF_OUTPUT_HOOK),-D_putchar=$(CONFIG_PRINTF_OUTPUT_HOOK))\
         $(if $(findstring y,$(CONFIG_PRINTF_DROP_FLOAT)),-DPRINTF_DISABLE_SUPPORT_FLOAT)\
         $(if $(findstring y,$(CONFIG_PRINTF_DROP_EXPONENTIAL)),-DPRINTF_DISABLE_SUPPORT_EXPONENTIAL)\
         $(if $(findstring y,$(CONFIG_PRINTF_DROP_LONG_LONG)),-DPRINTF_DISABLE_SUPPORT_LONG_LONG)\
        ))
defines-$(CONFIG_HAS_PRINTF) += printf=printf_\
                                sprintf=sprintf_\
                                snprintf=snprintf_\
                                vsnprintf=vsnprintf_\
                                vprintf=vprintf_

src-$(module)-$(CONFIG_HAS_PRINTF) += $(src-printf-y)
