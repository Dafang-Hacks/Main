#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

if [ ! -d dosfstools/.git ]
then
  git clone https://github.com/dosfstools/dosfstools
fi


cd dosfstools
autoreconf -i
./autogen.sh
./configure --host=mips-linux --prefix=${INSTALLDIR} --enable-compat-symlinks
make
make install
cp src/fatlabel src/fsck.fat src/mkfs.fat ${INSTALLDIR}/bin 
