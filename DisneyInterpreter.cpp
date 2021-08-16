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

void DisneyInterpreter::SetRefFromURL(std::string url, std::string key)
{
	Json::Value ref;
	const std::string rawJson = curlHandler->GetJSONFromURL(url);
	if (rawJson != "FAIL")
	{
		SetJsonValueFromRawJson(rawJson, &ref);
		refs[key] = ref;
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
	totalContainers = containers.size();
	for (unsigned int containerindex = 0; containerindex < containers.size(); ++containerindex)
	{
		const std::string type = containers[containerindex]["set"]["type"].asString();
		if (type == "SetRef")
		{
			const std::string refType = containers[containerindex]["set"]["refType"].asString();
			const std::string refId = containers[containerindex]["set"]["refId"].asString();
			SetRefFromURL(REF_JSON + refId + ".json", refId);
			GenerateImagesFromRef(refType, refId);
		}
		else if (type == "CuratedSet")
		{
			items = containers[containerindex]["set"]["items"];
			itemsPerContainer = items.size();
			for (unsigned int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
			{
				const std::string type = items[itemsindex]["type"].asString();
				Json::Value imageURL, masterId;

				GetImageFromType(type, imageURL, items, itemsindex, masterId);

				if (masterId.asString() != "")
				{
					SaveImageIfNotExist(masterId, imageURL);
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

void DisneyInterpreter::GenerateImagesFromRef(std::string refType, std::string key)
{
	Json::Value ref = refs[key];

	InterpretRefType(refType);
	Json::Value items = ref["data"][refType]["items"];
	itemsPerContainer = items.size();
	if (items.size() == 0)
	{
		std::cout << "ERROR::JSON: Encountered empty item list from generate: " << refType << std::endl;
		for (auto const& id : ref["data"].getMemberNames()) {
			std::cout << id << std::endl;
		}
	}
	for (unsigned int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		GetImageFromType(type, imageURL, items, itemsindex, masterId);

		if (masterId.asString() != "")
		{
			SaveImageIfNotExist(masterId, imageURL);
		}
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
}

void DisneyInterpreter::SaveImageIfNotExist(Json::Value& masterId, Json::Value& imageURL)
{
	namespace fs = boost::filesystem;
	if (!fileExists("textures/" + masterId.asString() + ".jpg"))
	{
		curlHandler->SaveImageFromURL(imageURL.asString(), ("textures/" + masterId.asString() + ".jpg").c_str());
	}
}

// true if file exists
bool DisneyInterpreter::fileExists(const std::string& file) {
	struct stat buf;
	return (stat(file.c_str(), &buf) == 0);
}

void DisneyInterpreter::DrawMainMenu(WindowController* window)
{
	BoundScreenHeight();
	const Json::Value containers = root["data"]["StandardCollection"]["containers"];
	Json::Value items;

	for (unsigned int containerIndex = 0; containerIndex < containers.size(); ++containerIndex)
	{
		BoundScreenWidth();
		if (mbTemp.menuY != containerIndex)
		{
			//mbTemp.xPos = mbTemp.xstart;
		}

		const std::string type = containers[containerIndex]["set"]["type"].asString();
		if (type == "SetRef")
		{
			const std::string refType = containers[containerIndex]["set"]["refType"].asString();
			const std::string refId = containers[containerIndex]["set"]["refId"].asString();
			DrawRefMenu(window, refType, refId, containerIndex);
		}
		else if (type == "CuratedSet")
		{
			items = containers[containerIndex]["set"]["items"];
			for (unsigned int itemsIndex = 0; itemsIndex < items.size(); ++itemsIndex)
			{
				const std::string type = items[itemsIndex]["type"].asString();
				Json::Value imageURL, masterId;
				GetImageFromType(type, imageURL, items, itemsIndex, masterId);

				if (masterId != "")
				{
					window->RenderImage(masterId.asString(), mbTemp.xPos, mbTemp.yPos, mbTemp.width, mbTemp.height, 0);
					if (containerIndex == mbTemp.menuY && itemsIndex == mbTemp.menuX)
					{
						window->RenderImage("border", mbTemp.xPos - mbTemp.xgapselect, mbTemp.yPos - mbTemp.ygapselect, 
							mbTemp.width + (mbTemp.xgapselect *2), mbTemp.height + (mbTemp.ygapselect * 2), 0);
					}
					mbTemp.xPos += mbTemp.width + mbTemp.xgap;
				}
				else
				{
					std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
				}
			}


			mbTemp.yPos += mbTemp.height + mbTemp.ygap;
			const std::string title = containers[containerIndex]["set"]["text"]["title"]["full"]["set"]["default"]["content"].asString();
			window->RenderText(title, "Mulish24", 20, mbTemp.yPos, 1, glm::vec3(1, 1, 1));
			mbTemp.yPos += mbTemp.ygapheader;
		}
		else
		{
			std::cout << "ERROR::JSON: Encountered unexpected type in main: " << type << std::endl;
		}
	}
}

void DisneyInterpreter::DrawRefMenu(WindowController* window, std::string refType, std::string refId, int containerIndex)
{
	Json::Value ref = refs[refId];
	InterpretRefType(refType);
	Json::Value items = ref["data"][refType]["items"];
	if (items.size() == 0)
	{
		std::cout << "ERROR::JSON: Encountered empty item list from render: " << refType << std::endl;
		for (auto const& id : ref["data"].getMemberNames()) {
			std::cout << id << std::endl;
		}
	}
	for (unsigned int itemsindex = 0; itemsindex < items.size(); ++itemsindex)
	{
		const std::string type = items[itemsindex]["type"].asString();
		Json::Value imageURL, masterId;
		GetImageFromType(type, imageURL, items, itemsindex, masterId);

		if (masterId.asString() != "")
		{
			window->RenderImage(masterId.asString(), mbTemp.xPos, mbTemp.yPos, mbTemp.width, mbTemp.height, 0);
			if (containerIndex == mbTemp.menuY && itemsindex == mbTemp.menuX)
			{
				window->RenderImage("border", mbTemp.xPos - mbTemp.xgapselect, mbTemp.yPos - mbTemp.ygapselect,
					mbTemp.width + (mbTemp.xgapselect * 2), mbTemp.height + (mbTemp.ygapselect * 2), 0);
			}
			mbTemp.xPos += mbTemp.width + mbTemp.xgap;
		}
		else
		{
			std::cout << "ERROR::JSON: Failed to retrieve MasterId: " << std::endl;
		}
	}
	mbTemp.xPos = mbTemp.xstart;
	mbTemp.yPos += mbTemp.height + mbTemp.ygap;
	const std::string title = ref["data"][refType]["text"]["title"]["full"]["set"]["default"]["content"].asString();
	window->RenderText(title, "Mulish24", 20, mbTemp.yPos, 1, glm::vec3(1, 1, 1));
	mbTemp.yPos += mbTemp.ygapheader;
}

void DisneyInterpreter::BoundScreenWidth()
{
	float menuXadj = mbTemp.menuX;
	if (mbTemp.menuX < mbTemp.xBorderL)
	{
		menuXadj = mbTemp.xBorderL;
	}
	else if (mbTemp.menuX >= itemsPerContainer - mbTemp.xBorderR + 1)
	{
		menuXadj = itemsPerContainer - mbTemp.xBorderR + 1;
	}
	mbTemp.xPos = mbTemp.xstart - ((menuXadj - mbTemp.xBorderL) * (mbTemp.width + mbTemp.xgap));
}

void DisneyInterpreter::BoundScreenHeight()
{
	float menuYadj = mbTemp.menuY;
	if (mbTemp.menuY < mbTemp.yBorderU)
	{
		menuYadj = mbTemp.yBorderU;
	}
	else if(mbTemp.menuY >= itemsPerContainer - mbTemp.yBorderD + 1)
	{
		menuYadj = itemsPerContainer - mbTemp.yBorder + 1;
	}
	mbTemp.yPos = mbTemp.ystart - ((menuYadj - mbTemp.yBorderU) * (mbTemp.height + mbTemp.ygap + mbTemp.ygapheader));
}

void DisneyInterpreter::ProcessInput(WindowController* window)
{
	if (window->Keys[GLFW_KEY_UP] && !window->KeysProcessed[GLFW_KEY_UP] && mbTemp.menuY > 0)
	{
		mbTemp.menuY -= 1;
		mbTemp.yBorderU = 1;
		mbTemp.yBorderD = mbTemp.yBorder;
		//mbTemp.menuX = 0;
		window->KeysProcessed[GLFW_KEY_UP] = true;
	}
	if (window->Keys[GLFW_KEY_DOWN] && !window->KeysProcessed[GLFW_KEY_DOWN] && mbTemp.menuY < totalContainers - 1)
	{
		mbTemp.menuY += 1;
		mbTemp.yBorderU = mbTemp.yBorder;
		mbTemp.yBorderD = 1;
		//mbTemp.menuX = 0;
		window->KeysProcessed[GLFW_KEY_DOWN] = true;
	}
	if (window->Keys[GLFW_KEY_LEFT] && !window->KeysProcessed[GLFW_KEY_LEFT] && mbTemp.menuX > 0)
	{
		mbTemp.menuX -= 1;
		mbTemp.xBorderL = 1;
		mbTemp.xBorderR = mbTemp.xBorder;
		window->KeysProcessed[GLFW_KEY_LEFT] = true;
	}
	if (window->Keys[GLFW_KEY_RIGHT] && !window->KeysProcessed[GLFW_KEY_RIGHT] && mbTemp.menuX < itemsPerContainer - 1)
	{
		mbTemp.menuX += 1;
		mbTemp.xBorderL = mbTemp.xBorder;
		mbTemp.xBorderR = 1;
		window->KeysProcessed[GLFW_KEY_RIGHT] = true;
	}
}

void DisneyInterpreter::Update(WindowController* window, float deltaTime)
{
}



void DisneyInterpreter::InterpretRefType(std::string& refType)
{
	if (refType == "BecauseYouSet")
	{
		refType = "CuratedSet";
	}
}

void DisneyInterpreter::GetImageFromType(const std::string& type, Json::Value& imageURL, Json::Value& items, int itemsindex, Json::Value& masterId)
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

