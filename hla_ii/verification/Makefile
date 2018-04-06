
SET_WAVEFORM = 0

BLOCK_PATH = $$PWD/..
BLOCK_TB = $(BLOCK_PATH)/verification

CC=g++

CERTI = -DRTI_USES_STD_FSTREAM -I$$CERTI_HOME/include/hla13 -I$$CERTI_HOME/include/libhla -L$$CERTI_HOME/lib 
CERTI_FLAGS = -lRTI-NGd -lFedTimed -lHLAd -lCERTId 

CPPFLAGS = -lsystemc -lm -pthread -g

SYSTEMC_LIB = $$SYSTEMC_HOME/lib-linux64/libsystemc.a
LIBS =  $(SYSTEMC_LIB)

SYSTEMCFLAGS += -DSC_INCLUDE_DYNAMIC_PROCESSES
SYSTEMCFLAGS += -I$$SYSTEMC_HOME/include
SYSTEMCLIBS += -L$$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$$SYSTEMC_HOME/lib-linux64

#Virtualbus
VIRTUALBUS_PATH = $(BLOCK_PATH)/virtualbus
VIRTUALBUS_FILES = $(VIRTUALBUS_PATH)/FederateAmbassador.cpp 
VIRTUALBUS_FILES += $(VIRTUALBUS_PATH)/VirtualBusFederate.cpp

#Testbench 
INC_COMP   = -I$(BLOCK_TB)/components
INC_TESTS  = -I$(BLOCK_TB)/tests
CPP_TOP_TB := $(BLOCK_TB)/ii_top_tb.cpp


#TRACE
ifeq ($(SET_WAVEFORM),1) 
  DEFINES += -D_TRACE_ 
endif

#REFERENCE MODEL
PATH_RM = $(BLOCK_PATH)/reference_model
ARCH_RM = $(PATH_RM)/rm_ii.cpp 

#DUT
PATH_DUT = $(BLOCK_PATH)/design
TARGET = run_verification
ARCH_DUT = $(PATH_DUT)/ii.cpp 
 
all: 
	$(CC) $(DEFINES) -I. $(CERTI) -I$(VIRTUALBUS_PATH) -I$(PATH_DUT) -I$(PATH_RM) $(INC_COMP) $(INC_TESTS) $(VIRTUALBUS_FILES) $(ARCH_DUT) $(ARCH_RM) $(CPP_TOP_TB) $(SYSTEMCLIBS) $(LIBS) -D_SYSTEMC_ -o $(TARGET) $(SYSTEMCFLAGS) $(CPPFLAGS) $(CERTI_FLAGS)


clean:
	@rm -rf $(TARGET) *.vcd logs *.log results/*

