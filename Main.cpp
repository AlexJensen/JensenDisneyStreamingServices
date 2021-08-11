#include "Main.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

WindowController Window(SCR_WIDTH, SCR_HEIGHT);

int main()
{
    DisneyInterpreter* disneyInterpreter = new DisneyInterpreter();
    GLFWwindow* window = InitGLFW();
    if (window != NULL)
    {
        Window.Init();

        // deltaTime variables
        // -------------------
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        // Main update loop
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            Window.ProcessInput(deltaTime);
            Window.Update(deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //Start draw here
            disneyInterpreter->DrawMainMenu(&Window);

            glfwSwapBuffers(window);
        }

        ResourceManager::Clear();
        glfwTerminate();
    }
    delete disneyInterpreter;
    return 0;
}

GLFWwindow* InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Jensen Disney Streaming Services", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
         // --------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Window.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            Window.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

