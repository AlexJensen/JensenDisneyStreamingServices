
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Represents the current state of the game
enum WindowState {
    WINDOW_MAIN
};
class WindowController
{
public:
    WindowState State;
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
    void Render();
};

#endif