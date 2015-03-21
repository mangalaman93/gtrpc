#include "cache.h"
#include <cstdlib>
#include <vector>
#include <map>

class RandCache : public Cache {
  vector<string> urls;
  map<string, string> url_doc_map;

public:
  RandCache(int m) : Cache(m) {}
  ~RandCache() {}

  string type() {
    return "RANDOMC CACHE REPLACEMENT POLICY";
  }

  void put(const string& key, const string& value) {
    docsize += value.length();

    // loop until the cache can fit the new document
    while(true) {
      if(docsize < maxdocsize) {
        numdoc += 1;
        urls.push_back(key);
        url_doc_map[key] = value;
        break;
      } else {
        srand(time(NULL));
        unsigned to_remove = rand() % urls.size();

        if(to_remove == (urls.size()-1)) {
          docsize -= url_doc_map[urls.back()].length();
          url_doc_map.erase(urls.back());
          urls.pop_back();
        } else {
          docsize -= url_doc_map[urls[to_remove]].length();
          url_doc_map.erase(urls[to_remove]);
          urls[to_remove] = urls.back();
          urls.pop_back();
        }

        numdoc -= 1;
      }
    }
  }

  void get(const string& key, string& value) {
    if(url_doc_map.count(key) > 0) {
      value = url_doc_map[key];
    } else {
      value = "";
    }
  }
};
