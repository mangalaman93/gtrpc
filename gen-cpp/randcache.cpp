#include "randcache.h"

RandCache::RandCache(int m) : Cache(m) {
  // assuming an average of 1KB document size
  data = new string*[m];
}

RandCache::~RandCache() {
  delete[] data;
}

string RandCache::type() {
  return "RANDOMC CACHE REPLACEMENT POLICY";
}

void RandCache::put(string key, string value) {
}

string RandCache::get(string key) {
}
