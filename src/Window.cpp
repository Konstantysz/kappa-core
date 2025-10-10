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

    WindowState Window::GetState() const
    {
        WindowState state;

        if (!handle)
        {
            LOG_WARN("Window::GetState() called on invalid window handle");
            return state;
        }

        state.isMaximized = glfwGetWindowAttrib(handle, GLFW_MAXIMIZED) != 0;

        glfwGetWindowPos(handle, &state.posX, &state.posY);
        glfwGetWindowSize(handle, &state.width, &state.height);

        return state;
    }

    void Window::SetState(const WindowState &state)
    {
        if (!handle)
        {
            LOG_WARN("Window::SetState() called on invalid window handle");
            return;
        }

        SetSize(state.width, state.height);

        if (state.posX >= 0 && state.posY >= 0)
        {
            SetPosition(state.posX, state.posY);
        }
        else
        {
            Center();
        }

        if (state.isMaximized)
        {
            Maximize();
        }
        else
        {
            Restore();
        }

        LOG_INFO("Window state applied: {}x{} at ({},{}), maximized: {}", state.width, state.height, state.posX,
                 state.posY, state.isMaximized);
    }

    void Window::GetPosition(int &outX, int &outY) const
    {
        if (handle)
        {
            glfwGetWindowPos(handle, &outX, &outY);
        }
    }

    void Window::SetPosition(int x, int y)
    {
        if (handle)
        {
            glfwSetWindowPos(handle, x, y);
        }
    }

    void Window::GetSize(int &outWidth, int &outHeight) const
    {
        if (handle)
        {
            glfwGetWindowSize(handle, &outWidth, &outHeight);
        }
    }

    void Window::SetSize(int width, int height)
    {
        if (handle)
        {
            glfwSetWindowSize(handle, width, height);
        }
    }

    bool Window::IsMaximized() const
    {
        if (handle)
        {
            return glfwGetWindowAttrib(handle, GLFW_MAXIMIZED) != 0;
        }
        return false;
    }

    void Window::Maximize()
    {
        if (handle)
        {
            glfwMaximizeWindow(handle);
        }
    }

    void Window::Restore()
    {
        if (handle)
        {
            glfwRestoreWindow(handle);
        }
    }

    void Window::Center()
    {
        if (!handle)
        {
            return;
        }

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        if (!monitor)
        {
            LOG_WARN("Window::Center() failed: no primary monitor found");
            return;
        }

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        if (!mode)
        {
            LOG_WARN("Window::Center() failed: could not get video mode");
            return;
        }

        int windowWidth, windowHeight;
        glfwGetWindowSize(handle, &windowWidth, &windowHeight);

        int centerX = (mode->width - windowWidth) / 2;
        int centerY = (mode->height - windowHeight) / 2;

        glfwSetWindowPos(handle, centerX, centerY);
    }
} // namespace Kappa
