#!/usr/bin/env bash
ROOTPATH=$(git rev-parse --show-toplevel)
INSTALL=${ROOTPATH}/_install
export CROSS_COMPILE=${ROOTPATH}/toolchain
export CC=/bin/mips-linux-gnu-gcc
export AR=/bin/mips-linux-gnu-ar
export CXX=/bin/mips-linux-gnu-g++
export CFLAGS="-muclibc -O3 -I${INSTALL}/include"
export CPPFLAGS="-muclibc -O3 -I${INSTALL}/include"
export LDFLAGS="-muclibc -O3 -lrt -L${INSTALL}/lib -lssl -ltls -lcrypto -lpthread"
if [ ! -d mosquitto/.git ]
then
  git clone https://github.com/eclipse/mosquitto.git
  patch mosquitto/config.mk config.diff 
fi

cd mosquitto
make

cp client/mosquitto_sub ${INSTALL}/bin/mosquitto_sub.bin
cp client/mosquitto_pub ${INSTALL}/bin/mosquitto_pub.bin
cp src/mosquitto ${INSTALL}/bin/mosquitto.bin
cp lib/libmosquitto.so.1 ${INSTALL}/lib
cp include/* ${INSTALL}/include
