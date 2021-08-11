#include "WindowController.h"

SpriteRenderer *Renderer;

const char* VERTEX_SHADER_PTH = "shaders/3.30core.shader.vs";
const char* FRAGMENT_SHADER_PTH = "shaders/3.30core.shader.fs";

WindowController::WindowController(unsigned int width, unsigned int height)
    : Keys(), Width(width), Height(height)
{

}

WindowController::~WindowController()
{
    delete Renderer;
}

void WindowController::Init()
{
    // load shaders
    ResourceManager::LoadShader(VERTEX_SHADER_PTH, FRAGMENT_SHADER_PTH, nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    // load textures
    LoadTextures();
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
    Renderer->DrawSprite(myTexture, glm::vec2(posx, posy), glm::vec2(sizex, sizey), rotate, glm::vec3(r, g, b));
}

