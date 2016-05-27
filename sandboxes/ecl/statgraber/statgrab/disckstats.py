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

par_descr = "1 - major_number\
         2 - minor_number\
         3 - device_name\
         4 - reads_completed_successfully\
         5 - reads_merged\
         6 - sectors_read\
         7 - time_spent_reading\
         8 - writes_completed\
         9 - writes_merged\
        10 - sectors_written\
        11 - time_spent_writing\
        12 - IOs_currently_in progress\
        13 - time_spent_doing_IOs\
        14 - weighted_time_spent_doing_IOs\
        "
        
def main(dev_name='sda'):
    argv = sys.argv[1:]
    dev_name = dev_name if (len(argv) == 0) else argv[0]
    cmd = "cat /proc/diskstats | grep " + dev_name
    res=os.popen(cmd, 'r',1).readlines()[0].split()
    params = par_descr.split(' - ')[1:]
    idx = 0
    for param in params:
        print param.split()[0] + ':\t' + res[idx]
        idx+=1
        pass
    pass

if __name__ == '__main__':
    sys.exit(main())