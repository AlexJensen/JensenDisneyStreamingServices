#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "font.h"

/// <summary>
/// Wrapper class used to draw textures onto the screen
/// </summary>
class SpriteRenderer
{
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    /// <summary>
    /// Draws an image to the screen
    /// </summary>
    /// <param name="texture">name of Image to draw</param>
    /// <param name="position">Top left corner of image to draw</param>
    /// <param name="size">default size of image to draw (before matrix translation)</param>
    /// <param name="rotate">rotation of image</param>
    /// <param name="color">color tint of image</param>
    void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    unsigned int VAO;

    /// <summary>
    /// Set up initial VAO and VBO
    /// </summary>
    void initRenderData();
};

#endif