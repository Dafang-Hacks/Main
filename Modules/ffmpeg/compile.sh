#!/usr/bin/env bash
set -e
. ../../setCompilePath.sh
export BUILD_PATH="$(pwd)/build"
FFMPEG_VERSION=4.3.2

if [ ! -d ffmpeg-$FFMPEG_VERSION ]
then
	wget https://ffmpeg.org/releases/ffmpeg-$FFMPEG_VERSION.tar.gz
	tar xf ffmpeg-$FFMPEG_VERSION.tar.gz
fi

cd ffmpeg-$FFMPEG_VERSION
./configure --enable-cross-compile --arch=mips --prefix=$BUILD_PATH --target-os=linux --extra-ldflags=-static \
    --disable-inline-asm --disable-mips32r5 --disable-mips64r6 --disable-mipsdspr2 --disable-mipsdsp --disable-mipsdspr2 \
    --disable-encoders --disable-decoders --disable-asm --disable-doc --disable-yasm --disable-optimizations  \
    --cross-prefix=mips-linux-gnu- --disable-hwaccels

make clean
make
make install
exit

#cp ../build/bin/* ${INSTALLDIR}/bin/
