'''
Created on Jul 1, 2015

@author: Serguei Koubli
'''
import sys
import os
from datetime import datetime
import time

from optparse import OptionParser

DEBUG = 0

__period__=60
__verbose__=0
if DEBUG:
    __period__=5
    __verbose__=9

__all__ = []
__version__ = 0.1
__date__ = '1967-06-13'
__updated__ = '1967-06-13'


__def_config_file__="./def_statistics.conf"
__def_out_file__="./def_statistics#.log"
__def_desc_file__="./def_statistics#.dsc"


class statConfigurator(OptionParser):
    def __init__(self,argv):
        OptionParser.__init__(self)
        '''Command line options.'''
        if argv is None:
            argv = sys.argv[1:]
        self.version='%%prog v0.1 (%s)' % __updated__
        self.epilog='''''' # optional - give further explanation about what the program does
        __description="Copyright 2015 user_name (organization_name)                                            \
                Licensed under the Apache License 2.0\nhttp://www.apache.org/licenses/LICENSE-2.0"
        #program_usage = '''usage: spam two eggs''' # optional - will be autogenerated by optparse
        self.add_option("-c", "--conf", dest="fconf", help="set config file path [default: %default]", metavar="FILE")
        self.add_option("-d", "--dsc", dest="fdsc", help="set session descripton file path [default: %default]", metavar="FILE")
        self.add_option("-o", "--out", dest="fout", help="set output file path [default: %default]", metavar="FILE")
        self.add_option("-p", "--period", dest="period", help="set period of shecking statistics in seconds  [default: %default]")
        self.add_option("-v", "--verbose", dest="verbose", action="count", help="set verbosity level [default: %default]")

        if len(argv)==1 and not os.path.isfile(__def_config_file__):
            argv.append("-h")

        self.time_marker=datetime.now().strftime("%Y%m%d%H%M%S")[2:]

        # set defaults
        self.set_defaults(fconf=__def_config_file__,
                          fout=self.make_fpath(__def_out_file__),
                          fdsc=self.make_fpath(__def_desc_file__),
                          period=__period__,
                          verbose=__verbose__)
        # process options
        self.opts, self.args = self.parse_args(argv)

        if self.opts.verbose > 0:
            print("verbosity level = %d" % self.opts.verbose)
        if self.opts.fconf:
            print("fconf = %s" % self.opts.fconf)
        if self.opts.fdsc:
            print("fdsc = %s" % self.opts.fdsc)
        if self.opts.fout:
            print("fout = %s" % self.opts.fout)
        if self.opts.period:
            print("period = %s" % self.opts.period)

        self.options,self.jobs=self.doparse(open(self.opts.fconf).readlines())

    def make_fpath(self,fpath="",time_marker=None):
        if time_marker is None:
            time_marker=self.time_marker
        if fpath.find('#')<0:
            return fpath
        x=fpath.split('#')
        return x[0]+'-'+time_marker+x[1]



    def doparse(self,_lines):
        __jobs=[]
        __options={}
        for ln in _lines:
            x=' '.join(ln.split())
            if len(x)<1:
                continue
            if x[0] in ('#','\n','=',':'):
                continue
            n=x.split('#')
            if n[0].find(':'):
                kv=n[0].split(':')
                __jobs.append((kv[0],kv[1])) #.replace(' ','')
            elif n[0].find('='):
                kv=n[0].split('=')
                __options[kv[0]]=kv[1]
            else:
                #TODO print to err
                print "err: something wrong in this line: %s"%x
        return __options,__jobs
