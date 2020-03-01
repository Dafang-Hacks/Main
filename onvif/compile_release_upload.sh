#!/usr/bin/env bash

# Created by Sian Croser

# use: ./compile_release_upload.sh {ip} {username} {password}
#
# Helper script to compile a release and upload all relevant files via ftp
# for the onvif service; This script assumes both ftp and http user and passwords
# are the same.

if [ -z "$1" ]; then
	echo "missing target ip"
	exit 1
fi

make release

# Stop service so that the binary can be overwritten
echo "Stopping Service"
../curl --request GET --silent "http://$2:$3@$1/cgi-bin/scripts.cgi?cmd=stop&script=onvif-srvd.sh" > /dev/null

# Give time for the service to be stopped before trying to upload
sleep 2

echo "Uploading Binary to $1"
../ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/bin/ onvif_srvd
echo "Uploading Boost Library"
../ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/lib/ ./libs/libboost_regex-mt-d-m32.so.1.72.0
echo "Uploading Controlscript"
../ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/controlscripts/ onvif-server
echo "Uploading Configs"
../ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/config/ ptz_presets.config.dist
echo "Restarting Onvif Service"
../curl --request GET --silent "http://$2:$3@$1/cgi-bin/scripts.cgi?cmd=start&script=onvif-srvd.sh" > /dev/null
echo "Finished..."
