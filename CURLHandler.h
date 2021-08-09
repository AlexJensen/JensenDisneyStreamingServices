#ifndef CURL_HANDLER_H
#define CURL_HANDLER_H

#include <curl/curl.h>
#include <string>

class CURLHandler
{
public:
	CURL* curl = curl_easy_init();
	CURLHandler() {}
	const std::string GetJSONFromURL(const std::string url);
	void SaveImageFromURL(const std::string url, const char* outfilename);
};

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);

#endif
