#!/usr/bin/env python
# encoding: utf-8
'''
Created on Jul 1, 2015

statistics.blockio -- TODO: shortdesc

statistics.blockio TODO: is a description

It defines blockio

@author: Serguei Koubli

@copyright:  Serguei Koubli. All rights reserved.

@license:    TODO: license

@contact:    SergueiKoubli@gmail.com
@deffield    updated: Updated ?
'''

import sys
import os
import requests
import json
#import pprint

class FastViewInterface():
    def __init__(self, host, user, password, file_name):
        self.user = user
        self.password = password
        self.host = host
        self._cgiPath = "cgi-bin/config.cgi"
        self._log_path = "sl-logs/statgrab/"
        self._log_dsc = self._log_path + file_name + ".dsc"
        self._log_name = self._log_path + file_name + ".log"
        self.session = None

    def login(self):
        self._nicePost("config=session&action=login", {"username": self.user, "password": self.password})

    def logout(self):
        self._nicePost("config=session&action=logout")

    def getLicense(self):
        return self._nicePost("config=license&action=get")

    def _nicePost(self, query, data=""):
        if not isinstance(data, basestring):
            data = "data=" + json.dumps(data)

        try:
            req = requests.post("https://" + self.host + "/" + self._cgiPath + "?" + query,
                                data=data, cookies={"session": self.session}, verify=False)
        except Exception as err:
            raise err

        if req.status_code != 200:
            raise Exception("Got non 200 status code")

        result = json.loads(req.text)

        if result["Result"]["Code"] != 0:
            raise Exception("Got non-zero result code")

        if self.session is None:
            self.session = req.cookies["session"]

        return result["Data"]

    def _getFile(self, _path):
        req = requests.get("https://" + self.host + "/" + _path ,
                            cookies={"session": self.session}, verify=False)
        #print req
        _p, ext=_path.split(".")
        n = _p.split("/")[-1]
        fn = "./" + n + "-" + self.host.split(':')[1] + "." + ext
        fd=open( fn , "w")
        fd.write(req.text)
        print fn
        #print req.txt
        #print "==============================================================="

    def do_job(self):
        self.login()
        #self._getFile(self._log_dsc)
        #self._getFile(self._log_name)
        #self._getFile("sl-logs/proxy.log")
        #self._getFile("sl-logs/proxy.log.1")
        self._getFile("sl-logs/smfhub.log")
        #self._getFile("sl-logs/smfhub.log.1")
        self._getFile("sl-logs/config.log")
        #self._getFile("sl-logs/config.log.1")
        #self.logout()

hosts = "\
10001111 = 217.163.62.10:10200 = def_statistics-150709225226\t\
10001112 = 217.163.62.10:10210 = def_statistics-150709225738\t\
10001128 = 217.163.62.10:10250 = def_statistics-150709225938\t\
10001130 = 217.163.62.10:10253 = def_statistics-150709230054\t\
10001131 = 217.163.62.10:10256 = def_statistics-150709230227\t\
10001137 = 217.163.62.10:10260 = def_statistics-150709230355\t\
10001144 = 217.163.62.10:10263 = def_statistics-150709225042"

def main(href=None):
    '''
    r = requests.get('http://python.org')
    print r
    print r.raw
    print r.elapsed
    print r.url
    print r.text
    '''
    
    for x in hosts.split("\t"):
        xx = x.split(" = ")
        FastViewInterface(xx[1], 'admin', 'admin', xx[2]).do_job()

if __name__ == '__main__':
    sys.exit(main())
