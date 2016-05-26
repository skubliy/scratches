#!/bin/bash

apt-get install nfs-kernel-server nfs-common&&echo "loaded"

echo "/ 192.168.xxx.0/24(rw,fsid=0,insecure,all_squash,no_subtree_check,async,anonuid=1000,anongid=1000)" >> /etc/exports
echo "/ 192.168.xxx.0/24(rw,fsid=0,insecure,all_squash,no_subtree_check,async,anonuid=1000,anongid=1000)" >> /etc/exports
echo "/ 192.168.xxx.0/24(rw,fsid=0,insecure,all_squash,no_subtree_check,async,anonuid=1000,anongid=1000)" >> /etc/exports

/etc/init.d/nfs-kernel-server start
