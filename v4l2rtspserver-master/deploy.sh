#!/usr/bin/env bash
echo "NOTE: Be sure to turn off the RTSP server."

if [ "$#" -ge 1 ]; then
    HOST=$1
else
    echo -n 'Hostname or ip of your camera? '
    read HOST
fi

echo -n 'Password for your root user? (not echoed) '
read -s PASS

echo Copying to ${HOST} v4l2rtspserver-master
ftp-upload --passive -h ${HOST} -u root --password ${PASS} -d /system/sdcard/bin/ _install/bin/*

for lib in libBasicUsageEnvironment.so libUsageEnvironment.so libgroupsock.so libliveMedia.so
do
    echo Copying to ${HOST} ${lib}
    ftp-upload --passive -h ${HOST} -u root --password ${PASS} -d /system/sdcard/lib/ ../_install/lib/${lib}
done
