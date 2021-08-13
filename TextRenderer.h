#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class TextRenderer
{
public:
    TextRenderer(Shader& shader);
    ~TextRenderer();

    void SetFont(std::string font);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    Shader shader;
    unsigned int VAO = 0, VBO = 0;
    const char* font;

    void initRenderData();
};

#endif