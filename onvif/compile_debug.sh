#!/usr/bin/env bash

# Created by Sian Croser

# use: ./compile_debug.sh {ip} {username} {password}
#
# Helper script to compile a debug binary then upload to development device.
# This script is useful for development only. Ensure other required files are
# on the device first eg. ptz_presets.conf, boost regex library etc.
# the startup script will have no effect on this version of onvif server and should
# be ran via ssh

make debug

ftp-upload -h $1 --passive -u $2 --password $3 -d /system/sdcard/onvif/ onvif_srvd_debug
