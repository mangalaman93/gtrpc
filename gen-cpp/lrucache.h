#include "cache.h"
#include <map>

typedef struct LRUDoc {
  string doc;
  string url;
  struct LRUDoc *prev;
  struct LRUDoc *next;

  LRUDoc(string d, string u, struct LRUDoc *p, struct LRUDoc *n) :
   doc(d), url(u), prev(p), next(n) {}
} LRUDoc;

class LRUCache : public Cache {
  LRUDoc* head;
  LRUDoc* tail;
  map<string, LRUDoc*> urlmap;

public:
  LRUCache(int m) : Cache(m) {
    head = NULL;
    tail = NULL;
  }

  ~LRUCache() {}

  string type() {
    return "LRU CACHE";
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
        LRUDoc *node = new LRUDoc(doc, url, NULL, head);
        urlmap[url] = node;
        node->next = head;
        head = node;
        break;
      } else {
        LRUDoc* to_remove;

        if(tail == head) {
          to_remove = tail;
          tail = NULL;
          head = NULL;
        } else {
          tail->prev->next = NULL;
          to_remove = tail;
          tail = tail->prev;
        }

        docsize -= to_remove->doc.length();
        urlmap.erase(to_remove->url);
        delete to_remove;
      }
    }
  }

  string get(const string& key) {
    LRUDoc *cur = urlmap[key];

    if(cur != head) {
      cur->prev->next = cur->next;
      cur->prev = NULL;
      cur->next = head;
      head = cur;
    }

    return cur->doc;
  }
};
