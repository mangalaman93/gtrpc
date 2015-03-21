#include "cache.h"

class FIFOCache : public Cache {
public:
  FIFOCache(int m) : Cache(m) {}
  ~FIFOCache() {}

  string type() {
    return "FIFO CACHE";
  }

  void put(const string& key, const string& value) {
  }

  void get(const string& key, string& value) {
  }
};
