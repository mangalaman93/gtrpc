// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <iostream>
#include <cstdlib>
#include "curl.h"
#include "cache.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ProxyRPC.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class ProxyRPCHandler : virtual public ProxyRPCIf {
  Cache *cache;
  int count;
  int hit;
 public:
  ProxyRPCHandler(int type, int cachesize) {
    switch(type) {
	    case 2: cache = new FIFOCache(cachesize);
		    break;
	    case 3: cache = new LRUCache(cachesize);
		    break;
	    case 1: cache = new RandCache(cachesize);
    		break;
      case 0: cache = new EmptyCache(cachesize);
        break;
	    default: break;
    }
  count=0;
  hit=0;
  }

  void getDocument(std::string& _return, const std::string& url) {
    _return.clear();
    count++;
    // check the cache [Cache class]
    bool present = cache->contains(url);

    // if present, return it [Cache class]
    if(present) {
      hit++;
      _return = cache->get(url);
      return;
    }
    if (count>=99) {
        printf("Hit rate: %d",(100*hit/count));
        count=0;
        hit=0;
    }
    // if not present, get the document [Curl class]
    else {
      Curl::get(url, _return);
    }

    // put the document in the cache [Cahce class]
    cache->put(url, _return);
  }

  ~ProxyRPCHandler() {
    delete cache;
  }
};

void exiting(int sig){
    printf("Exiting");
    exit(1);
}

int main(int argc, char **argv) {
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler=exiting;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags=0;
  sigaction(SIGTERM,&sigIntHandler,NULL);
  int port = 9090;

  if(argc != 3) {
    printf("usage: %s type cachesize\n", argv[0]);
    printf("type 0 for EmptyCache\n");
    printf("type 1 for RandCache\n");
    printf("type 2 for FIFOCache\n");
    printf("type 3 for LRUCache\n");
    return -1;
  }

  //int cachesize = 1024*1024; // 1 MB
  int cachesize = atoi(argv[2])*1024;
  int type = atoi(argv[1]);
  shared_ptr<ProxyRPCHandler> handler(new ProxyRPCHandler(type,cachesize));
  shared_ptr<TProcessor> processor(new ProxyRPCProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
