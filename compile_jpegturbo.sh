#!/usr/bin/env bash
TOOLCHAIN=$(pwd)/mips-gcc472-glibc216-64bit/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

if [ ! -d libjpeg-turbo-1.5.3 ]
then
  wget https://freefr.dl.sourceforge.net/project/libjpeg-turbo/1.5.3/libjpeg-turbo-1.5.3.tar.gz
  tar xvfz libjpeg-turbo-1.5.3.tar.gz
fi

cd libjpeg-turbo-1.5.3 
./configure --host=mips-linux-gnu --prefix=$(pwd)/_Install  --enable-static 
make clean
make
make install


