# FatFs is a generic FAT file system module for small embedded systems.
# The module is set up by unzip the release file downloaded from ChaN' proect website:
#	http://elm-chan.org/fsw/ff/00index_e.html
d      := $(where-am-i)
module := $(who-am-i)
libs-y := $(libs-y) $(module) 

d               := $d/FatFs
src-$(module)-y := $d/source/ff.c
includes-y      := $(includes-y) $d/source
