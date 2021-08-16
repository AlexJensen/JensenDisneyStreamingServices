#ifndef MAIN_H
#define MAIN_H

#define CURL_STATICLIB
#define GLFW_INCLUDE_GLCOREARB

//libaries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "WindowController.h"
#include "ResourceManager.h"
#include "DisneyInterpreter.h"

/// <summary>
/// Entry point for the program
/// </summary>
/// <returns>Final progam error state, 0 for success.</returns>
int main();

/// <summary>
/// Helper function to set up GLFW
/// </summary>
/// <returns>completed GLFWwondow object, or null if the initialization failed</returns>
GLFWwindow* InitGLFW();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

#endif