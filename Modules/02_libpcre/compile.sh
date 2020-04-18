#!/usr/bin/env bash
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

. ../../setCompilePath.sh
if [ ! -d pcre-8.43/ ]
then
    wget  wget https://ftp.pcre.org/pub/pcre/pcre-8.43.tar.gz
    tar xvfz pcre-8.43.tar.gz
    rm pcre-8.43.tar.gz
fi

cd pcre-8.43
./configure --host=mips-linux-gnu --prefix=${INSTALL}  --enable-static   --disable-shared --with-gnu-ld
make clean
make
make install

cp .libs/*.a ${INSTALL}/lib
cp pcre-config ${INSTALL}/bin
cp pcre.h ${INSTALL}/include
