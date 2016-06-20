#!/usr/bin/env python
# encoding: utf-8
'''
fsutils.ardiff -- Utility to help cleaning up the files archives/catalogs

fsutils.ardiff is an utility to help cleaning up the files(photo,video, etc.)
archives/catalogs. For now it is do only look up for duplicates thru the
"main"/"raw" archives/catalogs. Also this utility can find unique files (files
which not in the "main"/"raw" archives) in external catalogs.

It defines classes_and_methods

@author:	 Serguei Koubli

@copyright:  2014 Kub Inc. All rights reserved.

@license:	"Copyright 2014 Kub (Kub Inc.)  \
Licensed under the Apache License 2.0\n	\
http://www.apache.org/licenses/LICENSE-2.0"

@contact:	skubliy@gmail.com
@deffield	updated: Updated
'''

#from __future__ import print_function
from CatalogsWalker import CatalogsWalker
from CatalogsWalker import ConsoleIndicator
from LDict import LDict
from OptionParserWraper import OptionParserWraper
from OptionParserWraper import err_report
from OutputSplitter import OutputSplitter
import md5
import os
import sys
import time


__all__ = []
__version__ = "v0.1"
__date__ = '2014-11-24'
__update__ = '2014-11-24'

__version_string__ = '%%prog %s (%s)' % (__version__, __update__)

DEBUG = 1

__DUPLICATES_CALATOG_ONLY_SUFFIX__ = "catalogs_only.txt"
__SORTED_SUFFIX__ = "sorted.txt"
__ASIS_SUFFIX__ = "asis.txt"

class FileInfo(object):
    """ Class to hold file info """

    def __init__(self, _path, _name=None):
        """ if _path only passed the _path == path+name """
        self.__size, self.__hexdigest = None, None
        if _name is None:
            self.__path = os.path.dirname(_path)
            self.__name = os.path.basename(_path)
        else:
            self.__path, self.__name = _path, _name

    @property
    def path(self):
        return self.__path

    @property
    def name(self):
        return self.__name

    @property
    def full_name(self):
        return os.path.join(self.path, self.name)

    @property
    def size(self):
        """ calculate if need and return size for file """

        if self.__size is None:
            self.__size = os.stat(self.full_name).st_size
        return self.__size

    @property
    def hexdigest(self):
        """ calculate if need and return md5 hexdigest for file """

        if self.__hexdigest is None:
            _md5 = md5.new()
            _file = open(self.full_name, 'rb')
            _block = _file.read(_md5.block_size)
            while _block:
                _md5.update(_block)
                _block = _file.read(_md5.block_size)
            self.__hexdigest = _md5.hexdigest()
        return self.__hexdigest

    @hexdigest.setter
    def hexdigest(self, _hxd):
        """ hexdigest setter if we want to use something specific """

        self.__hexdigest = _hxd
        return self.__hexdigest

    def __eq__(self, finfo):
        if finfo.size == self.size:
            if self.name == finfo.name:
                return True
            else:
                if self.hexdigest == finfo.hexdigest:
                    return True
        return False

    def __ne__(self, o):
        return not self.__eq__(o)

    def is_in(self, _flist):
        for finfo in _flist:
            if self.__eq__(finfo):
                return True
        return False

    #def __str__(self):
    #	return self.full_name

class DuplicatesChecker(object):
    """ duplicate files holder """

    def __init__(self):
        self.__dup_files = LDict() #where dict[FileInfo.hexdigest]=FileInfo

    @property
    def dup_files(self):
        return self.__dup_files

    def check(self, files, finfo):
        """ checks if the file's info already in archives """
        for i in files:
            if finfo.hexdigest == i.hexdigest:
                dupl = self.__dup_files[finfo.hexdigest]
                #print "\n++\t", len(dupl), "++", i.full_name, "\t", i.size
                if len(dupl) == 0: #First duplicate
                    dupl.append(i)
                    #print "\n==\t", i.full_name, "\t", i.size
                else:	#More then one duplicate
                    pass
                #Remembering all duplicates
                #print "\n--\t", finfo.full_name, "\t", finfo.size
                dupl.append(finfo)
            else: #Same size but different files
                pass

