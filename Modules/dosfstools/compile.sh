#!/usr/bin/env bash
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"
. ../../setCompilePath.sh
if [ ! -d dosfstools/.git ]
then
  git clone https://github.com/dosfstools/dosfstools
fi


cd dosfstools
autoreconf -i
./autogen.sh
./configure --host=mips-linux --prefix=${INSTALL} --enable-compat-symlinks
make
make install
cp src/fatlabel src/fsck.fat src/mkfs.fat ${INSTALL}/bin 
