#!/usr/bin/env bash

if [ ! -d freetype2/.git ]
then
  git clone git://git.sv.nongnu.org/freetype/freetype2.git
fi

echo "Compiling freetype"
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

cd freetype2
make clean
./autogen.sh
./configure --host=mips-linux-gnu --without-harfbuzz --without-png --without-zlib --prefix=${INSTALL}
make install
cp objs/.libs/*.a ${INSTALL}/lib
cp -r freetype2/include/ ${INSTALL}
