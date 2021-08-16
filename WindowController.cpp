#include "WindowController.h"


SpriteRenderer *spriteRenderer;
TextRenderer* textRenderer;

const char* IMAGE_VERTEX_SHADER_PTH = "shaders/3.30coreImage.shader.vs";
const char* IMAGE_FRAGMENT_SHADER_PTH = "shaders/3.30coreImage.shader.fs";
const char* TEXT_VERTEX_SHADER_PTH = "shaders/3.30coreText.shader.vs";
const char* TEXT_FRAGMENT_SHADER_PTH = "shaders/3.30coreText.shader.fs";

WindowController::WindowController(unsigned int width, unsigned int height)
    : Keys(), Width(width), Height(height)
{

}

WindowController::~WindowController()
{
    delete spriteRenderer, textRenderer;
}

void WindowController::Init()
{   
    LoadShaders();
    LoadTextures();
    LoadFonts();  
}

void WindowController::LoadShaders()
{
    // load shaders
    ResourceManager::LoadShader(IMAGE_VERTEX_SHADER_PTH, IMAGE_FRAGMENT_SHADER_PTH, nullptr, "sprite");
    ResourceManager::LoadShader(TEXT_VERTEX_SHADER_PTH, TEXT_FRAGMENT_SHADER_PTH, nullptr, "text");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("text").Use().SetInteger("image", 0);
    ResourceManager::GetShader("text").SetMatrix4("projection", projection);

    // set render-specific controls
    Shader sprite, text;
    sprite = ResourceManager::GetShader("sprite");
    text = ResourceManager::GetShader("text");

    spriteRenderer = new SpriteRenderer(sprite);
    textRenderer = new TextRenderer(text, Width, Height);
}

void WindowController::LoadTextures()
{
    std::string path = "textures/";
    int typeLength = 4; // .png .jpg

    namespace fs = boost::filesystem;

    fs::path apk_path(path);
    fs::recursive_directory_iterator end;

    for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
    {
        const fs::path cp = (*i);
        std::string type = cp.string().substr(cp.string().length() - typeLength, typeLength);
        ResourceManager::LoadTexture((cp.string()).c_str(), type == ".png", cp.string().substr(path.length(), cp.string().length() - type.length() - path.length()));
    }
}

void WindowController::LoadFonts()
{
    std::string path = "fonts/";
    std::string type = ".ttf";

    namespace fs = boost::filesystem;

    fs::path apk_path(path);
    fs::recursive_directory_iterator end;
    const int fontSizes[7] = {12, 18, 24, 36, 48, 60, 72};
    const char* fontSizeStr[7] = { "12", "18", "24", "36", "48", "60", "72" };

    for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
    {
        const fs::path cp = (*i);
        for (unsigned int i = 0; i < 7; i++)
        {
            ResourceManager::LoadFont((cp.string()).c_str(), cp.string().substr(path.length(), cp.string().length() - type.length() - path.length()) + fontSizeStr[i], fontSizes[i]);
        }
    }
}

void WindowController::RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate)
{
    RenderImage(textureName, posx, posy, sizex, sizey, rotate, 1.0f, 1.0f, 1.0f);
}

void WindowController::RenderImage(std::string textureName, float posx, float posy, float sizex, float sizey, float rotate,
    float r, float g, float b)
{
    Texture2D myTexture;
    myTexture = ResourceManager::GetTexture(textureName);
    spriteRenderer->DrawSprite(myTexture, glm::vec2(posx, posy), glm::vec2(sizex, sizey), rotate, glm::vec3(r, g, b));
}

// render line of text
// -------------------
void WindowController::RenderText(std::string text, std::string font, float x, float y, float scale, glm::vec3 color)
{
    textRenderer->SetFont(font);
    textRenderer->RenderText(text, x, y, scale, color);
}

