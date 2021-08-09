#ifndef DISNEY_JSON_PARSER
#define DISNEY_JSON_PARSER

#include <string>
#include <json/json.h>

#include "CURLHandler.h"
class DisneyHomeJSONInterpreter
{
public:
	DisneyHomeJSONInterpreter();
	void InterpretJSONFromURL(std::string url);
	void InterpretJSON(std::string rawJson);
private:
	CURLHandler* curlHandler;
	Json::Value root;
	JSONCPP_STRING err;
};

#endif

