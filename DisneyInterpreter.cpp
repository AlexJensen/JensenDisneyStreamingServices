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
		const std::string type = containers[containerindex]["set"]["type"].asString();
		if (type == "SetRef")
		{
			const std::string refType = containers[containerindex]["set"]["refType"].asString();
			SetRefFromURL(REF_JSON + containers[containerindex]["set"]["refId"].asString() + ".json");
			GenerateImagesFromRef(refType);
		}
		else if (type == "CuratedSet")
		{
			items = containers[containerindex]["set"]["items"];
			for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
			{
				const std::string type = items[itemsindex]["type"].asString();
				Json::Value imageURL, masterId;

				InterpretType(type, imageURL, items, itemsindex, masterId);

				if (masterId.asString() != "")
				{
					SaveIfFileDoesNotExist(masterId, imageURL);
				}
				else
				{
					std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
				}
			}
		}
		else
		{
			std::cout << "ERROR::JSON: Encountered unexpected type: " << type << std::endl;
		}
	}
}

void DisneyInterpreter::GenerateImagesFromRef(std::string refType)
{
	InterpretRefType(refType);
	Json::Value items = ref["data"][refType]["items"];
	if (items.size() == 0)
	{
		std::cout << "ERROR::JSON: Encountered empty item list from generate: " << refType << std::endl;
		for (auto const& id : ref["data"].getMemberNames()) {
			std::cout << id << std::endl;
		}
	}
	for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		InterpretType(type, imageURL, items, itemsindex, masterId);

		if (masterId.asString() != "")
		{
			SaveIfFileDoesNotExist(masterId, imageURL);
		}
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
}

void DisneyInterpreter::SaveIfFileDoesNotExist(Json::Value& masterId, Json::Value& imageURL)
{
	namespace fs = boost::filesystem;
	if (!fileExists("textures/" + masterId.asString() + ".jpg"))
	{
		curlHandler->SaveImageFromURL(imageURL.asString(), "", ("textures/" + masterId.asString() + ".jpg").c_str());
	}
}

// true if file exists
bool DisneyInterpreter::fileExists(const std::string& file) {
	struct stat buf;
	return (stat(file.c_str(), &buf) == 0);
}

void DisneyInterpreter::DrawMainMenu(WindowController* window)
{
	mainMenuButton mbTemp;

	const Json::Value containers = root["data"]["StandardCollection"]["containers"];
	Json::Value items;

	int debug = containers.size();
	for (int containerindex = 0; containerindex < containers.size(); ++containerindex)
	{
		const std::string type = containers[containerindex]["set"]["type"].asString();
		if (type == "SetRef")
		{
			const std::string refType = containers[containerindex]["set"]["refType"].asString();
			SetRefFromURL(REF_JSON + containers[containerindex]["set"]["refId"].asString() + ".json");
			DrawRefMenu(window, refType, &mbTemp);
		}
		else if (type == "CuratedSet")
		{
			items = containers[containerindex]["set"]["items"];
			for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
			{
				const std::string type = items[itemsindex]["type"].asString();
				Json::Value imageURL, masterId;
				InterpretType(type, imageURL, items, itemsindex, masterId);

				if (masterId != "")
				{
					window->RenderImage(masterId.asString(), mbTemp.xPos, mbTemp.yPos, mbTemp.width, mbTemp.height, 0);
					mbTemp.xPos += mbTemp.width + mbTemp.xgap;
				}
				else
				{
					std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
				}
			}
			mbTemp.xPos = mbTemp.xstart;
			mbTemp.yPos += mbTemp.height + mbTemp.ygap;
		}
		else
		{
			std::cout << "ERROR::JSON: Encountered unexpected type in main: " << type << std::endl;
		}
	}
}

void DisneyInterpreter::DrawRefMenu(WindowController* window, std::string refType, mainMenuButton* mbTemp)
{
	InterpretRefType(refType);
	Json::Value items = ref["data"][refType]["items"];
	if (items.size() == 0)
	{
		std::cout << "ERROR::JSON: Encountered empty item list from render: " << refType << std::endl;
		for (auto const& id : ref["data"].getMemberNames()) {
			std::cout << id << std::endl;
		}
	}
	for (int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		InterpretType(type, imageURL, items, itemsindex, masterId);

		if (masterId.asString() != "")
		{
			window->RenderImage(masterId.asString(), mbTemp->xPos, mbTemp->yPos, mbTemp->width, mbTemp->height, 0);
			mbTemp->xPos += mbTemp->width + mbTemp->xgap;
		}	
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
	mbTemp->xPos = mbTemp->xstart;
	mbTemp->yPos += mbTemp->height + mbTemp->ygap;
}

void DisneyInterpreter::InterpretRefType(std::string& refType)
{
	if (refType == "BecauseYouSet")
	{
		refType = "CuratedSet";
	}
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
		std::cout << "ERROR::JSON: Encountered unexpected type during InterpretTypes: " << type << std::endl;
	}
}

