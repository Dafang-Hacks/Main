#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

echo "Using Toolchain $TOOLCHAIN"

if [ ! -d lame-3.100 ]
then
	wget https://freefr.dl.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
	tar xf lame-3.100.tar.gz
	rm lame-3.100.tar.gz
fi

cd lame-3.100
./configure --host=mips-linux-gnu --prefix=${INSTALLDIR}
make clean
make install

cp -r include ${INSTALLDIR}
cp libmp3lame/.libs/libmp3lame.a ${INSTALLDIR}/lib
cp mpglib/.libs/libmpgdecoder.a ${INSTALLDIR}/lib
