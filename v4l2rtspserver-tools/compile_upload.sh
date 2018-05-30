./compile.sh
HOST=192.168.1.18
ftp-upload -h ${HOST} -u root --password ismart12 -d /system/sdcard/bin/ getimage
ftp-upload -h ${HOST} -u root --password ismart12 -d /system/sdcard/bin/ setconf
