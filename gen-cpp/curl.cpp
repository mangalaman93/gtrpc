#include "curl.h"

/* This function is registered as a callback with CURL.  As the data
   from the requested webpage is returned in chunks, write_data is
   called with each chunk.  */
static size_t write_data(void *buffer, size_t size,
                         size_t nmemb, void *userp) {
  string *doc = (string*)userp;
  doc->append((char*)buffer, size*nmemb);

  return size * nmemb;
}

void Curl::get(const string& url, string& doc) {
  CURL *curl;
  doc.clear();

  /* Get a curl handle. */
  curl = curl_easy_init();

  if(NULL != curl) {
    /* Set the URL for the operation. */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* "write_data" function to call with returned data. */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    /* userp parameter passed to write_data. */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &doc);

    // follow redirection
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* Actually perform the query &
       Check the return value and do whatever. */
    if(0 != curl_easy_perform(curl)) {
      fprintf(stderr, "Error: could not get the document from URL:%s\n", url.c_str());
      exit(EXIT_FAILURE);
    }

    /* Clean up after ourselves. */
    curl_easy_cleanup(curl);
  } else {
    fprintf(stderr, "Error: could not get CURL handle.\n");
    exit(EXIT_FAILURE);
  }
}