class UniqsChecker(object):
    """ uniqs files holder """

    @staticmethod
    def check(rfs, catalogs_walker):
        """ look up external catalogs for files which isn't in archives """

        uniq_files = LDict()

        for p, n in catalogs_walker:

            finfo = FileInfo(p, n)

            if finfo.size in rfs:
                if not finfo.is_in(rfs[finfo.size]):
                    if finfo.hexdigest not in uniq_files:
                        x = []; x.append(finfo)
                        uniq_files[finfo.hexdigest] = x

            elif finfo.hexdigest not in uniq_files:
                x = []; x.append(finfo)
                uniq_files[finfo.hexdigest] = x

        return uniq_files

class RawCatalogs(object):
    """Class to manipulate and hold list of raw(archive) catalogs - main storage """

    def __init__(self, catalogs_walker, duplicates_checker=None):
        """ """
        self.__raw_files = LDict()
        self._add_arhs(duplicates_checker, catalogs_walker)

    @property
    def raw_files(self):
        return self.__raw_files

    def _add_to_arh_dict(self, dup_checker, finfo):
        """ add file's information to archives """

        x = self.__raw_files[finfo.size]
        if x:
            if dup_checker:
                dup_checker.check(x, finfo)
        x.append(finfo)

    def _add_arhs(self, dup_checker, catalogs_walker):
        """ main loop thru the archives to create internal structures """

        for p, n in catalogs_walker:
            finfo = FileInfo(p, n)
            self._add_to_arh_dict(dup_checker, finfo)

class printers(object):

    @staticmethod
    def group_by_folders(d):
        pd = LDict()
        for _, _, x in d.walk():
            pd[x.path].append(x)
        return pd

    @staticmethod
    def print_as_is(n, d, outs):
        if not hasattr(outs, 'write'): return
        w = outs.write
        w(n)
        for i in d:
            w("\n")
            for x in d[i]:
                w(x.hexdigest + repr(x.size).rjust(12) + (x.full_name).rjust(100) + '\n')
        w("\n")

    @staticmethod
    def print_sorted_by_full_name(d, outs):
        if not hasattr(outs, 'write'): return
        for i in sorted(d.items(), key=lambda x: x[1][0].full_name):
            print '\n',
            for z in d[i]:
                print z.full_name, "\t", z.size, "\t", z.hexdigest
        print "\n"
        
    @staticmethod
    def print_catalogs_only(n, d, outs):
        if not hasattr(outs, 'write'): return
        w = outs.write
        w(n)
        for i in d:
            w('\npath=' + i)
        w("\n")
        
    @staticmethod
    def print_catalogs_and_stats(n, d, outs):
        if not hasattr(outs, 'write'): return
        w = outs.write
        w(n)
        for i in d:
            w('\npath=' + i)
            w('\n#\t' + str(len(os.listdir(i))) + "\t" + str(len(i)))
        w("\n")

    @staticmethod
    def print_sorted_by_catalogs_and_names(n, d, outs):
        if not hasattr(outs, 'write'): return
        w = outs.write
        w(n)
        for i in d:
            w('\n\npath=' + i + "\nfiles=")
            for z in sorted(d[i], key=lambda x: x.name):
                w(z.name + "\t")# + " " + str(z.size) + "\t")#,z.hexdigest
        w("\n")

class prefix():

    @staticmethod
    def time_stamp():
        gmt = time.gmtime()
        return '-' + str(gmt.tm_year) + '-' + str(gmt.tm_mon) + '-' + str(gmt.tm_mday) + \
            '-' + str(gmt.tm_hour) + '-' + str(gmt.tm_min) + '-' + str(gmt.tm_sec) + '-'

    @staticmethod
    def get(r, p):
        return (os.path.join(r, p + prefix.time_stamp()) if r else p + prefix.time_stamp()) \
            if p else None

    @staticmethod
    def get_full(prefix, suffix):
        return prefix + suffix if prefix else None

