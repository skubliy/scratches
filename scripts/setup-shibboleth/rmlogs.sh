#!/bin/bash
source set-env.sh
echo ${INSTALL_PREFIX}
rm -rf ${INSTALL_PREFIX}/var/log/shibboleth/*.log
rm -rf ${INSTALL_PREFIX}/var/log/shibboleth-www/*.log
#rm -rf /var/log/apache2/*.log
rm -rf ${INSTALL_PREFIX}/var/log/apache2/*.log*


