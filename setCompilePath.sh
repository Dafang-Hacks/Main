#!/usr/bin/env bash

ROOTPATH=$(git rev-parse --show-toplevel)
echo "setting $ROOTPATH"
export TOOLCHAIN=${ROOTPATH}/toolchain/bin
export CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export CCLD=${CROSS_COMPILE}ld
export CXX=${CROSS_COMPILE}g++
export CXXLD=${CROSS_COMPILE}ld
export CPP=${CROSS_COMPILE}cpp
export CXXCPP=${CROSS_COMPILE}cpp
export AR=${CROSS_COMPILE}ar
export STRIP=${CROSS_COMPILE}strip
export INSTALLDIR=${ROOTPATH}/_install

export CFLAGS="-muclibc -O3 -I${INSTALLDIR}/include"
export CPPFLAGS="-muclibc -O3 -I${INSTALLDIR}/include"
export CXXFLAGS="-muclibc -O3 -I${INSTALLDIR}/include"
export LDFLAGS="-muclibc -O3 -L${INSTALLDIR}/lib"
