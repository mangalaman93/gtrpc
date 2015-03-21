#include "cache.h"

class RandCache : public Cache {
  string **data;

public:
  RandCache(int m);
  ~RandCache();

  string type();
  void put(string key, string value);
  string get(string key);
};
