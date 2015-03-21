#!/usr/local/bin/thrift --gen cpp

service ProxyRPC {
  binary getDocument(1:string url)
}
