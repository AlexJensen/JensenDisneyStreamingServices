#ifndef FONT_H
#define FONT_H

#include <map>
#include <iostream>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font
{
public:
    //font structs
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;

    Font Generate(const char* file);
};

#endif
