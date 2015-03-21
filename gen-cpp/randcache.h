#include "cache.h"

class RandCache : public Cache {
  string **data;

public:
  RandCache(int m) : Cache(m) {
    // assuming an average of 1KB document size
    data = new string*[m];
  }

  ~RandCache() {
    delete[] data;
  }

  string type() {
    return "RANDOMC CACHE REPLACEMENT POLICY";
  }

  void put(const string& key, const string& value) {
  }

  void get(const string& key, string& value) {
  }
};
