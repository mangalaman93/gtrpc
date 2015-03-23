#!/usr/bin/env python
from numpy import *
import numpy as np
import sys, subprocess,os,signal,time
import random as rd

urls = genfromtxt('urls.csv', delimiter=',',dtype="S40")

if len(sys.argv) is not 2: 
    print "Correct usage: " + sys.argv[0] +" repetitions"
    exit()
rep=int(sys.argv[1])

#array[cachetype][cachesize] returns array of entries

cache={1:'RandomCache',2:'FIFOCache'}
#,3:'LRUCache'}
i=j=0
cachesize=[10, 100,1024]
for cacheindex in cache:
    timing=np.zeros((len(cachesize),rep))
    j=0
    print "Starting Test for cache "+ cache[cacheindex]
    for size in cachesize:
        us=array([])
        #cmd="./bin/ProxyRPC_server 0 1024"
        cmd="./bin/ProxyRPC_server "+str(cacheindex)+" "+str(size)
        server=subprocess.Popen(cmd,stdout=subprocess.PIPE,shell=True,preexec_fn=os.setsid)
        print "Testing for different cache sizes: "+ str(size)
        for i in range(rep):
            index=rd.randint(0,len(urls)-1)
            cmd2="./bin/get "+urls[index]
            get=subprocess.Popen(cmd2,stdout=subprocess.PIPE,shell=True)
            get.wait()
            #print "ran"
            us=np.append(us,int(get.communicate()[0].split(",")[1]))
        timing[j]=us
        j+=1
        os.killpg(server.pid,signal.SIGTERM)
        print "terminated"
    file=str(rep)+"random_load_"+str(cache[cacheindex])+".csv"
    np.savetxt(file,timing,delimiter=',',fmt='%1.0f')
    print "saved file " + file

#for i in range(rep):
#    index=rd.randint(0,len(urls)-1)
#    print urls[index]

