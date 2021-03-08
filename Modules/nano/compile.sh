#!/usr/bin/env bash
. ../../setCompilePath.sh
export NANO_PATH="$(pwd)/nano"
export NCURSES_PATH="$(pwd)/ncurses"
export CFLAGS="${CFLAGS} -I$(pwd)/ncurses/include -I$(pwd)/ncurses/include/ncursesw -static"
export LDFLAGS="${LDFLAGS} -L$(pwd)/ncurses/lib"
export CPPFLAGS="${CPPFLAGS} -I$(pwd)/ncurses/include -I$(pwd)/ncurses/include/ncursesw"
NANO_VERSION=5.6.1
NCURSES_VERSION=5.9

##compile ncurses

if [ ! -d ncurses-$NCURSES_VERSION ]
then
	wget ftp://ftp.invisible-island.net/ncurses/ncurses-$NCURSES_VERSION.tar.gz
	tar xf ncurses-$NCURSES_VERSION.tar.gz
fi

cd ncurses-$NCURSES_VERSION
./configure --prefix=$NCURSES_PATH --host=mipsel-linux --target=mipsel-linux --without-manpages --without-normal --without-progs --without-debug --enable-widec --with-termlib --with-fallbacks=xterm-256color

make
make install
cd ..

ln -s $NCURSES_PATH/lib/libtinfow.a $NCURSES_PATH/lib/libtinfo.a

###compile nano

if [ ! -d nano-$NANO_VERSION ]
then
  wget https://www.nano-editor.org/dist/v5/nano-$NANO_VERSION.tar.gz
  tar xvf nano-$NANO_VERSION.tar.gz
fi

cd nano-$NANO_VERSION

./configure --prefix=$NANO_PATH --host=mipsel-linux --enable-utf8 --enable-nls --enable-color --enable-multibuffer --enable-nanorc

make
make install

cp ../nano/bin/nano ${INSTALLDIR}/bin/
