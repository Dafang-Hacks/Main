./compile.sh
HOST=10.0.0.18
ftp-upload --passive -h ${HOST} -u root --password ismart12 -d /system/sdcard/bin/ getimage
ftp-upload --passive -h ${HOST} -u root --password ismart12 -d /system/sdcard/bin/ setconf
