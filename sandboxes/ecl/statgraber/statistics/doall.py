#!/usr/bin/env python
# encoding: utf-8

'''
Created on Jan 27, 2014

@author: Serguei Koubli
'''

import sys
import os

    
def main(argv=None):
    lns=os.popen('create_xls.py ','r',1).readlines()
    h,d=create_xls(argv=None)
    print h,d

if __name__ == "__main__":
    sys.exit(main())