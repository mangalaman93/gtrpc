#include "ProxyRPC.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char **argv) {
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ProxyRPCClient client(protocol);

  try {
    transport->open();

    string doc;
    string url = "http://www.google.com/";
    client.getDocument(doc, url);
    cout << " **** got the document ***** " << endl;
    cout << doc << endl;
    cout << "--------------------------------" << endl;
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }

  transport->open();
  return 0;
}
