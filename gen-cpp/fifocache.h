#include "cache.h"
#include <queue>

class FIFOCache : public Cache {
  queue<string> urls;

public:
  FIFOCache(int m) : Cache(m) {}
  ~FIFOCache() {}

  string type() {
    return "FIFO CACHE";
  }

  void put(const string& key, const string& value) {
  	docsize += value.length();

    // loop until the cache can fit the new document
    while(true) {
      if(docsize < maxdocsize) {
        numdoc += 1;
        urls.push(key);
        url_doc_map[key] = value;
        break;
      } else {
        docsize -= url_doc_map[urls.front()].length();
        url_doc_map.erase(urls.front());
        urls.pop();
        numdoc -= 1;
      }
    }
  }
};
