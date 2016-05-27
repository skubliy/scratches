#!/usr/bin/env python
# encoding: utf-8
'''
fsutils.ardiff -- Utility to properly delete .bin config file for fv

Created on 2015-06-09

@author:     Serguei Koubli

@contact:    sergueikoubli@gmail.com
'''
import sys
from subprocess import Popen, PIPE
from string import split
#import os 

def run_cmd(rcmd):
    proc = Popen(
        rcmd,
        shell=True,
        stdout=PIPE, stderr=PIPE
    )
    proc.wait()
    res = proc.communicate()  # get tuple('stdout res', 'stderr res')
    if proc.returncode:
        print res[1]
    return res[0]

def main(argv=None):    
    for ln in run_cmd( "ps aux | grep %s"%sys.argv[1]).split('\n'):
        x = ln.split(' ')
        if len(x) > 1:
            run_cmd("kill -9 %s"%ln.split(' ')[1])
    pass


if __name__ == "__main__":
    if len(sys.argv) == 1:
        sys.argv.append("sl-")
    #print "zzz %s"%sys.argv    
    sys.exit(main())