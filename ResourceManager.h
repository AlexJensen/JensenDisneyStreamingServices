#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>


#include "texture.h"
#include "shader.h"
#include "Font.h"
#include "stb_image.h"



/// <summary>
// A static singleton ResourceManager class that hosts several functions to save and load various resources.
/// </summary>
class ResourceManager
{
public: 
    // resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, Font> Fonts;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored shader
    static Shader GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // loads (and generates) a font from file
    static Font LoadFont(const char* file, std::string name, unsigned int fontSize);
    // retrieves a stored font
    static Font::Character GetCharacter(std::string font, char character);
    // de-allocates all loaded resources
    static void Clear();
private:
    static FT_Library ft;
    // private constructor
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
    // loads a single font from file
    static Font loadFontFromFile(const char* file, unsigned int fontSize);
};

#endif