def main_scenario(opts, args):
    """ Main idea to have 2 copy of archive foto catalogs
    one raw - what is mean raw catalogs copied from camera, phones, etc.
    second raw-sorted - the same files but grouped by date and/or topics
    This script will help  to compare this 2 lists of catalogs
    and will help to find:
    - duplicates files in raw catalogs
    - fines which are in raw-sorted catalogs but not in raw catalogs
    Also it is possible to do backward ardiff using raw-sorted as first
    raw as second """

    dup_checker = DuplicatesChecker() if opts.check_dup == 'Y' else None

    console_indicator = None
    if opts.verbose != 0:
        cout = sys.stdout if opts.verbose == 1 else sys.stderr
        with OutputSplitter(cout) as outs:
            console_indicator = ConsoleIndicator(outs)

    rroot, rdirs = opts.raw_root, opts.raw.split(',') #list of folders separated by ','
    arch_dirs = [os.path.join(rroot, x) for x in rdirs] if opts.raw_root else rdirs

    rctlgs = RawCatalogs(CatalogsWalker(arch_dirs, console_indicator), dup_checker)
    cout = sys.stdout if opts.verbose == 2 else None
    if dup_checker is not None:

        fout_prefix = prefix.get(opts.out_root, opts.dup_prefix)
        if fout_prefix or cout:
            grouped_by_folder = printers.group_by_folders(dup_checker.dup_files)
            with OutputSplitter([cout, prefix.get_full(fout_prefix, __DUPLICATES_CALATOG_ONLY_SUFFIX__)]) as outs:
                if outs.pipes:
                    #printers.print_catalogs_only("\n[DUPLICATES CATALOGS ONLY]\n",
                    printers.print_catalogs_and_stats("\n[DUPLICATES CATALOGS ONLY]\n",
                                                               grouped_by_folder,
                                                               outs)

            with OutputSplitter([cout, prefix.get_full(fout_prefix, __SORTED_SUFFIX__)]) as outs:
                if outs.pipes:
                    printers.print_sorted_by_catalogs_and_names("\n[SORTED DUPLICATES]\n",
                                                               grouped_by_folder,
                                                               outs)

            with OutputSplitter([cout, prefix.get_full(fout_prefix, __ASIS_SUFFIX__)]) as outs:
                if outs.pipes:
                    printers.print_as_is("\n[DUPLICATES]\n",
                                        dup_checker.dup_files,
                                        outs)

    if opts.check_uniq == "Y":

        fout_prefix = prefix.get(opts.out_root, opts.uniq_prefix)
        if fout_prefix or cout:
            with OutputSplitter([cout, prefix.get_full(fout_prefix, __SORTED_SUFFIX__)]) as outs:
                if outs.pipes:
                    _catalogs = opts.raw_sorted.split(',')
                    _catalogs = [os.path.join(opts.raw_sorted_root, x) for x in _catalogs] \
                        if opts.raw_sorted_root else _catalogs	
                    uniq_files = UniqsChecker.check(rctlgs.raw_files, \
                                                    CatalogsWalker(_catalogs, console_indicator))

                    printers.print_sorted_by_catalogs_and_names("\n[UNIQS]\n",
                                                               printers.group_by_folders(uniq_files),
                                                               outs)

def main(argv=None):

    try:
        opt_parser = OptionParserWraper(argv)#, version_string = __version_string__)

        main_scenario(opt_parser.opts, opt_parser.args)

    except Exception:
        err_report()
        return 2

if __name__ == "__main__":
    if DEBUG:
        print sys.argv
        if len(sys.argv) == 2 and sys.argv[1] == 'null':
            sys.argv[1] = "--config=test.conf"

        if len(sys.argv) == 1:
            sys.argv.append("--config=test.conf")
            pass

    sys.exit(main())

