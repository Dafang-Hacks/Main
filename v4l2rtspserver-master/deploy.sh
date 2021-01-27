#!/usr/bin/env bash
HOST=10.0.0.17
if [ "$#" -ge 1 ]; then
    HOST=$1
fi

if [ $? == 0 ]; then
    echo Copying to ${HOST} v4l2rtspserver-master
    ftp-upload --passive -h ${HOST} -u root --password EarthaKitt -d /system/sdcard/bin/ _install/bin/*
 
    for lib in libBasicUsageEnvironment.so libUsageEnvironment.so libgroupsock.so libliveMedia.so
    do
        echo Copying to ${HOST} ${lib}
        ftp-upload --passive -h ${HOST} -u root --password EarthaKitt -d /system/sdcard/lib/ ../_install/lib/${lib}
    done
fi
