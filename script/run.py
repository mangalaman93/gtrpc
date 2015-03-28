#!/usr/bin/env python
from numpy import *
import numpy as np
import sys, subprocess,os,signal,time
import random as rd

if len(sys.argv) is not 3:
    print "Correct usage: " + sys.argv[0] +" load_type num-repetitions"
    print "load-type => 0: random workload, 1: exponential workload'"
    exit()

rep = int(sys.argv[2])

# determine load type
if int(sys.argv[1]):
    load_type="random"
    filename = 'data/top10.csv'
else:
    load_type="exp"
    filename = 'data/top100.csv'

urls = genfromtxt(filename, delimiter=',', dtype="S40")
cache={1:'RandomCache', 2:'FIFOCache', 3:'LRUCache'}

i=j=0
cachesize=[0, 64, 128, 256, 512, 1024, 4096]
for cacheindex in cache:
    timing = np.zeros((len(cachesize),rep))
    j = 0
    print "Testing cache "+ cache[cacheindex]
    for size in cachesize:
        us=array([])
        cmd = "./bin/ProxyRPC_server "+str(cacheindex)+" "+str(size)
        server=subprocess.Popen(cmd,shell=True,preexec_fn=os.setsid)
        print "Cache size: "+ str(size)
        for i in range(rep):
            index = rd.randint(0,len(urls)-1) if int(sys.argv[1]) else min(len(urls)-1,int(rd.expovariate(0.1)))
            cmd2 = "./bin/get 0.0.0.0 "+urls[index]
            get = subprocess.Popen(cmd2,stdout=subprocess.PIPE,shell=True)
            get.wait()
            a = get.communicate()[0]
            us = np.append(us,int(a.split(",")[1]))
        timing[j] = us
        j += 1
        os.killpg(server.pid,signal.SIGTERM)
    file = "data/"+str(rep)+load_type+"_load_"+str(cache[cacheindex])+".csv"
    np.savetxt(file,timing, delimiter=',', fmt='%1.0f')
    print "saved " + file
