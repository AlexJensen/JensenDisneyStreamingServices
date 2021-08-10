#ifndef DISNEY_INTERPRETER
#define DISNEY_INTERPRETER

#include <string>
#include <json/json.h>

#include "CURLHandler.h"
#include "WindowController.h"

class DisneyInterpreter
{
public:
	DisneyInterpreter();
	~DisneyInterpreter();
	void SetHomeFromURL(std::string url);
	void SetRefFromURL(std::string url);
	void SetJsonValueFromRawJson(std::string rawJson, Json::Value val);
	void GenerateImagesFromHome();
	void GenerateImagesFromRef();
	void DrawMainMenu(WindowController window);

private:
	CURLHandler* curlHandler;
	Json::Value root, ref;
	JSONCPP_STRING err;
};

#endif

