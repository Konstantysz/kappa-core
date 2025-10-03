#include "Application.h"
#include "EventBus.h"
#include "Layer.h"
#include "Logger.h"

#include <GLFW/glfw3.h>
#include <string>

using namespace Kappa;

// Custom event examples
struct PlayerMovedEvent : public Event
{
    float x, y;
    PlayerMovedEvent(float x, float y) : x(x), y(y) {}
};

struct ScoreChangedEvent : public Event
{
    int newScore;
    ScoreChangedEvent(int score) : newScore(score) {}
};

class EventDemoLayer : public Layer
{
private:
    EventBus eventBus;
    float playerX = 0.0f;
    float playerY = 0.0f;
    int score = 0;
    float time = 0.0f;

public:
    EventDemoLayer()
    {
        // Subscribe to custom events
        eventBus.Subscribe<PlayerMovedEvent>([this](const PlayerMovedEvent &e) {
            LOG_INFO("Player moved to ({:.2f}, {:.2f})", e.x, e.y);
            playerX = e.x;
            playerY = e.y;
        });

        eventBus.Subscribe<ScoreChangedEvent>([this](const ScoreChangedEvent &e) {
            LOG_INFO("Score changed to {}", e.newScore);
            score = e.newScore;
        });
    }

    void OnUpdate(float deltaTime) override
    {
        time += deltaTime;

        // Simulate player movement every 2 seconds
        if (static_cast<int>(time) % 2 == 0 && static_cast<int>(time * 10) % 10 == 0)
        {
            float newX = std::sin(time * 0.5f) * 0.5f;
            float newY = std::cos(time * 0.5f) * 0.5f;
            eventBus.Publish(PlayerMovedEvent(newX, newY));
        }

        // Simulate score increase every 3 seconds
        if (static_cast<int>(time) % 3 == 0 && static_cast<int>(time * 10) % 10 == 0)
        {
            eventBus.Publish(ScoreChangedEvent(score + 10));
        }
    }

    void OnRender() override
    {
        // Clear background
        glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render player
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(playerX - 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY + 0.05f);
        glVertex2f(playerX - 0.05f, playerY + 0.05f);
        glEnd();

        // Render score bar
        float scoreHeight = (score / 100.0f) * 1.8f - 0.9f;
        if (scoreHeight > -0.9f)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-0.95f, -0.9f);
            glVertex2f(-0.9f, -0.9f);
            glVertex2f(-0.9f, scoreHeight);
            glVertex2f(-0.95f, scoreHeight);
            glEnd();
        }
    }
};

class EventsApp : public Application
{
public:
    EventsApp() : Application(GetSpec())
    {
        PushLayer<EventDemoLayer>();
    }

private:
    static ApplicationSpecification GetSpec()
    {
        ApplicationSpecification spec;
        spec.name = "kappa-core Events Example";
        spec.windowSpecification.width = 800;
        spec.windowSpecification.height = 600;
        return spec;
    }
};

int main()
{
    LOG_INFO("Starting kappa-core Events Example");
    EventsApp app;
    app.Run();
    return 0;
}
