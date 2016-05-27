#!/usr/bin/env python
# encoding: utf-8
'''
Created on Jul 1, 2015

statistics.blockio -- TODO: shortdesc

statistics.blockio TODO: is a description

It defines blockio

@author: Serguei Koubli

@copyright:  Serguei Koubli. All rights reserved.

@license:    TODO: license

@contact:    SergueiKoubli@gmail.com
@deffield    updated: Updated ?
'''

import sys
import os
#import pprint
        
def main(nm = "Memory/data"):
    argv = sys.argv[1:]
    nm = nm if (len(argv) == 0) else argv[0]
    cmd = "du  -s /var/lib/fsdb/ResourceServer/" + nm
    res=os.popen(cmd, 'r',1).readlines()[0].split()[0]
    nn = nm.replace("/","-")
    
    print "ResourceServer-" + nn + ' : \t' + res + " "


if __name__ == '__main__':
    sys.exit(main())