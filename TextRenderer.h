#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "texture.h"
#include "shader.h"
#include "ResourceManager.h"

/// <summary>
/// Wrapper class used to draw text onto the screen
/// </summary>
class TextRenderer
{
public:
    TextRenderer(Shader& shader, float width, float height);
    ~TextRenderer();

    /// <summary>
    /// Changes the current font to draw. Must be called at least once before RenderText
    /// </summary>
    /// <param name="font">Font type to draw</param>
    void SetFont(std::string font);

    /// <summary>
    /// Draws text to the screen
    /// </summary>
    /// <param name="text">Text to draw</param>
    /// <param name="x">Initial x position</param>
    /// <param name="y">Initial y position</param>
    /// <param name="scale">scaling factor</param>
    /// <param name="color">color tint</param>
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    Shader shader;
    unsigned int VAO = 0, VBO = 0;
    std::string font = "";

    void initRenderData();
};

#endif