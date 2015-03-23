#include <iostream>
#include <map>
#include <queue>
#include <cstdlib>
#include <vector>
using namespace std;

class Cache {
protected:
  int docsize;
  int maxdocsize;

public:
  // maxdocsize m is in KiloBytes
  Cache(int m) {
    maxdocsize = m;
    docsize = 0;
  }

  virtual ~Cache() {};

  // returns type of cache replacement policy
  virtual string type() = 0;

  // check if a document for a given url is already present
  virtual bool contains(const string& url) = 0;

  // puts value for the key in the cache
  virtual void put(const string& url, const string& doc) = 0;

  // gets value for the given key [should only be called when present]
  virtual string get(const string& key) = 0;
};

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
    if(doc.length() > maxdocsize) {
      printf("cannot accomodate this document in cache!\n");
      return;
    }

    docsize += doc.length();

    // loop until the cache can fit the new document
    while(true) {
      if(docsize <= maxdocsize) {
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
