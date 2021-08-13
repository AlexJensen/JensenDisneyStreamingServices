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
    // FreeType
    // --------
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
}

WindowController::~WindowController()
{
    delete spriteRenderer;
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
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    spriteRenderer = new SpriteRenderer(myShader);
}

void WindowController::LoadTextures()
{
    std::string path = "textures/";
    std::string type = ".jpg";

    namespace fs = boost::filesystem;

    fs::path apk_path(path);
    fs::recursive_directory_iterator end;

    for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
    {
        const fs::path cp = (*i);
        ResourceManager::LoadTexture((cp.string()).c_str(), false, cp.string().substr(path.length(),cp.string().length() - type.length() - path.length()));
    }

}

void WindowController::LoadFonts()
{
    std::string path = "fonts/";
    std::string type = ".ttf";

    namespace fs = boost::filesystem;

    fs::path apk_path(path);
    fs::recursive_directory_iterator end;

    for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
    {
        const fs::path cp = (*i);
        ResourceManager::LoadFont((cp.string()).c_str(), cp.string().substr(path.length(), cp.string().length() - type.length() - path.length()));
    }
}

void WindowController::Update(float dt)
{

}

void WindowController::ProcessInput(float dt)
{

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

