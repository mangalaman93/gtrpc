#include <iostream>
using namespace std;

class Cache {
	int docsize;
	int maxsize;
  int numdoc;

public:
  // maxsize m is in KiloBytes
  Cache(int m);
  virtual ~Cache();

  // returns type of cache replacement policy
  virtual string type() = 0;

  // puts value for the key in the cache
  virtual void put(string key, string value) = 0;

  // gets value for the given key, returns ("") if the value is not present
  virtual string get(string key) = 0;
};
