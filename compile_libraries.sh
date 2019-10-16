#!/usr/bin/env bash

cd lame
echo "Compiling lame"
./compile.sh
cd ..

cd live
echo "Compiling live"
./compile.sh
cd ..

cd opus
echo "Compiling opus"
./compile.sh
cd ..

cd freetype2
echo "Compiling freetype"
TOOLCHAIN=$(pwd)/../toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
make clean
export CC=${CROSS_COMPILE}gcc
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

./autogen.sh
./configure --host=mips-linux-gnu --without-harfbuzz --without-png --without-zlib --prefix=$(pwd)/_install
make install
cd ..
