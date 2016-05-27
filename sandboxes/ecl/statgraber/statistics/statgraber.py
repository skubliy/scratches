#!/usr/bin/env python
# encoding: utf-8
'''
Created on Jul 1, 2015

statistics.statgraber -- TODO: shortdesc

statistics.statgraber TODO: is a description

It defines statgraber

@author: Serguei Koubli

@copyright:  Serguei Koubli. All rights reserved.

@license:    TODO: license

@contact:    SergueiKoubli@gmail.com
@deffield    updated: Updated ?
'''

import sys
import os
import pprint
from datetime import datetime
import time



TESTRUN = 0
PROFILE = 0

from configurator import statConfigurator

class statGraber(statConfigurator):
    def __init__(self,argv):
        statConfigurator.__init__(self,argv)

    def run_forever(self):
        fd=open(self.opts.fdsc, 'a')
        fd.write(self.make_desc())
        fd.close()
        while(True):
            fd=open(self.opts.fout, 'a')
            fd.write(self.do_jobs())
            fd.close()
            time.sleep(self.opts.period)

    def get_jobs(self,jobs=None):
        _jobs=[]
        if jobs is None:
            jobs=self.jobs
        for (cmd,args) in jobs:
            #print cmd
            _jobs.append((cmd,args))
        return _jobs
        
    def do_jobs(self,jobs=None):
        #result=datetime.now().strftime("%Y%m%d%I%M%S")[2:]+"\t"+repr(time.time())+"\t"
        result=repr(time.time())+"\t"
        _jobs=self.get_jobs(jobs)
        if len(_jobs)==0:
            print "There is nothing to do((\nCheck the jobs list in .conf file\n"
            return result
        for cmd,args in _jobs:
            __args=args.split(',')

            _saveall=True if "ALL" in args else False
            lns=os.popen(cmd,'r',1).readlines()
            #print lns
            for ln in lns:
                x=ln.split(':')
                key=' '.join(x[0].split())
                if _saveall or (key in __args):
                    v=' '.join(x[1].split()).split(' ')[0]
                    if v.isdigit():
                        v+='\t'
                    else:
                        v='0\t'
                    #print v
                    result+=v
        return result+'\n'



    def make_desc(self):
        result="date_and_time="+datetime.now().strftime("%Y.%m.%d %H:%M:%S")+"\n"
        result+="period=%s\n"%self.opts.period
        result+="cur_time\t:\t"+repr(time.time())+"\n"

        _jobs=self.get_jobs()
        if len(_jobs)==0:
            result+="There is nothing to do((\nCheck the jobs list in .conf file\n"
            return result
        for cmd,args in _jobs:
            result+='['+cmd+']\n'
            lns=os.popen(cmd,'r',1).readlines()
            __args=args.split(',')
            _saveall=True if "ALL" in args else False
            #print cmd, "\t", lns
            for ln in lns:
                x=ln.split(':')
                key=' '.join(x[0].split())
                result+=key+'   \t:\t'
                if _saveall or (key in __args):
                    result+=' '.join(x[1].split()).split(' ')[0]+'\n'
        return result


def main(argv=None):
    try:
        # configurate
        statGraber(argv).run_forever()

            #print "The End : %s" % time.ctime()

    except Exception, e:
        program_name = os.path.basename(sys.argv[0])
        indent = len(program_name) * " "
        sys.stderr.write(program_name + ": " + repr(e) + "\n")
        sys.stderr.write(indent + "  for help use --help")
        return 2


if __name__ == "__main__":

    if TESTRUN:
        import doctest
        doctest.testmod()
    if PROFILE:
        import cProfile
        import pstats
        profile_filename = 'statistics.statgraber_profile.txt'
        cProfile.run('main()', profile_filename)
        statsfile = open("profile_stats.txt", "wb")
        p = pstats.Stats(profile_filename, stream=statsfile)
        stats = p.strip_dirs().sort_stats('cumulative')
        stats.print_stats()
        statsfile.close()
        sys.exit(0)
    sys.exit(main())
