#ifndef MAIN_H
#define MAIN_H


int main();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);

#endif