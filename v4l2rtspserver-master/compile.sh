#!/usr/bin/env bash

TOOLCHAIN=$(pwd)/../toolchain/bin
export CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}g++
export PKG_CONFIG_PATH="$../_install/lib/pkgconfig"
export LIBRARY_PATH=../_install/lib
export CFLAGS="-muclibc -O3 -lrt -I../v4l2rtspserver-tools -I../_install/include/freetype2 -I../../_install/include/"
export CPPFLAGS="-muclibc -O3 -lrt -I../v4l2rtspserver-tools -I../_install/include/freetype2 -I../../_install/include/ -std=c++11"
export LDFLAGS="-muclibc -O3 -lrt -lstdc++ -lpthread -ldl"
rm CMakeCache.txt
rm -r CMakeFiles
cmake -DCMAKE_TOOLCHAIN_FILE="./dafang.toolchain"  -DCMAKE_INSTALL_PREFIX=./_install --debug-output && make VERBOSE=1 -j4 install

err=$?
if [ $err != 0 ]; then
    exit $err
else
    ${CROSS_COMPILE}strip -s _install/bin/*
    cp v4l2rtspserver-master.ini _install/bin/

    echo '-------------------------------------------------------'
    echo 'all good, run ./deploy.sh <ip> to deploy to your camera'
    exit 0
fi
