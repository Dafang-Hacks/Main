#!/usr/bin/env bash

#export CFLAGS="-muclibc -DDEBUG_TRACE -DFAKE_ROOT -g -ggdb"
#export CPPFLAGS="-muclibc -g -ggdb"
#export LDFLAGS="-muclibc -g -ggdb"

#export CFLAGS="-muclibc -O3"
#export CPPFLAGS="-muclibc -O3"
#export LDFLAGS="-muclibc -O3"

export LIBSSLL=${INSTALL}

if [ ! -d live555 ]
then
   #git clone https://github.com/rgaufman/live555.git
   wget http://www.live555.com/liveMedia/public/live555-latest.tar.gz
   tar xvfz live555-latest.tar.gz
   rm live555-latest.tar.gz
   mv live live555
fi

cd live555/


cat << EOF > config.dafang 
libliveMedia_LIB_SUFFIX=so

libBasicUsageEnvironment_VERSION_CURRENT=1
libBasicUsageEnvironment_VERSION_REVISION=0
libBasicUsageEnvironment_VERSION_AGE=0
libBasicUsageEnvironment_LIB_SUFFIX=so

libUsageEnvironment_VERSION_CURRENT=4
libUsageEnvironment_VERSION_REVISION=0
libUsageEnvironment_VERSION_AGE=1
libUsageEnvironment_LIB_SUFFIX=so

libgroupsock_VERSION_CURRENT=9
libgroupsock_VERSION_REVISION=1
libgroupsock_VERSION_AGE=1
libgroupsock_LIB_SUFFIX=so

#####

LIBSSL=\$(LIBSSLL)/lib/libssl.a \$(LIBSSLL)/lib/libcrypto.a \$(LIBSSLL)/lib/libtls.a  -pthread 
OBJ =			o
COMPILE_OPTS =        \$(INCLUDES) -I\$(LIBSSLL)/include -I. -O3 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -DXLOCALE_NOT_USED=1 -DLOCALE_NOT_USED -DBS1 -fPIC -DALLOW_RTSP_SERVER_PORT_REUSE=1 

C =            c
C_COMPILER =        \$(CROSS_COMPILE)gcc
CFLAGS +=        \$(COMPILE_OPTS)
C_FLAGS = \$(CFLAGS)
CPP =            cpp
CPLUSPLUS_COMPILER =    \$(CROSS_COMPILE)g++
CPLUSPLUS_FLAGS =    \$(COMPILE_OPTS) -Wall -DBSD=1
CPLUSPLUS_FLAGS += \$(CPPFLAGS) -fexceptions
OBJ =            o
LINK =            \$(CROSS_COMPILE)g++ -o
LINK_OPTS =        -L. \$(LDFLAGS)
CONSOLE_LINK_OPTS =    \$(LINK_OPTS)
LIBRARY_LINK =		\$(CROSS_COMPILE)gcc -o

SHORT_LIB_SUFFIX =	so
LIB_SUFFIX =	 	\$(SHORT_LIB_SUFFIX)
LIBRARY_LINK_OPTS =	\$(LIBSSL) -shared -Wl,-soname,\$(NAME).\$(SHORT_LIB_SUFFIX) \$(LDFLAGS)
LIBS_FOR_CONSOLE_APPLICATION = \$(CXXLIBS) \$(LIBSSL) 
LIBS_FOR_GUI_APPLICATION = \$(LIBS_FOR_CONSOLE_APPLICATION) \$(LIBSSL) 
EXE =

EOF

./genMakefiles dafang
make clean
make
cp BasicUsageEnvironment/libBasicUsageEnvironment.so ${INSTALL}/lib
cp UsageEnvironment/libUsageEnvironment.so ${INSTALL}/lib
cp groupsock/libgroupsock.so ${INSTALL}/lib
cp liveMedia/libliveMedia.so ${INSTALL}/lib
cd ..

