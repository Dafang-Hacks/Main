INCLUDES     = -I./include/live555/usageEnvironment/ -I./include/live555/groupsock/ -I./include/live555/liveMedia/ -I./include/live555/basicUsageEnvironment -I../v4l2rtspserver-master/inc/
LIBS = ../v4l2rtspserver-master/_install/libs/libimp.so ../v4l2rtspserver-master/_install/libs/libalog.so  ../v4l2rtspserver-master/_install/libs/libimp.so ../live/liveMedia/libliveMedia.so ../live/groupsock/libgroupsock.so ../live/UsageEnvironment/libUsageEnvironment.so ../live/BasicUsageEnvironment/libBasicUsageEnvironment.so

CROSS_COMPILE=../toolchain/bin/mips-linux-gnu-

C            =         c
CPP          =           cpp
OBJ          =           o

CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip

CFLAGS = $(INCLUDES) -O2 -Wall -muclibc
CPLUSPLUSFLAGS = $(INCLUDES) -O2 -Wall -muclibc


LDFLAGS +=  -muclibc -lrt -lstdc++ -lpthread -ldl 

LINK_OBJ     = imp-common.o capture_and_encoding.o h264_video_streamer.o
all: 	my-carrier-server

.$(C).$(OBJ):
	$(CC) -c $(CFLAGS) $<

.$(CPP).$(OBJ):
	$(CPLUSPLUS) -c $(CPLUSPLUSFLAGS) $<

my-carrier-server: $(LIBS) $(LINK_OBJ)
	$(CPLUSPLUS) $(LDFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm -f *.o *~

distclean: clean
	rm -f $(APP)

