INCLUDES     = -I ../Modules/live/live/liveMedia/include/ -I ../Modules/live/live/UsageEnvironment/include/ -I ../Modules/live/live/groupsock/include/ -I../Modules/live/live/BasicUsageEnvironment/include/ -I../v4l2rtspserver-master/inc/ -I../Modules/freetype2/include -I../v4l2rtspserver-tools/ -I../Modules/freetype2/freetype2/include/
LIBS = ../v4l2rtspserver-master/_install/libs/libimp.so ../v4l2rtspserver-master/_install/libs/libalog.so  ../v4l2rtspserver-master/_install/libs/libimp.so ../_install/lib/libliveMedia.so ../_install/lib//libgroupsock.so ../_install/lib/libUsageEnvironment.so ../_install/lib/libBasicUsageEnvironment.so ../_install/lib/libfreetype.a 

CROSS_COMPILE=../toolchain/bin/mips-linux-gnu-

C            =         c
CPP          =           cpp
OBJ          =           o

CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip
CFLAGS=-muclibc -O3 -lrt $(INCLUDES)
CPLUSPLUSFLAGS=$(INCLUDES) -O3 -lrt -std=c++11 -DLOGURU_STACKTRACES=0

LDFLAGS=-muclibc -O3 -lrt -lstdc++ -lpthread -ldl

#CFLAGS = $(INCLUDES) -O3 -Wall -muclibc
#CPLUSPLUSFLAGS = $(INCLUDES) -O3 -Wall -muclibc -std=c++11 -DLOGURU_STACKTRACES=0


LDFLAGS +=  -muclibc -lrt -lstdc++ -lpthread -ldl  

LINK_OBJ     = h264_video_streamer.o ImpEncoder.o OSD.o sharedmem.o
all:  rtspServer	

.$(C).$(OBJ):
	$(CC) -c $(CFLAGS) $<

.$(CPP).$(OBJ):
	$(CPLUSPLUS) $(CPLUSPLUSFLAGS) -c $<

rtspServer: $(LIBS) $(LINK_OBJ)
	$(CPLUSPLUS) $(LDFLAGS) -o $@ $^ $(LIBS) 

clean:
	rm -f *.o *~

distclean: clean
	rm -f $(APP)

