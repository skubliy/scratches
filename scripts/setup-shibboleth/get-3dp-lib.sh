#!/bin/bash

source set-env.sh

##### function helpers

do_wget(){ #function wraper/helper for wget
path=$1
fn=${path##*/}
if ! [ -f $fn ]; then
	echo 'There is no file "'${fn}'" will be downloaded'
	wget $path
fi
}


do_clone(){ #function wraper/helper for git clone
path=$1
fn=${path##*/}
dn=(${fn//.git/ })
echo $dn
if ! [ -d $dn ]; then
	echo 'There is no "'${dn}'" will be cloned'
	git clone $path
fi
}

do_extract(){ #function wraper/helper for tar
path=$1
fn=${path##*/}
dn=(${fn//.bz2/ })
#dn=${arr[0]}
if ! [ -d $dn ]; then
	echo 'There is no "'${dn}'" will be extracted'
	tar xvjf $path
fi
}

##### downloadind tars
if ! [ -d $LOCAL_REPO ]; then
	echo 'There is no directory "'$(LOCAL_REPO)'" will be created'
	mkdir -p $LOCAL_REPO
fi

pushd $LOCAL_REPO
do_wget "http://www.apache.org/dist/xerces/c/3/sources/xerces-c-3.1.3.tar.bz2"
do_wget "http://apachemirror.ovidiudan.com/santuario/c-library/xml-security-c-1.7.3.tar.bz2"
#do_wget "http://shibboleth.net/downloads/c++-opensaml/2.5.5/opensaml-2.5.5.tar.bz2"
#do_wget "http://shibboleth.net/downloads/c++-opensaml/2.5.5/xmltooling-1.5.5.tar.bz2"
popd

##### create sources
if ! [ -d $OPEN_SRC_DIR ]; then
	echo 'There is no directory "'${OPEN_SRC_DIR}'" will be created'
	mkdir -p $OPEN_SRC_DIR
fi

pushd $OPEN_SRC_DIR
do_extract "$LOCAL_REPO/xml-security-c-1.7.3.tar.bz2"
do_extract "$LOCAL_REPO/xerces-c-3.1.3.tar.bz2" 
#do_extract "$LOCAL_REPO/opensaml-2.5.5.tar.bz2"
#do_extract "$LOCAL_REPO/xmltooling-1.5.5.tar.bz2"

do_clone git://git.shibboleth.net/cpp-log4shib.git
do_clone git://git.shibboleth.net/cpp-xmltooling.git
do_clone git://git.shibboleth.net/cpp-opensaml.git
popd

