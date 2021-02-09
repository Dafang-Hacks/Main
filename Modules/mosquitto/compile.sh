#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

export CROSS_COMPILE=${ROOTPATH}/toolchain
export CC=/bin/mips-linux-gnu-gcc
export AR=/bin/mips-linux-gnu-ar
export CXX=/bin/mips-linux-gnu-g++
export LDFLAGS="${LDFLAGS} -lrt -lssl -ltls -lcrypto -lpthread"

if [ ! -d mosquitto/.git ]
then
  git clone https://github.com/eclipse/mosquitto.git
  patch mosquitto/config.mk config.diff 
fi

cd mosquitto
make

cp client/mosquitto_sub ${INSTALLDIR}/bin/mosquitto_sub.bin
cp client/mosquitto_pub ${INSTALLDIR}/bin/mosquitto_pub.bin
cp src/mosquitto ${INSTALLDIR}/bin/mosquitto.bin
cp lib/libmosquitto.so.1 ${INSTALLDIR}/lib
ln -s ${INSTALLDIR}/lib/libmosquitto.so.1 ${INSTALLDIR}/lib/libmosquitto.so
cp include/* ${INSTALLDIR}/include
