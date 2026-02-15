#include "Kappa/Application.h"
#include "Kappa/Layer.h"
#include "Kappa/Logger.h"

#include <GLFW/glfw3.h>

using namespace Kappa;

// Example: Background layer
class BackgroundLayer : public Layer
{
public:
    void OnRender() override
    {
        glClearColor(0.1f, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

// Example: Game logic layer
class GameLayer : public Layer
{
private:
    float time = 0.0f;

public:
    void OnUpdate(float deltaTime) override
    {
        time += deltaTime;
    }

    void OnRender() override
    {
        // Render pulsing quad
        float pulse = (std::sin(time * 2.0f) + 1.0f) * 0.5f;
        glColor3f(pulse, 0.5f, 1.0f - pulse);
        glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
    }
};

// Example: UI overlay layer
class UILayer : public Layer
{
public:
    void OnRender() override
    {
        // Render UI indicator
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-0.95f, 0.90f);
        glVertex2f(-0.85f, 0.90f);
        glVertex2f(-0.85f, 0.95f);
        glVertex2f(-0.95f, 0.95f);
        glEnd();
    }
};

class LayersApp : public Application
{
public:
    LayersApp() : Application(GetSpec())
    {
        PushLayer<BackgroundLayer>();
        PushLayer<GameLayer>();
        PushLayer<UILayer>();
    }

private:
    static ApplicationSpecification GetSpec()
    {
        ApplicationSpecification spec;
        spec.name = "kappa-core Layers Example";
        spec.windowSpecification.width = 1280;
        spec.windowSpecification.height = 720;
        return spec;
    }
};

int main()
{
    LOG_INFO("Starting kappa-core Layers Example");
    LayersApp app;
    app.Run();
    return 0;
}
