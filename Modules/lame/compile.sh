#!/usr/bin/env bash

. ../../setCompilePath.sh
echo "Using Toolchain $TOOLCHAIN"

export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

if [ ! -d lame-3.100 ]
then
	wget https://freefr.dl.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
	tar xf lame-3.100.tar.gz
	rm lame-3.100.tar.gz
fi

cd lame-3.100
make clean
./configure --host=mips-linux-gnu --prefix=${INSTALL}
make libmp3lame.so
make install

cp -r include ${INSTALL}
cp libmp3lame/.libs/libmp3lame.a ${INSTALL}/lib
cp mpglib/.libs/libmpgdecoder.a ${INSTALL}/lib

