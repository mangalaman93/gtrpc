#include "cache.h"

Cache::Cache(int m) {
  maxsize = m;
  docsize = 0;
  numdoc = 0;
}

Cache::~Cache() {}
