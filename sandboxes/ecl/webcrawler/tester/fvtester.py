#!/usr/bin/env python
# encoding: utf-8
'''
fsutils.ardiff -- Utility to properly delete .bin config file for fv

Created on 2015-07-28

@author:     Serguei Koubli

@contact:    sergueikoubli@gmail.com
'''
import sys
import requests
from time import sleep
from time import clock
from time import time
#import http.client, urllib.parse
#from httplib2 import Http
import urllib
import urllib2
#import pylzjb
from io import BytesIO
#from cStringIO import StringIO

import zipfile
 
import pprint
from subprocess import Popen, PIPE
from string import split
from Cookie import Cookie
#import os 

verbose = 1
pp = pprint.PrettyPrinter(indent=4).pprint

def ppv(vals, vl=99):
    if verbose > vl:
        if isinstance(vals, dict):
            for k,v in vals.items():
                print k,":\t",v
        elif isinstance(vals, basestring):
            print vals
        else:
            for v in vals:
                print v,
            print ""

class url_holder():
    def __init__(self, url):
        if url:
            self.url = url
        self.host = None
        self.port = None

    def parse_url(self, url):
        splitedurl = url.split('//')[-1]
        self.host = splitedurl[0]
        x = splitedurl[0].split(':')
        self.port = (x[1] if len(x) == 2 else 80)
        ppv((url,"\thost[%s:%s]"%(self.host, self.port)))
    
    def get_host(self):
        
        if self.host:
            return self.host
        self.parse_url(self.url)
        print "111",self.host
        return self.host
    
    

class client():
    clients = {}
    def __init__(self, client_name, str_headers = ""):
        self.cookies = {}
        self.hdrs = {}
        self.cur_client = client_name
        self.set_headers(str_headers)
        client.clients[client_name]=self

    def set_headers(self, str_headers=""):
        # User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.89 Safari/537.36\n\
        #Pragma: no-cache\n\
        str_static_header = "\
Connection: keep-alive\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\n\
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.93 Safari/537.36\n\
HTTPS: 1\n\
Accept-Encoding: gzip, deflate, sdch\n\
Accept-Language: en-GB,en-US;q=0.8,en;q=0.6"
        if not str_headers:
            str_headers = str_static_header

        for x in str_headers.split("\n"):
            kv = x.split(':')
            if len(kv) != 2:
                print "Wrong header it contaion more then 1 ':' will be skipped.\t", x
                continue
            else:
                self.hdrs[kv[0]] = kv[1]
                
    def set_url(self, url):

        if url:
            self.cur_url = url_holder(url)

        self.hdrs['Host'] = self.cur_url.get_host()

    def do_request(self, url = None):
        if url:
            self.set_url(url)

        cookie_val = self.get_str_cookies()
        if cookie_val != '':
            self.hdrs['Cookie'] = cookie_val
        
        ppv(self.hdrs,99)
        print "##########"
        # *** urlib2 ***
        request = urllib2.Request(self.cur_url.url, urllib.urlencode({'name' : 'SKub'}), self.hdrs)
        #base64string = encodestring('%s:%s' % ('user', 'pass')).replace('\n', '')
        
        #request.add_header('Pragma', 'no-cache')
        #for k, v in request.headers.items(): 
        #    print k,"=",v

        response = urllib2.urlopen(request)
        response.read()
        ppv(("RET CODE:",response.getcode()),40)
        ppv(("___ headers ___", 50))
        for k, v in response.headers.items():
            self.hdrs[k] = v
        ppv(self.hdrs, 50)
        ppv(("___ cookies ___"), 60)
        for x in response.headers.getallmatchingheaders("set-cookie"):
            p1, t, p2 = x.partition('=')
            k = p1.split(':')[1]
            v, t, t2 = p2.rpartition('\r')
            self.cookies[k] = v
        ppv((self.cookies), 60)
        """
        if verbose > 70: 
            r = False
            #if url.find("html") > 0 : print response.read()
            for ss in "html", "text":
                print ss,"   ", self.hdrs['content-type']
                if ss in self.hdrs['content-type']:
                    r = True
                    if 'gzip' in self.hdrs['content-encoding']: 
                        bff = response.read(8)
                        for j in range(0,7):
                            print "%x"%bff[j]
                        print zipfile.ZipFile(BytesIO(bff),'r')
                        
                    else: print response.read()
                    break
            if r : print "binary body ..."
            """
        ppv (("__end_do_request___",""),10)

    def get_cookies_as_str(self):
        str_cookies = ""
        for k in self.cookies.keys():
            str_cookies += "Set-Cookie: " + self.cookies[k] + "\r\n"
        return str_cookies 

    def get_str_cookies(self):
        if "set-cookie" in self.hdrs.keys():
            return self.hdrs["set-cookie"]
        return ""

    def clean_cookie(self):
        self.cookies = {}

