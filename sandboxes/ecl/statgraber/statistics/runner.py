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
#import pprint
import pxssh
import pexpect

token = "OGJCBBM"

def do_scenario(proc, scenario = ">bash\n<ls"):
    for ln in scenario.split("\n"):
        if ln[0] == '>':
            proc.expect(ln[1:])
        elif ln[0] == '<':
            proc.sendline(ln[1:])
        else
            print "ERROR WRONG SCENARIO: ", scenario
        
def do_job(_addr="217.163.62.10", _username = 'admin', _password = 'admin', _port = 10222):
    #child = pexpect.spawn ("sshpass -p 'admin' ssh -p 10222 admin@217.163.62.10")
    cmd = "sshpass -p '%s' ssh -p %d %s@%s"%(_password,_port,_username,_addr)
    print cmd
    child = pexpect.spawn (cmd)
    child.logfile_read = sys.stdout
    child.expect([pexpect.EOF, "admin"])
    child.sendline ('shell')
    #for ln in child.readlines():
    #    print ln
    #print child.readlines()
    index = child.expect([pexpect.EOF, "token", 'bash', "admin"])
    print "---------", index
    if index != 2:
        if index == 1:
            child.sendline (token)
            print child.expect([pexpect.EOF, 'bash', "admin"])
        else:
            print "error"
            child.sendline ('exit')
            return
    child.sendline ("ls -la ~/tmp/statgrab/")
    child.expect([pexpect.EOF, "bash", "admin"])
    child.sendline ('exit')
    child.expect([pexpect.EOF, "bash", "admin"])
    child.sendline ('exit')
    index = child.expect([pexpect.EOF, "xxxxzzz"])
    print "======", index

def do_run(_addr="217.163.62.10", _username = 'admin', _password = 'admin', _port = 10222):
    s = pxssh.pxssh()
    #for k,v in s.__dict__.items():
    #    print k, "\t", v 
    
    if not s.login (server = _addr, username = _username, password = _password, port = _port, terminal_type='dumb'): # terminal_type='ansi'):
        print "SSH session failed on login."
        print str(s)
    else:
        print "SSH session login successful"
        s.sendline ('help')
        s.prompt()         # match the prompt
        print s.before     # print everything before the prompt.
        s.logout()
        
        
def main(nm = "Memory/data"):
    argv = sys.argv[1:]
    nm = nm if (len(argv) == 0) else argv[0]
    cmd = "du  -s /var/lib/fsdb/ResourceServer/" + nm
    do_job()
    #do_job(_addr="172.16.0.69", _username = 'fvdev', _password = 'fvPassw0rd', _port = 22)
    #do_run()
    #do_run(_addr="172.16.0.69", _username = 'fvdev', _password = 'fvPassw0rd', _port = 22)
    print "hi"

if __name__ == '__main__':
    sys.exit(main())