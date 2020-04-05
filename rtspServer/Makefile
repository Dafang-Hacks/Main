INCLUDES     = -I./inc -I../Modules/live/live/liveMedia/include/ -I../Modules/live/live/UsageEnvironment/include/ -I../Modules/live/live/groupsock/include/ -I../Modules/live/live/BasicUsageEnvironment/include/ -I../v4l2rtspserver-master/inc/ -I../v4l2rtspserver-tools/ -I../Modules/freetype2/freetype2/include -I../_install/include
LIBS = ../_install/lib/libimp.so ../_install/lib/libalog.so  ../_install/lib/libimp.so ../_install/lib/libliveMedia.so ../_install/lib//libgroupsock.so ../_install/lib/libUsageEnvironment.so ../_install/lib/libBasicUsageEnvironment.so ../_install/lib/libfreetype.a ../_install/lib/libssl.a ../_install/lib/libtls.a ../_install/lib/libcrypto.a -lpthread

CROSS_COMPILE=../toolchain/bin/mips-linux-gnu-

LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip
#CFLAGS=-muclibc -O3 -lrt $(INCLUDES)
#CPLUSPLUSFLAGS=$(INCLUDES) -O3 -lrt -std=c++11 -DLOGURU_STACKTRACES=0

LDFLAGS=-muclibc -O3 -lrt -lstdc++ -lpthread -ldl

#CFLAGS = $(INCLUDES) -O3 -Wall -muclibc
#CPLUSPLUSFLAGS = $(INCLUDES) -O3 -Wall -muclibc -std=c++11 -DLOGURU_STACKTRACES=0


LDFLAGS +=  -muclibc -lrt -lstdc++ -lpthread -ldl  

#Compiler and Linker
CC          := $(CROSS_COMPILE)gcc
CXX         := $(CROSS_COMPILE)g++

#The Target Binary Program
CFLAGS      := -O3 -lrt -std=c++11 -DLOGURU_STACKTRACES=0
TARGET_EXEC ?= rtspServer

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(INCLUDES)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p




