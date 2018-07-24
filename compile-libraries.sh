./lame/compile.sh
./live/compile.sh
cd opus
./compile.sh
cd ..

cd freetype2
TOOLCHAIN=$(pwd)/../toolchain/bin
CROSS_COMPILE=$TOOLCHAIN/mips-linux-gnu-
make clean
export CC=${CROSS_COMPILE}gcc
export CFLAGS="-muclibc -O3"
export CPPFLAGS="-muclibc -O3"
export LDFLAGS="-muclibc -O3"

./autogen.sh
./configure --host=mips-linux-gnu --without-harfbuzz --without-png --without-zlib --prefix=$(pwd)/_install
make install
cd ..
