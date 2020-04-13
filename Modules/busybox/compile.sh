#!/usr/bin/env bash
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

if [ ! -d busybox/.git ]
then
  git clone --depth=1  git://git.busybox.net/busybox
fi

cd busybox/
make clean
make CROSS_COMPILE=$CROSS_COMPILE defconfig
#Remove some packages that doesn't compile
sed -e 's/CONFIG_FALLOCATE=y/CONFIG_FALLOCATE=n/' -i .config
sed -e 's/CONFIG_NSENTER=y/CONFIG_NSENTER=n/' -i .config
sed -e 's/CONFIG_FSYNC=y/CONFIG_FSYNC=n/' -i .config
sed -e 's/CONFIG_SYNC=y/CONFIG_SYNC=n/' -i .config
make CROSS_COMPILE=$CROSS_COMPILE

cp busybox ${INSTALL}/bin
