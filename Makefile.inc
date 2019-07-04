mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS += \
util.o \
Tracer.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)unit \
    $(mkfile_path)trace \
    $(mkfile_path)util

INCLUDES += \
	-I$(mkfile_path)unit \
    -I$(mkfile_path)trace \
    -I$(mkfile_path)util

COPTS += -fno-use-cxa-atexit
