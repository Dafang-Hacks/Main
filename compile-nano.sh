#!/usr/bin/env bash
export INSTALLPATH="$(pwd)/_install"
TOOLCHAIN=$(pwd)/toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}cpp
export LD=${CROSS_COMPILE}ld
export CFLAGS="-muclibc -O2 -static"
export CPPFLAGS="-muclibc -O2 -I${INSTALLPATH}/include -I${INSTALLPATH}/include/ncursesw"
export LDFLAGS="-muclibc -O2 -L${INSTALLPATH}/lib"

if [ ! -d nano-4.3 ]
then
  wget https://www.nano-editor.org/dist/v4/nano-4.3.tar.gz
  tar xvf nano-4.3.tar.gz
fi

cd nano-4.3

./configure --prefix=${INSTALLPATH} --host=mipsel-linux --enable-utf8 --enable-nls --enable-color --enable-multibuffer --enable-nanorc

# Last known good configuration:
#./configure --prefix=/root/Main/_install --host=mipsel-linux --enable-utf8 --disable-nls --enable-color --enable-extra --enable-multibuffer --enable-nanorc

make -j4
make install
