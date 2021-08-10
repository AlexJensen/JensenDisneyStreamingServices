#include "DisneyInterpreter.h"
#include <iostream>

const std::string HOME_JSON("https://cd-static.bamgrid.com/dp-117731241344/home.json");
const std::string REF_JSON("https://cd-static.bamgrid.com/dp-117731241344/sets/<refid>.json");

DisneyInterpreter::DisneyInterpreter()
{
	curlHandler = new CURLHandler();
	SetHomeFromURL(HOME_JSON);
	GenerateImagesFromHome();
}

DisneyInterpreter::~DisneyInterpreter()
{
	delete curlHandler;
}

void DisneyInterpreter::SetHomeFromURL(std::string url)
{
	const std::string rawJson = curlHandler->GetJSONFromURL(url);
	if (rawJson != "FAIL")
	{
		SetJsonValueFromRawJson(rawJson, root);
	}
}

void DisneyInterpreter::SetRefFromURL(std::string url)
{
	const std::string rawJson = curlHandler->GetJSONFromURL(url);
	if (rawJson != "FAIL")
	{
		SetJsonValueFromRawJson(rawJson, ref);
	}
}


void DisneyInterpreter::SetJsonValueFromRawJson(std::string rawJson, Json::Value val)
{
	const auto rawJsonLength = static_cast<int>(rawJson.length());

	std::unique_ptr<std::string> httpData(new std::string());
	Json::CharReaderBuilder builder;

	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &val, &err)) {
		std::cout << "ERROR::JSON: Failed to interpret JSON " << std::endl;
	}
}

void DisneyInterpreter::GenerateImagesFromHome()
{
	const Json::Value containers = root["data"]["StandardCollection"]["containers"];
	Json::Value items;
	for (int containerindex = 0; containerindex < containers.size(); ++containerindex)
	{
		const std::string contentClass = containers[containerindex]["set"]["contentClass"].asString();
		if (contentClass == "editorial")
		{
			items = containers[containerindex]["set"]["items"];
			for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
			{
				const std::string type = items[itemsindex]["type"].asString();
				Json::Value imageURL, masterId;
				if (type == "DmcSeries")
				{
					imageURL = items[itemsindex]["image"]["tile"]["1.78"]["series"]["default"]["url"];
					masterId = items[itemsindex]["image"]["tile"]["1.78"]["series"]["default"]["masterId"];
				}
				else if (type == "DmcVideo")
				{
					imageURL = items[itemsindex]["image"]["tile"]["1.78"]["program"]["default"]["url"];
					masterId = items[itemsindex]["image"]["tile"]["1.78"]["program"]["default"]["masterId"];
				}

				if (masterId != "")
				{
					curlHandler->SaveImageFromURL(imageURL.asString(), "", ("textures/" + masterId.asString() + ".jpg").c_str());
				}
			}
		}
		else if (contentClass == "BecauseYouSet")
		{
			SetRefFromURL(containers[containerindex]["set"]["refId"].asString());
			GenerateImagesFromRef();
		}
	}
}

void DisneyInterpreter::GenerateImagesFromRef()
{
	Json::Value items = ref["data"]["CuratedSet"]["items"];
}

void DisneyInterpreter::DrawMainMenu(WindowController window)
{

}


/*



	   curlHandler->SaveImageFromURL(imageURL, "textures/1.jpg");*/
