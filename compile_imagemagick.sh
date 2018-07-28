#!/usr/bin/env bash
export JPEGPATH="$(pwd)/libjpeg-turbo-1.5.3/_Install/"
TOOLCHAIN=$(pwd)/toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}cpp
export LD=${CROSS_COMPILE}ld
export CFLAGS="-muclibc -O2"
export CPPFLAGS="-muclibc -O2 -I${JPEGPATH}/include/"
export LDFLAGS="-muclibc -O2 -L${JPEGPATH}/lib"

cd ImageMagick
./configure --host=mipsel-linux --prefix=${PWD}/_install  --disable-openmp --disable-opencl --without-bzlib --without-dps --without-fftw --without-fpx --without-djvu --without-fontconfig --without-freetype  --without-jbig  --without-lcms  --without-lqr --without-lzma --without-openexr --without-pango --without-png  --without-tiff --without-webp  --disable-openmp --disable-opencl  --enable-delegate-build  -with-modules  --with-jpeg=yes

#define CODER_PATH "/home/osboxes/SRC/Main/ImageMagick/_install/lib/ImageMagick-6.7.4/modules-Q16/coders/"

sed -i 's/#define CODER_PATH .*/#define CODER_PATH "\/system\/sdcard\/lib\/coders"/' config/config.h
sed -i 's/#define MAGICKCORE_CODER_PATH .*/#define MAGICKCORE_CODER_PATH "\/system\/sdcard\/lib\/coders"/'  magick/magick-config.h

sed -i 's/#define CODER_RELATIVE_PATH .*/#define CODER_RELATIVE_PATH "\/system\/sdcard\lib\/coders"/' config/config.h
sed -i 's/#define MAGICKCORE_CODER_RELATIVE_PATH .*/#define MAGICKCORE_CODER_RELATIVE_PATH "\/system\/sdcard\/lib\/coders"/'  magick/magick-config.h

sed -i 's/#define MAGICKCORE_CONFIGURE_PATH .*/#define MAGICKCORE_CONFIGURE_PATH "\/etc\/ImageMagick"/' magick/magick-config.h
sed -i 's/#define CONFIGURE_PATH .*/#define CONFIGURE_PATH "\/etc\/ImageMagick"/'  config/config.h

sed -i 's/#define MAGICKCORE_EXECUTABLE_PATH .*/#define MAGICKCORE_EXECUTABLE_PATH "\/system\/sdcard\/bin"/' magick/magick-config.h
sed -i 's/#define EXECUTABLE_PATH .*/#define EXECUTABLE_PATH "\/system\/sdcard\/bin"/' config/config.h

sed -i 's/#define FILTER_PATH .*/#define FILTER_PATH "\/system\/sdcard\/lib\/filters"/' config/config.h
sed -i 's/#define MAGICKCORE_FILTER_PATH .*/#define MAGICKCORE_FILTER_PATH "\/system\/sdcard\/lib\/filters"/'  magick/magick-config.h

sed -i 's/#define FILTER_RELATIVE_PATH .*/#define FILTER_RELATIVE_PATH "lib\/filters"/' config/config.h
sed -i 's/#define MAGICKCORE_RELATIVE_FILTER_PATH .*/#define MAGICKCORE_RELATIVE_FILTER_PATH "lib\/filters"/'  magick/magick-config.h

make -j4
make install
