#include "ProxyRPC.h"

#include <sstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char **argv) {
  if(argc != 2) {
    printf( "usage: %s server-ip\n", argv[0]);
    return -1;
  }

  boost::shared_ptr<TTransport> socket(new TSocket(argv[1], 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ProxyRPCClient client(protocol);

  try {
    transport->open();

    string doc;
    string url = "https://www.google.com/?q=hello#safe=off&q=";

    for(int i=0; i<100; i++) {
      stringstream ss;
      ss<<url<<i;
      client.getDocument(doc, ss.str());
      doc.clear();
    }

    for(int i=0; i<100; i++) {
      stringstream ss;
      ss<<url<<i;
      client.getDocument(doc, ss.str());
      doc.clear();
    }
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }

  transport->open();
  return 0;
}