def do_compile(url):
    n = 0
    sleep_time = 1
    while True:
        usr=client("usr-%d"%n)
        n += 1
        usr.do_request(url)
        if 'x-sl-compstate' in usr.hdrs.keys():
            x = usr.hdrs['x-sl-compstate']
            if (x[0] == 'C') or (x[0] == 'T'):
                return n
            if x[0] == 'U':
                if n > 2: sleep_time = 2
                elif n > 3: sleep_time = 5
        sleep(sleep_time)

    
def do_request(usr = "usr-new", url = None):
    if "usr_new" in usr:
        if usr in client.clients.keys():
            client.clients[usr].clean_cookie()
            
    if usr not in client.clients.keys():
        print "Creating new client %s"%usr
        client(usr) 
        
    clnt = client.clients[usr]
    clnt.do_request(url)
    #print clnt.get_cookies_as_str()

def test_def(url):

    '''str_cookie = "\Cookie: SL_Audience=995|Accelerated| 96992777e9da903.329|1|0; SL_ClassKey=0.1.1.1.2; SL_UVId=2fc4ed142efa34da"
    cookie = {}
    str_cookie_value = str_cookie.split(':')[1]
    for x in str_cookie_value.split(';'):
        k, v = x.split("=")
        cookie[k] = v
    '''
    # first visitor
    print "---- %d ---- "%do_compile(url)
    print "==== COMPILED ==="
    sleep(0.5)
    do_request("usr-1", url)
    sleep(0.5)
    do_request("usr-1", url)
    print "==== THE END ===="
    xxx = 2
    while True:
        xxx = 3 if xxx == 2 else 2
        url = "http://192.168.140.172:8090/index%d.html"%xxx
        print "---- %d ---- "%do_compile(url)
        sleep(0.5)
        do_request("usr-1", url)
        sleep(0.5)
        do_request("usr-1", url)
        sleep(0.5)
    pass

def tspeed(url):
    global verbose
    print "+++++++++++++++++++++++++++++++++++++++++++", url
    do_request("usr-2", url)
    print "++++++++///////////////"
    verbose = 1
    num_req= 10000
    t1 = clock()

    for i in xrange(1, num_req):
        do_request("usr-2")
    t2 = clock()
    print "-------------"
    print repr(t2)
    print repr(t1)
    print " = " 
    print t2-t1," / ", num_req, " = ", (t2-t1)/num_req
    print "_____________"

def test(url):
    '''str_cookie = "\Cookie: SL_Audience=995|Accelerated| 96992777e9da903.329|1|0; SL_ClassKey=0.1.1.1.2; SL_UVId=2fc4ed142efa34da"
    cookie = {}
    str_cookie_value = str_cookie.split(':')[1]
    for x in str_cookie_value.split(';'):
        k, v = x.split("=")
        cookie[k] = v
    '''
    #test_requqests(url)
    global verbose 
    verbose = 71
    # first visitor
    print "---- %d ---- "%do_compile(url)
    print "==== COMPILED ==="
    sleep(0.5)
    do_request("usr-1", url)
    print "==== THE END ===="
    #300k slrs
    #picurl="http://192.168.59.26:7887/AverageSite/v.jpg?Lo0P=11515f80f40212f489c7a68cef8293b7Dc"
    #58k slrs
    #picurl = "http://192.168.59.26:7887/AverageSite/v.jpg?Lo0P=c9709f2f7c7d125a97cc67c7ea73ec70Dc"
    #8k slrs
    #picurl = "http://192.168.59.26:7887/AverageSite/v.jpg?Lo0P=f807cb8e04e4c794ee05aef081346ae6Dc"
    #TOS
    picurl = "http://192.168.59.26:7887/AverageSite/v.jpg"

    tspeed(picurl)
    verbose = 71
    picurl = "http://192.168.59.26:80/AverageSite/v.jpg"
    do_request("usr-3", picurl)
    tspeed(picurl)


def test_requqests(url):
    r = requests.get(url)
    #r = urllib2.urlopen(url).read()
    for item in r.headers:
        print item
    print "---"
    pass

def main(argv=None):    
    test(sys.argv[1])
    pass

if __name__ == "__main__":
    if len(sys.argv) == 1:
        sys.argv.append("http://192.168.59.26:7887/AverageSite/index1.html")
        #sys.argv.append("http://192.168.59.26:7887/AverageSite/v.jpg")
        #sys.argv.append("http://192.168.140.172:8090/index2.html")
    #print "zzz %s"%sys.argv    
    sys.exit(main())