#!/usr/bin/env bash
export CFLAGS="-muclibc -O3 -DFAKE_ROOT "
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"
. ../../setCompilePath.sh
if [ ! -d dropbear ]
then
	hg clone https://secure.ucc.asn.au/hg/dropbear
fi
cp *.h dropbear
cd dropbear/
echo '#define DEFAULT_PATH "/usr/bin:/bin:/system/bin:/system/sdcard/bin"' > localoptions.h

autoconf; autoheader
make clean
./configure --host=mips-linux --disable-zlib
make PROGRAMS="dropbear dbclient scp dropbearkey dropbearconvert" MULTI=1 SCPPROGRESS=1

cp dropbearmulti ${INSTALL}/bin
