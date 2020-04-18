#!/usr/bin/env bash

if [ -z "$TOOLCHAIN" ]
then
	ROOTPATH=$(git rev-parse --show-toplevel)
	echo "setting $ROOTPATH"
	cd ${ROOTPATH}
	export TOOLCHAIN=$(pwd)/toolchain/bin
	export CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
	export CC=${CROSS_COMPILE}gcc
	export LD=${CROSS_COMPILE}ld
	export CCLD=${CROSS_COMPILE}ld
	export CXX=${CROSS_COMPILE}g++
	export CPP=${CROSS_COMPILE}cpp
	export CXXCPP=${CROSS_COMPILE}cpp
	export AR=${CROSS_COMPILE}ar
	export INSTALL=$(pwd)/_install
	cd -
fi
