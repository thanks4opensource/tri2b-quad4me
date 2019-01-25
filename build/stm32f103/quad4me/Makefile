BUILD_ROOT ?= ../..

include $(BUILD_ROOT)/Makefile.triquad
include $(BUILD_ROOT)/Makefile.triquad_stm

PROGRAM_NAME = randomtest

CHIP = STM32F1xx

INIT_FILE ?= stm32f103_ram_init
LD_SCRIPT ?= stm32f103_ram.ld

EXTRA_CXX_FLAGS ?= -fno-threadsafe-statics			\
		   -DTRIQUAD_QUAD4ME				\
		    $(TRIQUAD_FLAGS)
EXTRA_INCLUDES  ?= -I$(DEVEL_ROOT)/include/triquad		\
-I$(DEVEL_ROOT)/triquad\
-I$(DEVEL_ROOT)/quad4me\
-I$(DEVEL_ROOT)/quad4me\
-I$(DEVEL_ROOT)/ports/stm32f103/triquad\
-I$(DEVEL_ROOT)/include/stm\
-I$(DEVEL_ROOT)/include/util\
		   -I$(DEVEL_ROOT)/include/st/32f		\
		   -I$(DEVEL_ROOT)/ports/stm32f103/util	\
		   -I$(DEVEL_ROOT)/util/quad4me			\
		   -I$(DEVEL_ROOT)/util/triquad			\
		   -I..
EXTRA_VPATH      =   $(DEVEL_ROOT)/randomtest			\
$(DEVEL_ROOT)/triquad\
$(DEVEL_ROOT)/quad4me\
$(DEVEL_ROOT)/ports/stm32f103/triquad\
		     $(DEVEL_ROOT)/ports/stm32f103/util	\
		     $(DEVEL_ROOT)/util/quad4me			\
		     $(DEVEL_ROOT)/util/triquad

OPTIMIZE_FLAG  ?= -O1
STANDARD_FLAG  ?= -std=c++11
DEBUG_FLAG     ?= -g3

CXX_SRCS = $(PROGRAM_NAME).cxx 	\
	   mcu.cxx 		\
           tim.cxx 		\
           tim_m_s.cxx 		\
	   tri_quad.cxx		\
	   quad4me.cxx	 	\
	   quad4me_base.cxx	\
	   $(HRDW_SRCS)

EXP_ROOT = ../..

include $(BUILD_ROOT)/Makefile.base
