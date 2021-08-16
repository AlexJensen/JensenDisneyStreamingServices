
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>


#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

/// <summary>
/// Stores information needed to create and draw to the screen, load resources, register user input, and other various functions to perform common drawing commands.
/// </summary>
class WindowController
{
public:
    bool Keys[1024], KeysProcessed[1024];
    unsigned int Width, Height;

    // constructor/destructor
    WindowController(unsigned int width, unsigned int height);
    ~WindowController();

    // initialize window state (load all shaders/textures/fonts)
    void Init();

    // draw function wrapper classes
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate);
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate,
        float r, float g, float b);
    void RenderText(std::string text, std::string font, float x, float y, float scale, glm::vec3 color);

private:
    void LoadShaders();
    void LoadTextures();
    void LoadFonts();
};

#endif