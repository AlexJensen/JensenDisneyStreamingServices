
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>


#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

// Represents the current state of the game

class WindowController
{
public:
    bool Keys[1024];
    unsigned int Width, Height;

    // constructor/destructor
    WindowController(unsigned int width, unsigned int height);
    ~WindowController();

    // initialize window state (load all shaders/textures/fonts)
    void Init();

    // main render loop
    void ProcessInput(float dt);
    void Update(float dt);

    // draw function wrapper classes
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate);
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate,
        float r, float g, float b);
    void RenderText(std::string text, std::string font, float x, float y, float scale, glm::vec3 color);

private:
    void LoadShaders();
    void LoadTextures();
    void LoadFonts();


    FT_Library ft;
};

#endif