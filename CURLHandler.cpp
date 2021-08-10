#include "CURLHandler.h"


const std::string CURLHandler::GetJSONFromURL(const std::string url)
{
    curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLoption::CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLoption::CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLoption::CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLoption::CURLOPT_FOLLOWLOCATION, 1L);

    // Response information.
    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLoption::CURLOPT_WRITEFUNCTION, write_callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLoption::CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO::CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        std::cout << "\nGot successful response from " << url << std::endl;
        return httpData.get()->c_str();
    }
    else
    {
        std::cout << "\nGot failed response from " << url << std::endl;
        return "FAIL";
    }
}

void CURLHandler::SaveImageFromURL(const std::string url, const char* directory, const char* outfilename)
{
    FILE* fp;
    CURLcode res;
    errno_t ferr;

    BuildDirectory(directory);

    std::string readBuffer;
    ferr = fopen_s(&fp, outfilename, "wb");
    if (ferr == 0)
    {
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        if (fp != NULL)
            fclose(fp);
    }
}

void CURLHandler::BuildDirectory(const char* directory)
{
    //directory building
    std::string d = directory;
    std::string delimiter = "\\";

    size_t pos = 0;
    std::string token;
    while ((pos = d.find(delimiter)) != std::string::npos) {
        token = d.substr(0, pos);
        std::cout << token << std::endl;
        d.erase(0, pos + delimiter.length());
    }
}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE * stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}
