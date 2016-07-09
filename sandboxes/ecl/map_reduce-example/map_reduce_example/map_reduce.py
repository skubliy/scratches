'''
Created on Jun 6, 2016

@author: someone
'''
import os
import sys
import time

def test():
    print sum(x*x for x in xrange(1, 10**2, 2))
    for y  in filter(lambda x: x%2, map(lambda x: x*x, xrange(10**2))):
        print y

    print reduce(lambda x, y: x+y, filter(lambda x: x%2, map(lambda x: x*x, xrange(10**2))))
    print sum(x*x for x in xrange(1, 10**2, 2))
    
    print reduce(lambda x, y: x+y, filter(lambda x: x%2, map(lambda x: x*x, xrange(10**2)))) == sum(x*x for x in xrange(1, 10**2, 2))
    # sum(x*x for x in xrange(1, 10**6, 2))
    pass

def main(argv=None):
    test()
    print "the end"

if __name__ == '__main__':
    sys.exit(main())
