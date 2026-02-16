#include "Kappa/Application.h"

#include <cassert>
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

#include "Kappa/Logger.h"

namespace Kappa
{
    static Application *instance = nullptr;

    static void GLFWErrorCallback(int error, const char *description)
    {
        LOG_ERROR("[GLFW Error] ({}): {}.", error, description);
    }

    Application::Application(const ApplicationSpecification &spec) : specification(spec)
    {
        if (instance)
        {
            throw std::logic_error("Application already exists!");
        }

        instance = this;

        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        if (specification.windowSpecification.title.empty())
        {
            specification.windowSpecification.title = specification.name;
        }

        window = std::make_shared<Window>(specification.windowSpecification);
        window->Create();
    }

    Application::~Application()
    {
        window->Destroy();

        glfwTerminate();

        instance = nullptr;
    }

    void Application::Run()
    {
        isRunning = true;

        // Timestep clamping: prevent simulation instability from long frames
        constexpr float minTimestep = 0.001f; // 1ms minimum (1000 FPS cap)
        constexpr float maxTimestep = 0.1f;   // 100ms maximum (handle long frames/debugging)

        auto lastTime = GetTime();

        while (isRunning)
        {
            glfwPollEvents();

            if (window->ShouldClose())
            {
                Stop();
                break;
            }

            const auto currentTime = GetTime();
            const auto timestep = glm::clamp(currentTime - lastTime, minTimestep, maxTimestep);
            lastTime = currentTime;

            for (auto &layer : layerStack)
            {
                layer->OnUpdate(timestep);
            }

            BeginFrame();

            for (auto &layer : layerStack)
            {
                layer->OnRender();
            }

            EndFrame();

            window->Update();
        }
    }

    void Application::Stop()
    {
        isRunning = false;
    }

    glm::vec2 Application::GetFramebufferSize() const
    {
        return window->GetFrameBufferSize();
    }

    Window &Application::GetWindow()
    {
        assert(window);
        return *window;
    }

    const Window &Application::GetWindow() const
    {
        assert(window);
        return *window;
    }

    std::span<const std::unique_ptr<Layer>> Application::GetLayers() const
    {
        return layerStack;
    }

    Application &Application::Get()
    {
        assert(instance);
        return *instance;
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }

    EventBus &Application::GetEventBus()
    {
        return eventBus;
    }
} // namespace Kappa
