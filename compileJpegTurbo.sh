#!/usr/bin/env bash
TOOLCHAIN=$(pwd)/toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

if [ ! -d libjpeg-turbo/.git ]
then
   git clone https://github.com/libjpeg-turbo/libjpeg-turbo
fi

cd libjpeg-turbo
./configure --host=mips-linux-gnu --prefix=$(pwd)/_Install  --enable-static   --disable-shared
make clean
make
make install
