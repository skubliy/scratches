#!/usr/bin/env python
# encoding: utf-8
'''
Created on Dec 6, 2014

@author: Serguei Koubli
'''

import sys
import os

#import pprint
from OutputSplitter import OutputSplitter
from optparse import OptionParser


__def_vers__ = "v0.1"
__def_duild_date__ = "2014-11-24"

def err_report():
    
    last_type, last_value, last_traceback = sys.exc_info()
    
    tb, descript = last_traceback, []
    
    while tb :
        fname, lno = tb.tb_frame.f_code.co_filename, tb.tb_lineno
        descript.append('\tfile "%s", line %s, in %s\n'%(fname, lno, tb.tb_frame.f_code.co_name))
        tb = tb.tb_next
        
    descript.append('%s : %s\n'%(last_type.__name__, last_value))
    
    for i in descript : 
        sys.stderr.write(i),

class Error(Exception):
    """Base class for ardiff exceptions."""

    def __init__(self, msg=None):
        self.msg = msg

    def __str__(self):
        return self.msg
    
    #def __repr__(self, *args, **kwargs):
    #    return Exception.__repr__(self, *args, **kwargs)
    
class Vars(object):
    """
    extension for os.path to normalize and expand vars
    """
    
    import re
    varprog = re.compile(r'\$(\w+|\{[^}]*\})') 
    
    #def __init__(self):
    #    super(Vars, self).__init__(os.path)
        #self._path=self._normalize(path, env)
      
    @staticmethod  
    def normalize(_path):
        """ normalizing path in view of os.environ vars """ 

        return Vars._normalize(_path, os.environ)

    @staticmethod
    def _normalize(_path, _env):
        while '$' in _path:
            _path = Vars.expandvars(_path,_env)
        _path = os.path.expanduser(os.path.normpath(_path))
        #print _path
        return _path
    
    @staticmethod
    def expandvars(path, _env):
        """Expand shell variables of form $var and ${var}.  Unknown variables
        are left unchanged."""
        #cllps = (lambda s: ' '.join(s.split()))
        path=' '.join(path.split())
        if '$' not in path:
            return path

        i = 0
        while True:
            m = Vars.varprog.search(path, i)
            if not m:
                break
            i, j = m.span(0)
            name = m.group(1)
            if name.startswith('{') and name.endswith('}'):
                name = name[1:-1]
            if name in _env:
                tail = path[j:]
                path = path[:i] + _env[name]
                i = len(path)
                path += tail
            else:
                i = j
        return path
    
class ConfigNotFoundError(Error):
    """Raised if config file passed but not found."""

    def __str__(self):
        return "call expression when config file passed but not found"

class OptionParserWraper(OptionParser):
    def __init__(self,argv,
                 version_string = '%%prog %s (%s)' % (__def_vers__, __def_duild_date__),
                 longdesc = '''''', # optional - give further explanation about what the program does
                 license_ = "Copyright 2014 Gurman (Gurman Inc.)                                            \
                    Licensed under the Apache License 2.0\nhttp://www.apache.org/licenses/LICENSE-2.0"                 
                 ):
        '''Command line options.'''
        # setup option parser
        OptionParser.__init__(self,
                              version=version_string, 
                              epilog=longdesc, 
                              description=license_)
        
        x=self.add_option
        x('--config', dest='config', default=None, metavar='FILE', 
          help='config file')
        x('--raw_root', dest='raw_root', default=None, metavar='PATH', 
          help='prefix(root path) for raw catalogs')        
        x('--raw', dest='raw', default='raw', metavar='DIRS', 
          help='list of raw catalogs names - archives names')
        x('--raw_sorted_root', dest='raw_sorted_root',default= None, metavar='PATH', 
          help='prefix(root path) for sorted raw catalogs')      
        x('--raw_sorted', dest='raw_sorted', default='raw_sorted', metavar='DIRS', 
          help='list of sorted raw catalogs names')
        x('--verbose',  dest='verbose', type=int, default=2, metavar='int', 
          help='level of verbosity')
        x('--check_dup', dest='check_dup', default='Y', metavar='Y/n', 
          help='Y for check duplicates default[Y]')
        x('--out_root', dest='out_root', default=None, metavar='FILE', 
          help='root dir to results output')
        x('--dup_prefix', dest='dup_prefix', default=None, metavar='FILE', 
          help='filenames prefix to out list of duplicate files')
        x('--check_uniq', dest='check_uniq', default='n', metavar='Y/n', 
          help='Y for check uniq files default[n]')
        x('--uniq_prefix', dest='uniq_prefix', default=None, metavar='FILE', 
          help='filenames prefix to out list of uniq files')
            #TODO ('snapshot',        'snapshot.txt', 'FILE', 'filename where to save current state of the raw catalogs'),
            #TODO ('use_snapshot',    'snapshot.txt', 'FILE','filename to use to create of the raw catalogs')
                       
        def config_parser_and_options_merger(filename, argv=None):

            norm = Vars.normalize
            dopts = {}
        
            fconf = open(filename, 'r')
            for ln in fconf.read().split('\n'):
                #skip all comments and bad lines 
                if ln and ln[0] ==' #' or len(ln) < 3 or '=' not in ln:
                    continue
                
                kv = ln.split("=")
                #normalize all parameters form file
                dopts[ norm( kv[0] ) ] = norm( kv[1] )
            
            for a in argv:    
                kv=a.split("=")
                #normalize all parameters in argv
                dopts[ norm( kv[0] ) ] = norm( kv[1] )
                
            finopt = []
            for k in dopts:
                finopt.append( k + '=' + dopts[k] )
                
            return finopt
            
        if argv is None:
            argv = sys.argv[1:]
                
        # process options
        (opts, args) = self.parse_args(argv)
                
        if opts.config:
            print opts.config
            if os.path.isfile(opts.config):
                (opts,args) = self.parse_args( config_parser_and_options_merger(opts.config,argv) )
            else:
                raise ConfigNotFoundError()
        
        if opts.verbose > 0:
            opt_parser_out = OutputSplitter([sys.stdout]) if opts.verbose == 1  else OutputSplitter([sys.stderr])
                 
            opt_parser_out.write("\nUsed options:\n")
            for o,v in sorted(opts.__dict__.items()):
                opt_parser_out.write("\t" + o + "=" + repr(v) +"\n")
            opt_parser_out.write("--------------------------------\n\n")
            opt_parser_out.flush()
        
        self.__opts, self.__args = opts, args
            
    @property        
    def opts(self):
        return self.__opts
    
    @property
    def args(self):
        return self.__args

if __name__ == "__main__":
    def test():
        """ for test OptionParserWraper itself """
        try:
            print "WARNING: It only test. The real utilitt ardiff.py"

            _ = OptionParserWraper( sys.argv[1:] )#, version_string = __version_string__)
        
        except Exception:
            err_report()
            return 2
    
    sys.exit(test())
