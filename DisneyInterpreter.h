#ifndef DISNEY_INTERPRETER
#define DISNEY_INTERPRETER

#include <string>
#include <json/json.h>
#include <iostream>

#include "CURLHandler.h"
#include "WindowController.h"
#include "DisneyMenuItemTypes.h"

/// <summary>
/// Interpreter class used to convert JSON API from URls into content specific information to render to the window.
/// </summary>
class DisneyInterpreter
{
public:
	DisneyInterpreter();
	~DisneyInterpreter();

	/// <summary>
	/// Draws the main menu grid of menu buttons
	/// </summary>
	/// <param name="window"Used to draw to the actual window</param>
	void DrawMainMenu(WindowController* window);

	/// <summary>
	/// Registers user input
	/// </summary>
	/// <param name="window">Used to draw to the actual window</param>
	void ProcessInput(WindowController* window);

	/// <summary>
	/// Currently doesn't do anything, but animation lerps would likely be added here
	/// </summary>
	/// <param name="window">Used to draw to the actual window</param>
	/// <param name="deltaTime">Used to update animation lerps</param>
	void Update(WindowController* window, float deltaTime);

	

private:
	// link to CURL library functions
	CURLHandler* curlHandler;
	// storage for root home JSON file
	Json::Value root;
	// map to store all ref JSON files
	std::map<std::string, Json::Value> refs;
	// storage for any error messages during JSON interpretation
	JSONCPP_STRING err;
	// template struct used to define specific UI features of the Main Menu
	mainMenuButtonGrid mbTemp;
	// tracks the total number of rows and columns in the main menu JSON grid
	int totalContainers, itemsPerContainer;

	/// <summary>
	/// Sets the root JSON object to the one obtained from the input URL.
	/// </summary>
	/// <param name="url">URL to recieve root from</param>
	void SetHomeFromURL(std::string url);

	/// <summary>
	/// Creates a new JSON object ref from the input URL with the given key and adds it to the Ref map
	/// </summary>
	/// <param name="url">URL to revieve ref from</param>
	/// <param name="key">Name of key to add to Ref map</param>
	void SetRefFromURL(std::string url, std::string key);

	/// <summary>
	/// Loads all the Images from the root JSON object and all associated ref JSON objects into the Textures folder
	/// </summary>
	void GenerateImagesFromHome();

	/// <summary>
	/// Loads all the Images from a ref JSON object into the Textures folder
	/// </summary>
	/// <param name="refType"></param>
	/// <param name="key"></param>
	void GenerateImagesFromRef(std::string refType, std::string key);

	/// <summary>
	/// Creates a new image file if a file with the same name in the Textures folder does not already exist
	/// </summary>
	/// <param name="masterId">Unique ID of image to store in map</param>
	/// <param name="imageURL">URL that points to data for image</param>
	void SaveImageIfNotExist(Json::Value& masterId, Json::Value& imageURL);

	/// <summary>
	/// Checks if a file exists
	/// </summary>
	/// <param name="file">File to check</param>
	/// <returns>if file exists</returns>
	bool fileExists(const std::string& file);

	/// <summary>
	/// Draws a row of menu buttons given data from a JSON ref file
	/// </summary>
	/// <param name="window">Used to draw to the actual window</param>
	/// <param name="refType">Name of RefType JSON attribute</param>
	/// <param name="refId">Unique ID of Ref to store in map</param>
	/// <param name="containerIndex">Current y index of main menu grid used for selection</param>
	void DrawRefMenu(WindowController* window, std::string refType, std::string refId, int containerIndex);

	/// <summary>
	/// Converts raw JSON from a string into a JSON object
	/// </summary>
	/// <param name="rawJson">raw JSON</param>
	/// <param name="val">JSON object passed in will return with raw JSON interpreted onto it</param>
	void SetJsonValueFromRawJson(std::string rawJson, Json::Value* val);

	/// <summary>
	/// Some JSON refType tags do not automatically line up with their associated refType in the ref JSON file. This is used as a translation layer to convert unexpected menu types into recognized menu types
	/// </summary>
	/// <param name="refType">RefType to interpret</param>
	void InterpretRefType(std::string& refType);

	/// <summary>
	/// Different types have different JSON paths. This is used to correctly path the image URL based on the type given
	/// </summary>
	/// <param name="type">The type of JSOM element</param>
	/// <param name="imageURL">Will populate with the URL of the image to download</param>
	/// <param name="items">items JSON object to pull from</param>
	/// <param name="itemsindex">current x index on the grid</param>
	/// <param name="masterId">unique ID to add to the image map</param>
	void GetImageFromType(const std::string& type, Json::Value& imageURL, Json::Value& items, int itemsindex, Json::Value& masterId);

	/// <summary>
	/// Ensures the grid is always drawn inside the window for every user selection on the x axis
	/// </summary>
	void BoundScreenWidth();
	/// <summary>
	/// Ensures the grid is always drawn inside the window for every user selection on the y axis
	/// </summary>
	void BoundScreenHeight();
};

#endif

