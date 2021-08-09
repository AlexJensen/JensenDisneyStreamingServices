#include "DisneyHomeJSONInterpreter.h"
#include <iostream>

DisneyHomeJSONInterpreter::DisneyHomeJSONInterpreter()
{
	curlHandler = new CURLHandler();
}

void DisneyHomeJSONInterpreter::InterpretJSONFromURL(std::string url)
{
	const std::string rawJson = curlHandler->GetJSONFromURL(url);
	if (rawJson != "FAIL")
	{
		InterpretJSON(rawJson);
	}
}

void DisneyHomeJSONInterpreter::InterpretJSON(std::string rawJson)
{
	const auto rawJsonLength = static_cast<int>(rawJson.length());

	std::unique_ptr<std::string> httpData(new std::string());
	Json::CharReaderBuilder builder;

	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err)) {
		std::cout << "ERROR::JSON: Failed to interpret JSON " << std::endl;
	}
}


/*Json::Value item = root["data"]["StandardCollection"]["containers"][2]["set"]["items"][0];
	   std::string typeStr = item["type"].asString();

	   std::string URLContainer = "";
	   if (typeStr == "DmcSeries")
	   {
		   URLContainer = "series";
	   }
	   if (typeStr == "DmcVideo")
	   {
		   URLContainer = "program";
	   }
	   std::string imageURL = item["image"]["tile"]["0.71"][URLContainer]["default"]["url"].asString();


	   curlHandler->SaveImageFromURL(imageURL, "textures/1.jpg");*/
