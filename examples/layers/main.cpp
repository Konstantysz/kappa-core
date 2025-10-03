#include "Core/Application.h"
#include "Core/EventBus.h"
#include "Core/Layer.h"
#include "Core/Logger.h"
#include <GLFW/glfw3.h>

using namespace Kappa;

// Example: Background layer that renders a colored background
class BackgroundLayer : public Layer
{
public:
    void OnAttach() override
    {
        LOG_INFO("BackgroundLayer attached");
    }

    void OnUpdate(float deltaTime) override
    {
        // Background doesn't need updates
    }

    void OnRender() override
    {
        // Render dark blue background
        glClearColor(0.1f, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

// Example: Game logic layer
class GameLayer : public Layer
{
private:
    float time = 0.0f;
    float colorPhase = 0.0f;

public:
    void OnAttach() override
    {
        LOG_INFO("GameLayer attached");
    }

    void OnUpdate(float deltaTime) override
    {
        time += deltaTime;
        colorPhase += deltaTime * 2.0f; // Cycle through colors

        if (static_cast<int>(time) % 5 == 0 && static_cast<int>(time * 10) % 10 == 0)
        {
            LOG_DEBUG("GameLayer running for {:.1f} seconds", time);
        }
    }

    void OnRender() override
    {
        // Render a pulsing quad in the center
        float red = (std::sin(colorPhase) + 1.0f) * 0.5f;
        float green = (std::sin(colorPhase + 2.0f) + 1.0f) * 0.5f;
        float blue = (std::sin(colorPhase + 4.0f) + 1.0f) * 0.5f;

        glColor3f(red, green, blue);
        glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
    }

    void OnEvent(const Event &event) override
    {
        if (auto *keyEvent = dynamic_cast<const KeyPressedEvent *>(&event))
        {
            LOG_INFO("GameLayer received key press: {}", keyEvent->key);

            if (keyEvent->key == GLFW_KEY_R)
            {
                LOG_INFO("Resetting color phase");
                colorPhase = 0.0f;
            }
        }
    }
};

// Example: UI overlay layer
class UILayer : public Layer
{
private:
    int frameCount = 0;
    float fps = 0.0f;
    float fpsTimer = 0.0f;

public:
    void OnAttach() override
    {
        LOG_INFO("UILayer attached");
    }

    void OnUpdate(float deltaTime) override
    {
        frameCount++;
        fpsTimer += deltaTime;

        if (fpsTimer >= 1.0f)
        {
            fps = frameCount / fpsTimer;
            frameCount = 0;
            fpsTimer = 0.0f;
        }
    }

    void OnRender() override
    {
        // Render FPS counter in top-left (simple text rendering would go here)
        // For this example, we'll just render a small indicator quad
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-0.95f, 0.90f);
        glVertex2f(-0.85f, 0.90f);
        glVertex2f(-0.85f, 0.95f);
        glVertex2f(-0.95f, 0.95f);
        glEnd();
    }

    void OnEvent(const Event &event) override
    {
        if (auto *keyEvent = dynamic_cast<const KeyPressedEvent *>(&event))
        {
            if (keyEvent->key == GLFW_KEY_F)
            {
                LOG_INFO("Current FPS: {:.1f}", fps);
            }
        }
    }
};

int main()
{
    Logger::Init();

    ApplicationSpecification spec;
    spec.name = "kappa-core Layers Example";
    spec.width = 1280;
    spec.height = 720;
    spec.vsync = true;

    Application app(spec);

    // Add layers in order (bottom to top)
    app.PushLayer(std::make_shared<BackgroundLayer>());
    app.PushLayer(std::make_shared<GameLayer>());
    app.PushLayer(std::make_shared<UILayer>());

    LOG_INFO("Starting application with 3 layers");
    LOG_INFO("Press 'R' to reset color animation");
    LOG_INFO("Press 'F' to show FPS in console");

    app.Run();

    return 0;
}
