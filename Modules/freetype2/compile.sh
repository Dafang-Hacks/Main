#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

if [ ! -d freetype2/.git ]
then
  git clone git://git.sv.nongnu.org/freetype/freetype2.git
fi

echo "Compiling freetype"

cd freetype2
make clean
./autogen.sh
./configure --host=mips-linux-gnu --without-harfbuzz --without-png --without-zlib --without-bzip2 --prefix=${INSTALLDIR}
make install
cp objs/.libs/*.a ${INSTALLDIR}/lib
cp -r include ${INSTALLDIR}
