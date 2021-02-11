#!/usr/bin/env bash

set -e # fail out if any step fails

. ../../setCompilePath.sh

export CFLAGS="${CFLAGS} -std=c99 -DNULL=0 -DTCP_NOTSENT_LOWAT=25"

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
make os=linux HAVE_IPV6=0 CC="$CC $CFLAGS" LD="$LD" STRIP="$STRIP" LINKOPTS="-lrt" SAResponder
cp build/prod/mDNSResponderPosix ${INSTALLDIR}/bin/mDNSResponder
