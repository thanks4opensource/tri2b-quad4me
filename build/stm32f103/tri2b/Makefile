BUILD_ROOT ?= ../..

include $(BUILD_ROOT)/Makefile.triquad
include $(BUILD_ROOT)/Makefile.triquad_stm


PROGRAM_NAME = randomtest

CHIP = STM32F1xx

INIT_FILE ?= stm32f103_ram_init
LD_SCRIPT ?= stm32f103_ram.ld

EXTRA_CXX_FLAGS ?= -fno-threadsafe-statics			\
		   -DTRIQUAD_TRI2B				\
		    $(TRIQUAD_FLAGS)
EXTRA_INCLUDES  ?= -I$(DEVEL_ROOT)/include/triquad		\
-I$(DEVEL_ROOT)/tri2b\
-I$(DEVEL_ROOT)/ports/stm32f103/triquad\
-I$(DEVEL_ROOT)/include/stm\
-I$(DEVEL_ROOT)/include/util\
		   -I$(DEVEL_ROOT)/include/st/32f		\
		   -I$(DEVEL_ROOT)/ports/stm32f103/util	\
		   -I$(DEVEL_ROOT)/util/tri2b			\
		   -I..
EXTRA_VPATH      =   $(DEVEL_ROOT)/randomtest			\
$(DEVEL_ROOT)/tri2b\
$(DEVEL_ROOT)/ports/stm32f103/triquad\
		     $(DEVEL_ROOT)/ports/stm32f103/util	\
		     $(DEVEL_ROOT)/util/tri2b

OPTIMIZE_FLAG  ?= -O1
STANDARD_FLAG  ?= -std=c++11
DEBUG_FLAG     ?= -g3

CXX_SRCS = $(PROGRAM_NAME).cxx	\
	   mcu.cxx 		\
	   tim.cxx		\
           tim_m_s.cxx 		\
	   tri2b.cxx 	 	\
	   tri2b_base.cxx

EXP_ROOT = ../..

include $(BUILD_ROOT)/Makefile.base
