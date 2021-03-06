#include "TextRenderer.h"


TextRenderer::TextRenderer(Shader& shader, float width, float height)
{
    // load and configure shader
    this->shader = shader;
    this->shader.SetMatrix4("projection", glm::ortho(0.0f, width, height, 0.0f), true);
    this->shader.SetInteger("text", 0);

    initRenderData();
}

TextRenderer::~TextRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
}

void TextRenderer::SetFont(std::string font)
{
    this->font = font.c_str();
}

// render line of text
// -------------------
void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    this->shader.Use();
    this->shader.SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // iterate through all characters
    std::string::const_iterator c;
    if (font == "")
    {
        std::cout << "ERROR::TEXTRENDERER: Font is not set! A font must be defined before text can be drawn." << std::endl;
        return;
    }
    for (c = text.begin(); c != text.end(); c++)
    {
        Font::Character ch = ResourceManager::GetCharacter(font, *c);

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (ResourceManager::GetCharacter(font, 'H').Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character{}
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::initRenderData()
{
    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
