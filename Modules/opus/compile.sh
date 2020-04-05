#!/usr/bin/env bash
export CFLAGS="-muclibc -O3 -DDEBUG_TRACE -DFAKE_ROOT "
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

if [ ! -d opus-1.3.1 ]
then
	wget https://archive.mozilla.org/pub/opus/opus-1.3.1.tar.gz
	tar xf opus-1.3.1.tar.gz
	rm opus-1.3.1.tar.gz
fi
cd opus-1.3.1/
./autogen.sh
./configure --host=mips-linux --prefix=${INSTALL}
make
cp --preserve=links -L .libs/lib* ${INSTALL}/lib 
cp include/* ${INSTALL}/include
