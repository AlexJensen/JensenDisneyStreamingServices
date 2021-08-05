#pragma once
class XAMLLeftovers
{
};

const std::string url("https://cd-static.bamgrid.com/dp-117731241344/home.json");

CURL* curl = curl_easy_init();

// Set remote URL.
curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

// Don't bother trying IPv6, which would increase DNS resolution time.
curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

// Don't wait forever, time out after 10 seconds.
curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

// Follow HTTP redirects if necessary.
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

// Response information.
long httpCode(0);
std::unique_ptr<std::string> httpData(new std::string());

// Hook up data handling function.
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

// Hook up data container (will be passed as the last parameter to the
// callback handling function).  Can be any pointer type, since it will
// internally be passed as a void pointer.
curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

// Run our HTTP GET command, capture the HTTP response code, and clean up.
curl_easy_perform(curl);
curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
curl_easy_cleanup(curl);

if (httpCode == 200)
{
    std::cout << "\nGot successful response from " << url << std::endl;

    // Response looks good - done using Curl now.  Try to parse the results
    // and print them out.
    const std::string rawJson = *httpData.get();
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;

    std::unique_ptr<std::string> httpData(new std::string());
    Json::Value jsonData;
    Json::CharReaderBuilder builder;

    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {

    }
    std::string one = root["data"]["StandardCollection"]["containers"][0]["set"]["items"][0]["image"]["tile"]["2.29"]["series"]["default"]["url"].toStyledString();