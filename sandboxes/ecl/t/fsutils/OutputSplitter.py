#!/usr/bin/env python
# encoding: utf-8
'''
Created on Dec 6, 2014

@author: Serguei Koubli
'''

import os
import sys

from sets import Set

class OutputSplitter(object):
    """ Output streams splitter """
    
    def __init__(self, out_splitter):
        self.__pipes=None
        self.__files_to_close=[]
        if isinstance(out_splitter, OutputSplitter):
                self.__pipes = Set(out_splitter.pipes)
        elif out_splitter:
            if type(out_splitter) == list: 
                for pn in out_splitter:
                    self.add_pipe(pn)
            else:
                self.add_pipe(out_splitter)
                   
    @property
    def pipes(self):
        return self.__pipes
    
    def add_pipe(self, pn):
        if isinstance(pn, basestring):
            if pn == 'stderr': x = sys.stderr
            elif pn == 'stdout': x = sys.stdout
            else: 
                p = os.path.dirname(pn)
                if p and not os.path.exists(p):
                    os.makedirs(p)
                try: 
                    x = open(pn,'w+')
                    self.__files_to_close.append(x)
                except IOError:
                    raise
                    
        else: x = pn
        
        if hasattr(x, 'write') and hasattr(x, 'flush'):
            if self.__pipes is None:
                self.__pipes=Set()
            self.__pipes.add(x)
    
    def write(self,s):
        if self.__pipes:
            for p in self.__pipes:
                p.write(s)
    
    def flush(self):
        if self.__pipes:
            for p in self.__pipes:
                p.flush()
    
    def __enter__(self): return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        for f in self.__files_to_close: f.close()

if __name__ == '__main__':
    with OutputSplitter("stdout") as o:
        o.write("test")
        o.flash()
    pass