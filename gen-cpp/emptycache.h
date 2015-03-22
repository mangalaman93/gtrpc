#include "cache.h"

class EmptyCache : public Cache {
public:
  EmptyCache(int m) : Cache(m) {}
  ~EmptyCache() {}

  string type() {
    return "NO CACHE";
  }

  bool contains(const string& url) {
    return false;
  }

  void put(const string& key, const string& value) {}

  void get(const string& key, string& value) {
    value = "";
  }
};
