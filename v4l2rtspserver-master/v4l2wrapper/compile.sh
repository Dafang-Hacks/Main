#!/usr/bin/env bash
TOOLCHAIN=$(pwd)/../../mips-gcc472-glibc216-64bit/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CROSS_COMPILE=${CROSS_COMPILE}
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export AR=${CROSS_COMPILE}ar
export CFLAGS="-muclibc -O2 "
export CPPFLAGS="-muclibc -O2"
export LDFLAGS="-muclibc -O2"
make clean
make all

