CROSS_COMPILE=        ../mips-gcc472-glibc216-64bit/bin/mips-linux-gnu-
CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar cr
STRIP = $(CROSS_COMPILE)strip
CFLAGS = $(INCLUDES) -O2 -Wall -march=mips32r2
CFLAGS += -muclibc
LDFLAG += -muclibc


LDFLAG += -Wl,-gc-sections


all:  getimage setconf

getimage: getimage.cpp sharedmem.cpp
	$(CPLUSPLUS) $(LDFLAG) -o $@ $^ $(LIBS) -lpthread -lm -lrt
	$(STRIP) $@

setconf: setconf.cpp sharedmem.cpp
	$(CPLUSPLUS) $(LDFLAG) -o $@ $^ $(LIBS) -lpthread -lm -lrt
	$(STRIP) $@



%.o:%.c sample-common.h
	$(CC) -c $(CFLAGS) $< -o $@

%.o:%.cpp
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@


clean:
	rm -f *.o *~

distclean: clean
	rm -f $(SAMPLES)
