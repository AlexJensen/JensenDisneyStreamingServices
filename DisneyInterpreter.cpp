#include "DisneyInterpreter.h"

const std::string HOME_JSON("https://cd-static.bamgrid.com/dp-117731241344/home.json");
const std::string REF_JSON("https://cd-static.bamgrid.com/dp-117731241344/sets/");

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
		SetJsonValueFromRawJson(rawJson, &root);
	}
}

void DisneyInterpreter::SetRefFromURL(std::string url)
{
	const std::string rawJson = curlHandler->GetJSONFromURL(url);
	if (rawJson != "FAIL")
	{
		SetJsonValueFromRawJson(rawJson, &ref);
	}
}


void DisneyInterpreter::SetJsonValueFromRawJson(std::string rawJson, Json::Value* val)
{
	const auto rawJsonLength = static_cast<int>(rawJson.length());

	std::unique_ptr<std::string> httpData(new std::string());
	Json::CharReaderBuilder builder;

	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, val, &err)) {
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
				InterpretType(type, imageURL, items, itemsindex, masterId);

				if (masterId != "")
				{
					curlHandler->SaveImageFromURL(imageURL.asString(), "", ("textures/" + masterId.asString() + ".jpg").c_str());
				}
				else
				{
					std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
				}
			}
		}
		else if (contentClass == "BecauseYouSet" || contentClass == "TrendingSet" || contentClass == "PersonalizedCuratedSet")
		{
			SetRefFromURL(REF_JSON + containers[containerindex]["set"]["refId"].asString() + ".json");
			GenerateImagesFromRef();
		}
	}
}

void DisneyInterpreter::GenerateImagesFromRef()
{
	Json::Value items = ref["data"]["CuratedSet"]["items"];
	for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		InterpretType(type, imageURL, items, itemsindex, masterId);

		if (masterId != "")
		{
			curlHandler->SaveImageFromURL(imageURL.asString(), "", ("textures/" + masterId.asString() + ".jpg").c_str());
		}
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
}

void DisneyInterpreter::DrawMainMenu(WindowController* window)
{
	int xPos = 20, yPos = 20;

	const Json::Value containers = root["data"]["StandardCollection"]["containers"];
	Json::Value items;

	int debug = containers.size();
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
				InterpretType(type, imageURL, items, itemsindex, masterId);

				if (masterId != "")
				{
					window->RenderImage(masterId.asString(), xPos, yPos, 50, 28, 0);
					xPos += 60;
				}
				else
				{
					std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
				}
			}
			xPos = 20;
			yPos += 48;
		}
		else if (contentClass == "BecauseYouSet" || contentClass == "TrendingSet" || contentClass == "PersonalizedCuratedSet")
		{
			SetRefFromURL(REF_JSON + containers[containerindex]["set"]["refId"].asString() + ".json");
			DrawRefMenu(window, &xPos, &yPos);
		}
	}
}

void DisneyInterpreter::DrawRefMenu(WindowController* window, int *xPos, int *yPos)
{
	Json::Value items = ref["data"]["CuratedSet"]["items"];
	for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		InterpretType(type, imageURL, items, itemsindex, masterId);

		if (masterId != "")
		{
			window->RenderImage(masterId.asString(), *xPos, *yPos, 50, 28, 0);
			*xPos += 60;
		}	
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
	*xPos = 20;
	*yPos += 48;
}


void DisneyInterpreter::InterpretType(const std::string& type, Json::Value& imageURL, Json::Value& items, int itemsindex, Json::Value& masterId)
{
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
	else if (type == "StandardCollection")
	{
		imageURL = items[itemsindex]["image"]["tile"]["1.78"]["default"]["default"]["url"];
		masterId = items[itemsindex]["image"]["tile"]["1.78"]["default"]["default"]["masterId"];
	}
	else
	{
		std::cout << "ERROR::JSON: Encountered unexpected type: " << type << std::endl;
	}
}

