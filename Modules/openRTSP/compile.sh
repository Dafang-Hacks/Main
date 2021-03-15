#!/usr/bin/env bash
. ../../setCompilePath.sh
export BUILD_PATH="$(pwd)/build"
export CFLAGS="${CFLAGS} -I./include -I../UsageEnvironment/include -I../groupsock/include"
export CPPFLAGS="${CPPFLAGS} -I./include -I../UsageEnvironment/include -I../groupsock/include"
export CXXFLAGS="${CXXFLAGS} -I-/include -I../UsageEnvironment/include -I../groupsock/include"

if [ ! -d live ]
then
	wget http://www.live555.com/liveMedia/public/live555-latest.tar.gz
	tar xf live555-latest.tar.gz
fi
cp config.linux-mips live/
cd live
#grep -rl 'PREFIX = /usr/local' ./ | xargs sed -i '' -e "s,PREFIX = /usr/local,PREFIX = $BUILD_PATH,g"

./genMakefiles linux-mips

make clean
make -j 4

#make install
