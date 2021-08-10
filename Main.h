#ifndef MAIN_H
#define MAIN_H

//libaries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "WindowController.h"
#include "ResourceManager.h"
#include "DisneyInterpreter.h"

int main();
GLFWwindow* InitGLFW();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

#endif