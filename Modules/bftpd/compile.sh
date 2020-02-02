#!/usr/bin/env bash
export CFLAGS=""
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"
if [ ! -d bftpd ]
then
  wget https://iweb.dl.sourceforge.net/project/bftpd/bftpd/bftpd-5.4/bftpd-5.4.tar.gz
  tar xvfz bftpd-5.4.tar.gz
  rm bftpd-5.4.tar.gz
fi

cd bftpd/
make clean
./configure --host=mips-linux --enable-debug --libdir=${INSTALL}/lib
make
cp bftpd ${INSTALL}/bin
