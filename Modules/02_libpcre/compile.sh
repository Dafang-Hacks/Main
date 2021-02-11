#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

if [ ! -d pcre-8.43 ]
then
    wget https://ftp.pcre.org/pub/pcre/pcre-8.43.tar.gz
    tar xvfz pcre-8.43.tar.gz
    rm pcre-8.43.tar.gz
fi

cd pcre-8.43
#./configure --host=mips-linux-gnu --prefix=${INSTALLDIR} --with-gnu-ld
./configure --host=mips-linux-gnu --prefix=${INSTALLDIR}
make clean
make
make install
