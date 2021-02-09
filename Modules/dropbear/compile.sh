#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

export CFLAGS="${CFLAGS} -DFAKE_ROOT"
if [ ! -d dropbear ]
then
    git clone https://github.com/mkj/dropbear
fi
cp *.h dropbear
cd dropbear/
echo '#define DEFAULT_PATH "/usr/bin:/bin:/system/bin:/system/sdcard/bin"' > localoptions.h

autoconf; autoheader
./configure --host=mips-linux --disable-zlib
make clean
make PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" MULTI=1 SCPPROGRESS=1

cp dropbearmulti ${INSTALLDIR}/bin
