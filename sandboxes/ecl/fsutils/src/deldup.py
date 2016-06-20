#!/usr/bin/env python
# encoding: utf-8
'''
fsutils.deldup -- Utility to help cleaning up the files archives/catalogs
It defines classes_and_methods

@author:     Serguei Koubli

@copyright:  2014 Kub Inc. All rights reserved.

@license:    "Copyright 2014 Kub (Kub Inc.)  \
Licensed under the Apache License 2.0\n    \
http://www.apache.org/licenses/LICENSE-2.0"

@contact:    skubliy@gmail.com
@deffield    updated: Updated
'''

#from CatalogsWalker import CatalogsWalker
#from CatalogsWalker import ConsoleIndicator
from LDict import LDict
#from OptionParserWraper import OptionParserWraper
from OptionParserWraper import err_report
from OutputSplitter import OutputSplitter
from ardiff import prefix
#import md5
import os
import sys
import time

__def_vers__ = "v0.1"
__def_duild_date__ = "2016-05-26"

DEBUG = 1

def read_duplicates(lines):
    pfd = LDict()
    curpath = ""
    for ln in lines[4:]:
        if ln[0] == 'p':
            curpath = ln[5:-1]
        elif ln[0] == 'f':
            for x in ln[6:-1].split("\t"):
                if len(x):
                    pfd[curpath].append(x)
            #    pfd[curpath].append(x[:x.rfind(' ')])
            pass
        pass
    return pfd


import shutil
def del_dir(d):
    #os.system("rm -rf dirname")
    print "del-dir:", d
    shutil.rmtree(d, ignore_errors=True)

def calc_tmp_path(d):
    todel = "/SomeReallyStrangPass"
    np = d.split('/')
    if len(np)>4:
        split_point = d.find(np[4])
        todel = d[:split_point] + "tmp_dir_to_del/" + d[split_point:]
    return todel
    
def mktmpdir(d):
    if not os.path.isdir(d):
        os.system("mkdir -p \"" + d + "\"")
        #print todel

import fnmatch
def deleter(pfd, lines):
    for ln in lines:
        if ln[0] == 'p' and ln[1] == 'a':
            lp = ln[5:-1]
            todel = calc_tmp_path(lp)
            mktmpdir(todel)
            #print lp
            for f in pfd[lp]:
                if len(f):
                    #print "from: ", os.path.join(lp, f)
                    #print "  to: ",  os.path.join(todel, f)
                    ffp=os.path.join(lp, f)
                    if os.path.isfile(ffp):
                        os.rename(ffp, os.path.join(todel, f))

            if os.path.isdir(lp):
                fllist = fnmatch.filter(os.listdir(lp), '*')
                sz = len(fllist)
                if sz == 0 :
                    del_dir(lp)
                elif sz == 1 :
                    if fllist[0].find("Thumbs.db") != -1:
                        del_dir(lp)
        pass
"""
def tester(pfd):
    VERBOS = 2
    cout = sys.stdout if VERBOS == 2 else None
    with OutputSplitter([cout, prefix.get_full("results/dup_ca_", __DUPLICATES_CALATOG_ONLY_SUFFIX__)]) as outs:
        if outs.pipes:
            pritners.print_catalogs_only("\n[DUPLICATES CATALOGS ONLY]\n",
                                        pfd,
                                        outs)
"""

def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    try:
        print argv
        pfd = read_duplicates(open(argv[0], "r").readlines())
        #print open(argv[1], "r").readlines()
        deleter(pfd, open(argv[1], "r").readlines())
        print "==="

    except Exception:
        err_report()
        return 2

if __name__ == "__main__":
    if DEBUG:
        print sys.argv
        if len(sys.argv) == 1:
            sys.argv.append("../results/dup--2016-5-28-17-35-51-sorted.txt")
            sys.argv.append("../results/dup--2016-5-28-17-35-51-catalogs_only.txt")
            pass

    sys.exit(main())