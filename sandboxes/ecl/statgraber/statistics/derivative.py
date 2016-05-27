#!/usr/bin/python
# encoding: utf-8
'''
Created on Jan 27, 2014

@author: skub
'''

import sys
import os
import ast
from text_corrector import split_to_lines
from create_xls import get_header

#def _num (s):
#    try:
#        return int(s)
#    except Exception.ValueError:
#        return float(s)
    
def num (s):
    return ast.literal_eval(s)

def get_period(argv=None):
    try:
        if argv is None:
            argv=sys.argv[1:]
        if len(argv)==0:
            raise NameError('Error: There is no input file')
        dsc_file_name=argv[0][:argv[0].rfind('.')+1]+'dsc'
        return open(dsc_file_name).readlines()[1].split('=')[1]
    
    except Exception, e:
        program_name = os.path.basename(sys.argv[0])
        sys.stderr.write(program_name + ": " + repr(e) + "\n")
        return 2

def get_derivative(lns,d):
    prev=[]
    d=num(d)
    lines=lns.split('\n')
    for x in lines[0].split('\t')[:-1]:
        prev.append(num(x))
    for ln in lines[1:]:
        cur=ln.split('\t')
        for i in xrange(len(cur)-1):
            cx=num(cur[i])
            print (cx-prev[i])/d,'\t',
            prev[i]=cx
        print ''

xx="11.1\t90\t15.789\n12.0\t93\t15.9\n12.8\t98\t16.9\t\n"
    
def main(argv=None):
    print get_header(argv).replace('\t','_d\t'),
    
    get_derivative(split_to_lines(argv),get_period(argv))

if __name__ == "__main__":
    sys.exit(main())
