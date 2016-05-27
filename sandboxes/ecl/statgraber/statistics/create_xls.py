#!/usr/bin/python
# encoding: utf-8

'''
Created on Jan 27, 2014

@author: Serguei Koubli
'''

import sys
import os
from text_corrector import split_to_lines

def get_header(argv=None):
    try:
        if argv is None:
            argv=sys.argv[1:]
        if len(argv)==0:
            raise NameError('Error: There is no input file')
        dsc_file_name=argv[0][:argv[0].rfind('.')+1]+'dsc'
        dsc=open(dsc_file_name).readlines()[2:]
        header=""
        for ln in dsc:
            if ln.find(':')>0:
                header+=ln.split(':')[0].replace('   \t','\t')
        return header+'\n'
    
    except Exception, e:
        program_name = os.path.basename(sys.argv[0])
        sys.stderr.write(program_name + ": " + repr(e) + "\n")
        return 2
    
def create_xls(argv=None):
    header=get_header(argv)
    data=split_to_lines(argv)
    return header,data
    
def main(argv=None):
    h,d=create_xls(argv=None)
    print h,d

if __name__ == "__main__":
    sys.exit(main())