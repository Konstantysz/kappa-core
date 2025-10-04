#include "Window.h"

#include "Logger.h"

namespace Kappa
{
    Window::Window(const WindowSpecification &spec) : specification(spec), handle(nullptr)
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        // Try OpenGL 4.5 first (Windows native), fall back to 4.2 (WSL/WSLg max)
        const int preferredMajor = 4;
        const int preferredMinor = 5;
        const int fallbackMinor = 2;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, preferredMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, preferredMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        handle =
            glfwCreateWindow(specification.width, specification.height, specification.title.c_str(), nullptr, nullptr);

        // If 4.5 fails, try 4.2 (WSL compatibility)
        if (!handle)
        {
            LOG_WARN("Failed to create OpenGL {}.{} context, trying {}.{}",
                     preferredMajor, preferredMinor, preferredMajor, fallbackMinor);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, preferredMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, fallbackMinor);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

            handle = glfwCreateWindow(specification.width, specification.height,
                                     specification.title.c_str(), nullptr, nullptr);
        }

        if (!handle)
        {
            LOG_ERROR("Failed to create GLFW window with OpenGL {}.{} or {}.{}",
                     preferredMajor, preferredMinor, preferredMajor, fallbackMinor);
            assert(false);
        }

        glfwMakeContextCurrent(handle);
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

        // Log the actual OpenGL version we got
        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        LOG_INFO("OpenGL context created: {}", version ? version : "unknown");

        glfwSwapInterval(specification.vSync ? 1 : 0);
    }

    void Window::Destroy()
    {
        if (handle)
        {
            glfwDestroyWindow(handle);
        }

        handle = nullptr;
    }

    void Window::Update()
    {
        glfwSwapBuffers(handle);
    }

    glm::vec2 Window::GetFrameBufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(handle, &width, &height);

        return { width, height };
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(handle);
    }

    GLFWwindow *Window::GetHandle() const
    {
        return handle;
    }
} // namespace Kappa
