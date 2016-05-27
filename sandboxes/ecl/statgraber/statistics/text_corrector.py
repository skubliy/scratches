#!/usr/bin/python
# encoding: utf-8

'''
Created on Jan 22, 2014

@author: Serguei Koubli
'''

import sys
import os


def split_to_lines(argv=None):
    try:
        if argv is None:
            argv=sys.argv[1:]
        if len(argv)==0:
            raise NameError('Error: There is no input file')
        data=open(argv[0]).read()
        if data.count('\n')>1:
            return data
        elif data.count('\t\t')>1:
            data=data.replace('\t\t','\t\n')
            return data
        else:
            splited=data.split('.')
            prev=splited[0]
            data=''
            for l in splited[1:-1]:
                data+=prev+'.'+l[:l.rfind('\t')]+'\n'
                prev=l[l.rfind('\t')+1:]
                #sys.stderr.write("%s"%prev)

            data+=prev+'.'+splited[-1]+'\n'
            return data
    except Exception, e:
        program_name = os.path.basename(sys.argv[0])
        sys.stderr.write(program_name + ": " + repr(e) + "\n")

        #indent = len(program_name) * " "
        #sys.stderr.write(indent + "  for help use --help\n")
        return 2
    
def main(argv=None):
    print split_to_lines(None)

if __name__ == "__main__":
    sys.exit(main())