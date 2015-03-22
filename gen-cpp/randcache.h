#include "cache.h"
#include <cstdlib>
#include <map>
#include <vector>

class RandCache : public Cache {
  vector<string> urls;
  map<string, string> urlmap;

public:
  RandCache(int m) : Cache(m) {}
  ~RandCache() {}

  string type() {
    return "RANDOMC CACHE REPLACEMENT POLICY";
  }

  bool contains(const string& url) {
    return (urlmap.count(url) > 0);
  }

  void put(const string& url, const string& doc) {
    docsize += doc.length();

    // loop until the cache can fit the new document
    while(true) {
      if(docsize < maxdocsize) {
        urls.push_back(url);
        urlmap[url] = doc;
        break;
      } else {
        srand(time(NULL));
        unsigned to_remove = rand() % urls.size();

        string str = urlmap[urls[to_remove]];
        urlmap.erase(urls[to_remove]);
        docsize -= str.length();

        if(to_remove != (urls.size()-1)) {
          urls[to_remove] = urls.back();
        }

        urls.pop_back();
      }
    }
  }

  string get(const string& key) {
    return urlmap[key];
  }
};
