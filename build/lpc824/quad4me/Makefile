BUILD_ROOT ?= ../..

include $(BUILD_ROOT)/Makefile.triquad
include $(BUILD_ROOT)/Makefile.triquad_nxp


PROGRAM_NAME = randomtest

CHIP = LPC8xx

INIT_FILE ?= lpc8xx_ram_init
LD_SCRIPT ?= lpc824_ram.ld

EXTRA_CC_FLAGS  ?=  -DLPC82X
EXTRA_CXX_FLAGS ?=  -DLPC82X					\
		    -fno-threadsafe-statics			\
		    -DTRIQUAD_QUAD4ME				\
		     $(TRIQUAD_FLAGS)
EXTRA_INCLUDES  ?= -I$(DEVEL_ROOT)/include/triquad		\
-I$(DEVEL_ROOT)/quad4me\
-I$(DEVEL_ROOT)/ports/lpc824/triquad\
-I$(DEVEL_ROOT)/include/util\
		   -I$(DEVEL_ROOT)/include/nxp/lpc82x		\
		   -I$(DEVEL_ROOT)/ports/lpc824/util			\
		   -I$(DEVEL_ROOT)/util/quad4me			\
		   -I..
EXTRA_VPATH      =   $(DEVEL_ROOT)/randomtest			\
$(DEVEL_ROOT)/quad4me\
$(DEVEL_ROOT)/ports/lpc824/triquad\
		     $(DEVEL_ROOT)/ports/lpc824/util			\
		     $(DEVEL_ROOT)/util/quad4me

OPTIMIZE_FLAG  ?= -O1
STANDARD_FLAG  ?= -std=c++11
DEBUG_FLAG     ?= -g3

CXX_SRCS = $(PROGRAM_NAME).cxx 	\
	   mcu.cxx 		\
	   mrt.cxx 		\
	   sct.cxx		\
	   quad4me.cxx	 	\
	   quad4me_base.cxx

EXP_ROOT = ../..

include $(BUILD_ROOT)/Makefile.base
