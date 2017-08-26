#!/usr/bin/make -f
# Makefile to the 'libdir' library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules (https://github.com/pure-data/pd-lib-builder).

lib.name = OSVR

# special file that does not provide a class
lib.setup.sources =

# all other C and C++ files in subdirs are source files per class
# (alternatively, enumerate them by hand)
class.sources = osvrdata.c

datafiles = \
TODO.txt \
LICENSE.txt \
README.txt \
libdir-meta.pd

datadirs =

OSVRDIR=/home/max/Code/OSVR/osvr-core-ubuntu-build-script/dist/

cflags = -DVERSION='"$(lib.version)"' -I$(OSVRDIR)/include
ldlibs = -L$(OSVRDIR)/lib -losvrClientKit

################################################################################
### pdlibbuilder ###############################################################
################################################################################

# This Makefile is based on the Makefile from pd-lib-builder written by
# Katja Vetter. You can get it from:
# https://github.com/pure-data/pd-lib-builder
PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
