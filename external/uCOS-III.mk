d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(module) $(libs-y)

d               := $d/$(module)
src-$(module)-y := $(wildcard $d/src/*)
includes-y      := $(includes-y) $d/inc
