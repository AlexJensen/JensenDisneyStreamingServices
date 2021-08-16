#ifndef FONT_H
#define FONT_H

#include <map>
#include <iostream>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

/// <summary>
/// Stores a map of characters for a specified font of a specified size
/// </summary>
class Font
{
public:
    //character structs
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    
    std::map<char, Character> Characters;

    // Loads characters into the character map for a font stored in the fonts folder with a specifed font size
    void Generate(const char* file, unsigned int fontSize);
};

#endif
