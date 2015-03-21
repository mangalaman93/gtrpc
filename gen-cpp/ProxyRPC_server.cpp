// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <iostream>
#include "curl.h"
#include "randcache.h"

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

 public:
  ProxyRPCHandler(int cachesize) {
    cache = new RandCache(cachesize);
  }

  void getDocument(std::string& _return, const std::string& url) {
    _return.clear();

    // check the cache [Cache class]
    cache->get(url, _return);

    // if present, return it [Cache class]
    if(_return.compare("") != 0) {
      return;
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

int main(int argc, char **argv) {
  int port = 9090;

  // @todo, replace it with the argument passed from command line
  int cachesize = 1024*1024; // 1 MB

  shared_ptr<ProxyRPCHandler> handler(new ProxyRPCHandler(cachesize));
  shared_ptr<TProcessor> processor(new ProxyRPCProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
