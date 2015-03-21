#include "cache.h"

class LRUCache : public Cache {
public:
  LRUCache(int m) : Cache(m) {}
  ~LRUCache() {}

  string type() {
    return "LRU CACHE";
  }

  void put(const string& key, const string& value) {
  }

  void get(const string& key, string& value) {
  }
};
