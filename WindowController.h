
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include "SpriteRenderer.h"

// Represents the current state of the game

class WindowController
{
public:
    bool Keys[1024];
    unsigned int Width, Height;

    // constructor/destructor
    WindowController(unsigned int width, unsigned int height);
    ~WindowController();

    // initialize window state (load all shaders/textures/levels)
    void Init();

    // main render loop
    void ProcessInput(float dt);
    void Update(float dt);

    // draw function wrapper classes
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate);
    void RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate,
        float r, float g, float b);
};

#endif