#ifndef DISNEY_INTERPRETER
#define DISNEY_INTERPRETER

#include <string>
#include <json/json.h>
#include <iostream>

#include "CURLHandler.h"
#include "WindowController.h"

struct mainMenuButton
{
	const int width = 200, height = 112, xstart = 40, ystart = 40, xgap = 10, ygap = 40;
	int xPos = xstart, yPos = ystart;
};


class DisneyInterpreter
{
public:
	DisneyInterpreter();
	~DisneyInterpreter();

	void DrawMainMenu(WindowController* window);

private:
	CURLHandler* curlHandler;
	Json::Value root, ref;
	JSONCPP_STRING err;

	void SetHomeFromURL(std::string url);
	void SetRefFromURL(std::string url);
	void GenerateImagesFromHome();
	void GenerateImagesFromRef(std::string refType);

	void SaveIfFileDoesNotExist(Json::Value& masterId, Json::Value& imageURL);

	bool fileExists(const std::string& file);

	void DrawRefMenu(WindowController* window, std::string refType, mainMenuButton* mbTemp);

	void SetJsonValueFromRawJson(std::string rawJson, Json::Value* val);

	void InterpretRefType(std::string& refType);
	void InterpretType(const std::string& type, Json::Value& imageURL, Json::Value& items, int itemsindex, Json::Value& masterId);
};

#endif

