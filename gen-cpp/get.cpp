#include "ProxyRPC.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <sys/time.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char **argv) {
  if(argc != 3) {
    printf( "usage: %s server-ip url\n", argv[0]);
    return -1;
  }

  boost::shared_ptr<TTransport> socket(new TSocket(argv[1], 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  ProxyRPCClient client(protocol);

  struct timeval start, end;
  long delta,seconds, useconds;

  // Get List of URLs +  probabilities from doc
  string url = argv[2];

  // Maybe check the format of the url? "http://www.google.com/";
  try {
    transport->open();

    string doc;
    gettimeofday(&start, NULL);
    client.getDocument(doc, url);
    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    delta = seconds*1000000 + useconds;
    //cout << " **** got the document ***** " << endl;
    cout << "Took ,"<< delta << ", us to get it"<< endl;
    //cout << "--------------------------------" << endl;
  } catch (TException& tx) {
    cout << "ERROR: " << tx.what() << endl;
  }

  transport->close();
  return 0;
}
