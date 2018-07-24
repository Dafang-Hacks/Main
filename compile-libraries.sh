./lame/compile.sh
./live/compile.sh
cd opus
./compile.sh
cd ..

TOOLCHAIN=$(pwd)/mips-gcc472-glibc216-64bit/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-

export INSTALL_DIR=${PWD}/_install
export CC=${CROSS_COMPILE}gcc
export CFLAGS="-muclibc -O3 -I$INSTALL_DIR/include"
export CXXFLAGS="-muclibc -O3 -I$INSTALL_DIR/include"
export LDFLAGS="-muclibc -O3"
export PKG_CONFIG_PATH="$INSTALL_DIR/lib/pkgconfig"


cd freetype2
./autogen.sh --host=mips-linux-gnu --prefix=$INSTALL_DIR --without-harfbuzz --without-png --without-zlib
make install
cd ..
