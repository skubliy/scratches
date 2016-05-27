#!/usr/bin/python
# encoding: utf-8
'''
Created on Feb 14, 2014

@author: skub
'''
import sys
import os
import ast

exept=['flush',]

def get_level(path):
    for folder, subs, files in os.walk(path):
        print folder
        #print subs
        for f in files:
            #print "--- s =",f
            if f not in exept: 
                print '\t',f,'=',open(folder+'/'+f).read(),
        print '-----------------'
        for s in subs:
            get_level(folder+'/'+s)

def main(argv=None):
    try:
        if argv is None:
            argv=sys.argv[1:]
        if len(argv)==0:
            raise NameError('Error: There is no path in argv ')
        
        get_level(argv[0])
            
  
    except Exception, e:
        program_name = os.path.basename(sys.argv[0])
        sys.stderr.write(program_name + ": " + repr(e) + "\n")

        return 2

if __name__ == "__main__":
    sys.exit(main())