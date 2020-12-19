#!/usr/bin/env bash

TOOLCHAIN=$(pwd)/../../toolchain/bin
INSTALL=$(pwd)/../../_install
CROSS_COMPILE=$TOOLCHAIN/mips-linux-uclibc-gnu-
export CROSS_COMPILE=${CROSS_COMPILE}
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export STRIP=${CROSS_COMPILE}strip
export CFLAGS="-muclibc -O3 -std=c99 -DNULL=0 -DTCP_NOTSENT_LOWAT=25"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

VERSION="1096.40.7"

if [ ! -d "mDNSResponder-${VERSION}/" ]
then
    wget "https://opensource.apple.com/tarballs/mDNSResponder/mDNSResponder-${VERSION}.tar.gz"
    tar xvfz "mDNSResponder-${VERSION}.tar.gz" --exclude='._*'
    rm "mDNSResponder-${VERSION}.tar.gz"
    patch -d "mDNSResponder-${VERSION}/" -p1 <unicast.patch
fi

cd "mDNSResponder-${VERSION}/mDNSPosix"
make os=linux clean
make os=linux HAVE_IPV6=0 CC="$CC" LD="$LD" STRIP="$STRIP" LINKOPTS="-lrt" SAResponder
cp build/prod/mDNSResponderPosix ${INSTALL}/bin/mDNSResponder
