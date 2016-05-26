#!/bin/bash

source set-env.sh
#rm $THIRD_PARTY_DIR
#rm -rf $INSTALL_PREFIX
if ! [ -d $THIRD_PARTY_DIR ]; then
	echo 'There is no "'${INSTALL_PREFIX}'" will be created'
	sudo mkdir -p $INSTALL_PREFIX
fi


./get-3dp-lib.sh
./recompile-3dp-lib.sh

if ! [ -d $THIRD_PARTY_DIR ]; then
	echo 'There is no link"'${THIRD_PARTY_DIR}'" to "' ${INSTALL_PREFIX} '" will be created'
	sudo ln -s $INSTALL_PREFIX $THIRD_PARTY_DIR
fi
