#!/bin/bash

source set-env.sh

##### function helpers
do_recompile(){
pushd $1
autoreconf -i
./configure --prefix=${INSTALL_PREFIX} --disable-static $2
make clean
make
sudo make install
popd
}

##### recompile
pushd $OPEN_SRC_DIR
do_recompile cpp-log4shib " --disable-doxygen" 
do_recompile  xerces-c-3.1.3 " --enable-netaccessor-socket"
do_recompile xml-security-c-1.7.3 " --with-xerces=${INSTALL_PREFIX} --with-openssl=${OPEN_SSL_PREFIX} --without-xalan"  
do_recompile cpp-xmltooling " --with-log4shib=${INSTALL_PREFIX} --with-libcurl=${OPEN_SSL_PREFIX} -C"
do_recompile cpp-opensaml " --with-log4shib=${INSTALL_PREFIX} --enable-debug -C"
do_recompile shibboleth-sp-2.5.5 "--with-log4shib=${INSTALL_PREFIX} --enable-apache-24 --enable-debug"
popd



