#pragma once

void ProcessInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);