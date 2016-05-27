'''
Created on Dec 6, 2014

@author: Serguei Koubli
'''

from _collections import defaultdict

class LDict(defaultdict): # it's possible to use dict instead 
    
    """ Dictionary of lists - attempt to create a more predictable
        dictionary whose elements are lists """
    
    def __init__(self, *args, **kwds):
        __args=self.__check_args(*args)
        self.__check_kwds(**kwds)
        defaultdict.__init__(self,(lambda:[]), *__args, **kwds)
        # dict.__init__(self, *__args, **kwds)

    def __is_valid_item(self,item):
        if len(item)==2 and not hasattr(item[0],"__iter__") and hasattr(item[1],"__iter__"):
            return True
        return False
    
    def __check_args(self,*args):
        if len(args)>1:
            if type(args) == tuple and self.__is_valid_item(args):
                return [args,]  # tuple of key and list of values
            else:
                raise TypeError("Item of %s must be a tuple of (key,IterableValue) but %s=%s is not"%\
                                        (self.__class__.__name__, 
                                         args.__class__.__name__,
                                         repr(args)))
        if len(args) != 1: return args
        
        if isinstance(args[0], LDict): return args
        
        if hasattr(args[0],"__iter__"): 
            if len(args[0]) == 0: return args # empty iterator

            items = args[0].items() if type(args[0]) == dict else args[0]
            if self.__is_valid_item(items):
                return [args,]  # tuple of key and list of values
            for v in items:
                if not (type(v) == tuple and len(v)==2):
                    raise TypeError("Item of %s must be a tuple of (key, IterableValue) but %s=%s is not"%\
                                        (self.__class__.__name__, 
                                         v.__class__.__name__,
                                         v))   
                if not hasattr(v[1],"__iter__"):
                    raise TypeError("Value of %s must be iterable but %s(%s) is not"%\
                                        (self.__class__.__name__, 
                                         v[1].__class__.__name__,
                                         repr(v[1])))
                    
        else: raise TypeError(" %s must be initialized by {},[],() or %s but %s is not"%\
                                  (self.__class__.__name__, 
                                   self.__class__.__name__, 
                                   args[0].__class__.__name__))
        return args
    
    def __check_kwds(self, **kwds):
        for v in kwds.itervalues():
            if not hasattr(v,"__iter__"):
                        raise TypeError("Value of %s must be iterable but %s(%s) is not"%\
                                    (self.__class__.__name__, 
                                     v.__class__.__name__,
                                     repr(v)))

    def walk(self):
        for k, v in self.iteritems():
            for x in v:
                yield k, v, x
        raise StopIteration
    
    ''' in case we want to use dict instead of defaultdict
    def __missing__(self, key):
        x=[]
        self.__setitem__(key, x)
        return x
    '''
    def __setitem__(self, *args, **kwargs):
        self.__check_args(*args)
        self.__check_kwds(**kwargs)       
        return defaultdict.__setitem__(self, *args, **kwargs)
    
    def update(self, *args, **kwds):
        _args=self.__check_args(*args)
        self.__check_kwds(**kwds)
        defaultdict.update(self,*_args, **kwds)




if __name__ == '__main__': 
    import inspect
    correct = [ 
            {}, [], (),             # empty iterable
            {'k1':[]},              # 1 item dict
            {'k2':[], 'k22':[]},    # multipe items dict
            [('k3',[]),('k32',[])], # array tuples key list val
            (('k4',[]),('k42',[])), # tuple of tuples key list val
            ('k5',[])               # tuple of key list val
            ]

    strange = [
           ('e0','12'),
           ('e1','123'),
           {'e2':'12'},
           ['e3','12'],
           ['e4','123']
           ]
    def init_tester(dict_class,t_array,cs):
        
        print "\n%s %s %s"%(inspect.currentframe().f_code.co_name,
                            dict_class().__class__.__name__,
                            cs
                            )
        
        for i in t_array:
            try:
                print repr(i).ljust(26), repr(dict_class(i)).ljust(74),
                print ' work '.ljust(8)
            except Exception,e:
                print "dosn't work ",
                print e
                continue
            
        print "------------------------------"
    init_tester( LDict, correct, "correct")
    init_tester( dict, correct, "correct")
    init_tester( LDict, strange, "strange")
    init_tester( dict, strange, "strange")
    
    md=LDict()
    md['k0'].append("v0")
    md['k0'].append("v1")
    print md
    
    pass        
    for k,v in md.iteritems():
        if not isinstance(v,list):
            print "xxxx"
