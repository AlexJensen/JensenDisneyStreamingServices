#include "WindowController.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

// Game-related State data
SpriteRenderer *Renderer;

const char* VERTEX_SHADER_PTH = "shaders/3.30core.shader.vs";
const char* FRAGMENT_SHADER_PTH = "shaders/3.30core.shader.fs";

WindowController::WindowController(unsigned int width, unsigned int height)
    : State(WINDOW_MAIN), Keys(), Width(width), Height(height)
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
    ResourceManager::LoadTexture("1.jpg", false, "1");
}

void WindowController::Update(float dt)
{

}

void WindowController::ProcessInput(float dt)
{

}

void WindowController::Render()
{
    Texture2D myTexture;
    myTexture = ResourceManager::GetTexture("1");
    Renderer->DrawSprite(myTexture, glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myTexture, glm::vec2(180.0f, 50.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myTexture, glm::vec2(680.0f, 80.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myTexture, glm::vec2(180.0f, 50.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myTexture, glm::vec2(180.0f, 200.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myTexture, glm::vec2(180.0f, 50.0f), glm::vec2(100.0f, 125.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}