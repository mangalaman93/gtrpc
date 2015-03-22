#include <iostream>
using namespace std;

class Cache {
protected:
	int docsize;
	int maxdocsize;

public:
  // maxdocsize m is in KiloBytes
  Cache(int m) {
    maxdocsize = m;
    docsize = 0;
  }

  virtual ~Cache() {};

  // returns type of cache replacement policy
  virtual string type() = 0;

  // check if a document for a given url is already present
  virtual bool contains(const string& url) = 0;

  // puts value for the key in the cache
  virtual void put(const string& url, const string& doc) = 0;

  // gets value for the given key [should only be called when present]
  virtual string get(const string& key) = 0;
};
