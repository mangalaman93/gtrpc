//#include "cache.h"
#include <map>
#include <queue>

class FIFOCache : public Cache {
  queue<string> urls;
  map<string, string> urlmap;

public:
  FIFOCache(int m) : Cache(m) {}
  ~FIFOCache() {}

  string type() {
    return "FIFO CACHE";
  }

  bool contains(const string& url) {
    return (urlmap.count(url) > 0);
  }

  void put(const string& url, const string& doc) {
    if(doc.length() > maxdocsize) {
      printf("cannot accomodate this document in cache!\n");
      return;
    }

  	docsize += doc.length();

    // loop until the cache can fit the new document
    while(true) {
      if(docsize <= maxdocsize) {
        urls.push(url);
        urlmap[url] = doc;
        break;
      } else {
        docsize -= urlmap[urls.front()].length();
        urlmap.erase(urls.front());
        urls.pop();
      }
    }
  }

  string get(const string& key) {
    return urlmap[key];
  }
};
