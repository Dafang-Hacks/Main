#!/usr/bin/env bash
unset CROSS_COMPILE
. ../../setCompilePath.sh
export LIBSSL="${INSTALL}/lib/libtls.a ${INSTALL}/lib/libssl.a ${INSTALL}/lib/libcrypto.a -lpthread"
if [ ! -d mosquitto/.git ]
then
  git clone https://github.com/eclipse/mosquitto.git
  #Use .a lib instead
  sed -i 's/\-lssl//' mosquitto/config.mk 
  sed -i 's/\-lcrypto//' mosquitto/config.mk 
  LIBSPECIAL=$(echo "$LIBSSL" | sed 's/\//\\\//g')
  sed -i "s/CLIENT_STATIC_LDADD:=$/CLIENT_STATIC_LDADD:=${LIBSPECIAL}/" mosquitto/config.mk
  sed -i "s/BROKER_LDADD:=$/BROKER_LDADD:=${LIBSPECIAL}/" mosquitto/config.mk
  sed -i "s/CLIENT_LDADD:=$/CLIENT_LDADD:=${LIBSPECIAL}/" mosquitto/config.mk
  sed -i "s/PASSWD_LDADD:=$/PASSWD_LDADD:=${LIBSPECIAL}/" mosquitto/config.mk
  sed -i "s/WITH_STATIC_LIBRARIES:=no/WITH_STATIC_LIBRARIES:=yes/" mosquitto/config.mk
  sed -i "s/WITH_SHARED_LIBRARIES:=yes/WITH_SHARED_LIBRARIES:=no/" mosquitto/config.mk
  sed -i "s/WITH_MEMORY_TRACKING:=yes/WITH_MEMORY_TRACKING:=no/" mosquitto/config.mk

fi

export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3 -I${INSTALL}/include -L${INSTALL}/lib "
export LDFLAGS="-muclibc -O3 -I${INSTALL}/include -L${INSTALL}/lib ${LIBSSL}"

cd mosquitto
make DESTDIR=$INSTALL

cp client/mosquitto_sub ${INSTALL}/bin/mosquitto_sub.bin
cp client/mosquitto_pub ${INSTALL}/bin/mosquitto_pub.bin
