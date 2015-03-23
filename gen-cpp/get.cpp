#include "ProxyRPC.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include<sys/time.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char **argv) {
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  ProxyRPCClient client(protocol);

  struct timeval start, end;
  long delta,seconds, useconds;

  if (argc != 2){ printf( "usage: %s url", argv[0] ); return -1;}
  //Get List of URLs +  probabilities from doc
  string url = argv[1];
  //Maybe check the format of the url? "http://www.google.com/";
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
  // Why do we open it again?
  transport->open();
  return 0;
}
