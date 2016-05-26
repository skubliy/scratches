#!/bin/bash
source set-env.sh

/etc/init.d/apache2 stop
killall shibd
#need if shibd start without -f
rm /opt/shibboleth-sp/var/run/shibboleth/shibd.sock
rm /tmp/markers.txt

/opt/shibboleth-sp/sbin/shibd -t

./rmlogs.sh

/opt/shibboleth-sp/sbin/shibd -f
/etc/init.d/apache2 start

