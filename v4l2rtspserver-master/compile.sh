#!/usr/bin/env bash
HOST=10.0.0.17
if [ "$#" -ge 1 ]; then
   HOST=$1
fi

TOOLCHAIN=$(pwd)/../toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CROSS_COMPILE=${CROSS_COMPILE}
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}g++
export PKG_CONFIG_PATH="$../_install/lib/pkgconfig"
export LIBRARY_PATH=../_install/lib
export CFLAGS="-muclibc -O3 -lrt -I../v4l2rtspserver-tools -I../_install/include/freetype2 -I../../_install/include/"
export CPPFLAGS="-muclibc -O3 -lrt -I../v4l2rtspserver-tools -I../_install/include/freetype2 -I../../_install/include/ -std=c++11"
export LDFLAGS="-muclibc -O3 -lrt -lstdc++ -lpthread -ldl"
rm CMakeCache.txt
rm -r CMakeFiles
cmake -DCMAKE_TOOLCHAIN_FILE="./dafang.toolchain"  -DCMAKE_INSTALL_PREFIX=./_install && make VERBOSE=1 -j4 install

if [ $? == 0 ]; then
  ${CROSS_COMPILE}strip -s _install/bin/*
  cp v4l2rtspserver-master.ini _install/bin/
  echo Copying to ${HOST} v4l2rtspserver-master
  ftp-upload --passive -h ${HOST} -u root --password ismart12 -d /system/sdcard/bin/ _install/bin/*
 
  for lib in libBasicUsageEnvironment.so libUsageEnvironment.so libgroupsock.so libliveMedia.so
  do
	  echo Copying to ${HOST}  ${lib}
	  ftp-upload --passive -h ${HOST} -u root --password ismart12 -d /system/sdcard/lib/ ../_install/lib/${lib}
  done
fi
