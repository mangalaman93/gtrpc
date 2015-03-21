#include <iostream>
using namespace std;

class Cache {
	int docsize;
	int maxdocsize;
  int numdoc;

public:
  // maxdocsize m is in KiloBytes
  Cache(int m) {
    maxdocsize = m;
    docsize = 0;
    numdoc = 0;
  }

  virtual ~Cache() {};

  // returns type of cache replacement policy
  virtual string type() = 0;

  // puts value for the key in the cache
  virtual void put(const string& key, const string& value) = 0;

  // gets value for the given key, returns ("") if the value is not present
  virtual void get(const string& key, string& value) = 0;
};
