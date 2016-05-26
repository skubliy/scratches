#!/bin/bash

source set-env.sh

##### function helpers
do_recompile(){
pushd $1
#autoreconf -i
#./configure --prefix=${INSTALL_PREFIX} --disable-static $2
#make clean
make
sudo make install
popd
}

##### recompile
pushd $OPEN_SRC_DIR
do_recompile shibboleth-sp-2.5.6 "--with-log4shib=${INSTALL_PREFIX} --enable-apache-24 --enable-debug"
popd
sudo cp  /home/dev/w/opnsrc/shibboleth-sp-2.5.6/apache/.libs/mod_shib_24.so* /usr/lib/apache2/modules/
sudo ./restart-all.sh


