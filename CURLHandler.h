#ifndef CURL_HANDLER_H
#define CURL_HANDLER_H

#include <curl/curl.h>
#include <string>
#include <filesystem>
#include <memory>
#include <iostream>

/// <summary>
/// Handles grabbing data from URLs
/// </summary>
class CURLHandler
{
public:
	CURL* curl = curl_easy_init();
	CURLHandler() {}

	/// <summary>
	/// Returns a raw JSON string that represents data pulled from a URL.
	/// </summary>
	/// <param name="url">URL of JSON file to download.</param>
	/// <returns>Raw JSON</returns>
	const std::string GetJSONFromURL(const std::string url);

	/// <summary>
	/// Saves an image to the Textures folder from data pulled from a URL.
	/// </summary>
	/// <param name="url">URL of Image file to download</param>
	/// <param name="outfilename"></param>
	void SaveImageFromURL(const std::string url, std::string outfilename);
};

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);

#endif

