#Prject Info
TARGET  = twjoyd
INC_DIR += ../libsmsg ../proto ../../
LIB_DIR += ../libsmsg
SRC_DIR = .
LIB     += -lsmsg -lPocoFoundation -lPocoUtil -lPocoNet -lserial -pthread
OUT_DIR = ./bin
LIB_DEP =

#Makefile Template For Application
TARGET  := $(strip $(TARGET))
#CC      = gcc$(GCC_SUFFIX)
#CXX     = g++$(GCC_SUFFIX)
#CPP     = cpp$(GCC_SUFFIX)
CFLAGS  += $(C_INC) -Wall
CXXFLAGS+= $(C_INC) -std=gnu++0x -Wall

C_INC   = $(INC_DIR:%=-I%) $(INC)
C_LIB   = $(LIB_DIR:%=-L%) $(LIB)

C_SRC   = $(wildcard $(SRC_DIR:%=%/*.c))
CXX_SRC = $(wildcard $(SRC_DIR:%=%/*.cpp))
C_OBJ   = $(C_SRC:%.c=%.o)
CXX_OBJ = $(CXX_SRC:%.cpp=%.o)
C_DEP   = $(C_SRC:%.c=%.d)
CXX_DEP = $(CXX_SRC:%.cpp=%.d)

ALL_OBJ = $(C_OBJ) $(CXX_OBJ)
ALL_DEP = $(C_DEP) $(CXX_DEP)

ifeq ($(debug), 1)
CFLAGS   += -O0 -g -DLOCAL_DEBUG
CXXFLAGS += -O0 -g -DLOCAL_DEBUG
endif


.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(ALL_OBJ) $(ALL_DEP) $(TARGET) $(OUT_DIR)/$(TARGET)

$(C_DEP): %.d : %.c
	$(CPP) $(C_INC) -MM $< > $@

$(CXX_DEP): %.d : %.cpp
	$(CPP) $(C_INC) -MM $< > $@


ifneq ($(MAKECMDGOALS), clean)
-include $(ALL_DEP)
endif

$(TARGET): $(ALL_DEP) $(ALL_OBJ)
	$(CXX) $(ALL_OBJ) $(C_LIB) $(LDFLAGS) -o $@
