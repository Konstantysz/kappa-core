#include "Application.h"
#include "Layer.h"
#include "Logger.h"

#include <glad/glad.h>

/**
 * @brief Minimal example layer demonstrating basic rendering.
 */
class ExampleLayer : public Kappa::Layer
{
public:
    void OnRender() override
    {
        // Clear screen with a nice color
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class MinimalApp : public Kappa::Application
{
public:
    MinimalApp() : Application(GetSpec())
    {
        PushLayer<ExampleLayer>();
    }

private:
    static Kappa::ApplicationSpecification GetSpec()
    {
        Kappa::ApplicationSpecification spec;
        spec.name = "kappa-core Minimal Example";
        spec.windowSpecification.width = 1280;
        spec.windowSpecification.height = 720;
        return spec;
    }
};

int main()
{
    Kappa::Logger::Init();
    MinimalApp app;
    app.Run();
    return 0;
}
