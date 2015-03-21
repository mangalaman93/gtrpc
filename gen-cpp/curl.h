#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <curl/curl.h>
using namespace std;

class Curl {
public:
  static void get(const string& url, string& doc);
};
