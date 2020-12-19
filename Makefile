# To recursively find all files in a directory:
#	$(call find,foo/,*.c)
find = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call find,$d/,$2))

excludes := Makefile build.mk external
dirs     := $(foreach s,$(wildcard *),$(if $(findstring $s,$(excludes)),,$s))
targets  := $(dir $(sort $(foreach s,$(dirs),$(call find,$s/,Makefile))))

.PHONY: all clean

all clean:
	@- $(foreach s,$(targets), \
	     echo $(MAKE) -C $s $(MAKECMDGOALS); \
	     $(MAKE) -C $s $(MAKECMDGOALS) || exit; \
	    )
