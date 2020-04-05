#!/usr/bin/env bash
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

compile()
{
	module=${1}
	echo Compiling ${module}
	cd ${MODULESROOTPATH}/${module}
	source compile.sh 
}

mkdir -p ${INSTALL}
mkdir -p ${INSTALL}/lib
mkdir -p ${INSTALL}/bin
mkdir -p ${INSTALL}/include
export MODULESROOTPATH=$(pwd)
for module in Modules/* 
do
        compile ${module}
done
