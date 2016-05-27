'''
Created on Dec 6, 2014

@author: Serguei Koubli
'''

from os import walk
from time import time

from abc import ABCMeta, abstractmethod

class Reporter(object):
    __metaclass__=ABCMeta
    
    @abstractmethod
    def report(self, catalogs_cntr, files_ctnr, speed):
        """ output catalogs_cntr, files_ctnr, speed to somewhere """
        #pass
    
class ConsoleIndicator(Reporter):
    def __init__(self, out_splitter):
        self.__out_splitter=out_splitter
        
    def report(self, catalogs_cntr, files_ctnr, speed):
        if self.__out_splitter and self.__out_splitter.pipes:
            result = []
            result.append("\rcataloges:")
            result.append(repr(catalogs_cntr).rjust(8)) 
            result.append("\tfiles:")
            result.append(repr(files_ctnr).rjust(13))  
            result.append("\tspeed: %12.3f"%speed)
            result.append(" files/s")
            
            #result.append("catalogs:{!r:>8}".format(catalogs_cntr))
            #result.append("files:{:!r>13}".format(files_ctnr)) 
            #result.append("speed: {:12.3f} files/s".format(speed))
            result = '\r'+' '.join(result)
            
            self.__out_splitter.write(result)
            self.__out_splitter.flush()        
                
class CatalogsWalker(object):
    """ Class to walk thru a tree of catalogs  """
    
    def __init__(self, catalogs, reporter=None):
        """ """
        
        self.__files_cntr, self.__catalogs_cntr = 0, 0
        self.__start_time, self.__speed = 0, 0
        self.__reporter = reporter 
        self.__catalogs = catalogs if hasattr(catalogs, "__iter__") else [catalogs]
        self._walker = self._walk()
 
    def __iter__(self):
        return self
        
    def next(self):
        return next(self._walker)
        
    def _walk(self):
        self.__files_cntr, self.__catalogs_cntr = 0, 0 
        self.__start_time, self.__speed = 0, 0
        self.__start_time=time()
        for catalog in self.__catalogs:
            for p, _, ns in walk(catalog): 
                self.__catalogs_cntr+=1
                for n in ns:
                    self.__files_cntr+=1
                    self.__speed=self.__files_cntr/(time()-self.__start_time)
                    if self.__reporter:
                        self.__reporter.report(self.__catalogs_cntr, 
                                               self.__files_cntr, 
                                               self.__speed)
                    yield p,n
                    
    @property
    def reporter(self):
        return self.__reporter
    
    @property
    def catalog_counter(self):
        return self.__catalogs_cntr    
    
    @property
    def file_counter(self):
        return self.__files_cntr
    
    @property
    def start_time(self):
        return self.__start_time
    
    @property
    def speed(self):
        return self.__speed    
   


if __name__ == '__main__':
    from OutputSplitter import OutputSplitter
    cw=CatalogsWalker("./",ConsoleIndicator(OutputSplitter("stdout")))
    for p, n in cw:
        print p,n
        pass
    print 'the end'
    