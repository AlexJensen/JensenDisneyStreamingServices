#ifndef DISNEY_INTERPRETER
#define DISNEY_INTERPRETER

#include <string>
#include <json/json.h>
#include <iostream>

#include "CURLHandler.h"
#include "WindowController.h"

class DisneyInterpreter
{
public:
	DisneyInterpreter();
	~DisneyInterpreter();

	void DrawMainMenu(WindowController window);

private:
	CURLHandler* curlHandler;
	Json::Value root, ref;
	JSONCPP_STRING err;

	void SetHomeFromURL(std::string url);
	void SetRefFromURL(std::string url);
	void GenerateImagesFromHome();
	void GenerateImagesFromRef();

	void SetJsonValueFromRawJson(std::string rawJson, Json::Value* val);
	void InterpretType(const std::string& type, Json::Value& imageURL, Json::Value& items, int itemsindex, Json::Value& masterId);
};

#endif

