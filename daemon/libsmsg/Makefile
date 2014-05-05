TARGET	    = libsmsg.a
SPEC_TARGET = libsmsg-spec
SRCS  	    = $(wildcard *.cpp)
SPECS 	    = $(wildcard spec/*.cpp)
OBJS	    = $(SRCS:.cpp=.o)
SPECOBJS    = $(SPECS:.cpp=.o) spec/gtest/gmock-gtest-all.o  spec/gtest/gmock_main.o
CXXFLAGS    += -I. -I./spec/gtest

all: $(TARGET)

spec: $(SPEC_TARGET)
	./$(SPEC_TARGET)

$(TARGET): $(OBJS)
	$(AR) -r $(TARGET) $(OBJS)

$(SPEC_TARGET): $(OBJS) $(SPECOBJS)
	$(CXX) -o $(SPEC_TARGET) $(OBJS) $(SPECOBJS) -pthread

clean:
	rm -fv $(TARGET) $(OBJS) $(SPECOBJS) $(SPEC_TARGET)
