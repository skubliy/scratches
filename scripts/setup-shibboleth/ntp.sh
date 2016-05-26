#!/bin/bash

# Version 0.1 – FJR – A script to Set up the date and time

service ntpd stop
ntpdate us.pool.ntp.org
hwclock --systohc

mv /etc/localtime /etc/localtime.bak
ln -s /usr/share/zoneinfo/America/Vancouver /etc/localtime

service ntpd start

