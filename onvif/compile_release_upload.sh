#!/usr/bin/env bash
# @Author: Sian Croser
# @Date:   2020-03-03T11:37:52+10:30
# @Email:  CQoute@gmail.com
# @Filename: compile_release_upload.sh
# @Last modified by:   Sian Croser
# @Last modified time: 2020-03-05T16:20:28+10:30
# @License: GPL-3

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
curl --request GET --silent "http://$2:$3@$1/cgi-bin/scripts.cgi?cmd=stop&script=onvif-server" > /dev/null

# Give time for the service to be stopped before trying to upload
sleep 3

echo "Uploading Binary to $1"
ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/bin/ onvif_srvd
echo "Uploading Controlscript"
ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/controlscripts/ onvif-server
echo "Uploading Configs"
ftp-upload -h "$1" --passive -u $2 --password $3 -d /system/sdcard/config/ ptz_presets.conf.dist
echo "Restarting Onvif Service"
curl --request GET --silent "http://$2:$3@$1/cgi-bin/scripts.cgi?cmd=start&script=onvif-server" > /dev/null
echo "Finished..."
