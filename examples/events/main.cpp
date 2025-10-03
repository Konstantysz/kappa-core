#include "Core/Application.h"
#include "Core/EventBus.h"
#include "Core/Layer.h"
#include "Core/Logger.h"
#include <GLFW/glfw3.h>
#include <string>

using namespace Core;

// Custom event examples
struct PlayerMovedEvent : public Event
{
    float x, y;
    PlayerMovedEvent(float x, float y) : x(x), y(y) {}
};

struct ScoreChangedEvent : public Event
{
    int newScore;
    int delta;
    ScoreChangedEvent(int score, int delta) : newScore(score), delta(delta) {}
};

struct GameStateEvent : public Event
{
    enum class State
    {
        Playing,
        Paused,
        GameOver
    };
    State state;
    GameStateEvent(State s) : state(s) {}
};

// Example layer demonstrating EventBus usage
class EventDemoLayer : public Layer
{
private:
    float playerX = 0.0f;
    float playerY = 0.0f;
    int score = 0;
    GameStateEvent::State gameState = GameStateEvent::State::Playing;
    float moveSpeed = 2.0f;

public:
    void OnAttach() override
    {
        LOG_INFO("EventDemoLayer attached");

        // Subscribe to custom events
        EventBus::Subscribe<PlayerMovedEvent>([this](const PlayerMovedEvent &e) {
            LOG_DEBUG("Player moved to ({:.2f}, {:.2f})", e.x, e.y);
        });

        EventBus::Subscribe<ScoreChangedEvent>([this](const ScoreChangedEvent &e) {
            if (e.delta > 0)
            {
                LOG_INFO("Score increased by {}! New score: {}", e.delta, e.newScore);
            }
            else
            {
                LOG_WARN("Score decreased by {}. New score: {}", -e.delta, e.newScore);
            }
        });

        EventBus::Subscribe<GameStateEvent>([this](const GameStateEvent &e) {
            switch (e.state)
            {
            case GameStateEvent::State::Playing:
                LOG_INFO("Game state: PLAYING");
                break;
            case GameStateEvent::State::Paused:
                LOG_INFO("Game state: PAUSED");
                break;
            case GameStateEvent::State::GameOver:
                LOG_CRITICAL("Game state: GAME OVER");
                break;
            }
        });

        LOG_INFO("Controls:");
        LOG_INFO("  Arrow Keys - Move player");
        LOG_INFO("  SPACE      - Add 10 points");
        LOG_INFO("  P          - Toggle pause");
        LOG_INFO("  ESC        - Game over");
    }

    void OnUpdate(float deltaTime) override
    {
        if (gameState != GameStateEvent::State::Playing)
        {
            return; // Don't update when paused or game over
        }

        // Movement logic would go here
    }

    void OnRender() override
    {
        // Clear background
        if (gameState == GameStateEvent::State::GameOver)
        {
            glClearColor(0.3f, 0.0f, 0.0f, 1.0f); // Red tint for game over
        }
        else if (gameState == GameStateEvent::State::Paused)
        {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Gray for paused
        }
        else
        {
            glClearColor(0.0f, 0.1f, 0.2f, 1.0f); // Normal blue
        }
        glClear(GL_COLOR_BUFFER_BIT);

        // Render player (white square)
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(playerX - 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY + 0.05f);
        glVertex2f(playerX - 0.05f, playerY + 0.05f);
        glEnd();

        // Render score indicator (visual representation)
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

    void OnEvent(const Event &event) override
    {
        if (auto *keyEvent = dynamic_cast<const KeyPressedEvent *>(&event))
        {
            HandleKeyPress(keyEvent->key);
        }
    }

private:
    void HandleKeyPress(int key)
    {
        if (gameState == GameStateEvent::State::GameOver)
        {
            return; // Ignore input when game over
        }

        switch (key)
        {
        case GLFW_KEY_LEFT:
            if (gameState == GameStateEvent::State::Playing)
            {
                playerX -= 0.1f;
                EventBus::Publish(PlayerMovedEvent(playerX, playerY));
            }
            break;

        case GLFW_KEY_RIGHT:
            if (gameState == GameStateEvent::State::Playing)
            {
                playerX += 0.1f;
                EventBus::Publish(PlayerMovedEvent(playerX, playerY));
            }
            break;

        case GLFW_KEY_UP:
            if (gameState == GameStateEvent::State::Playing)
            {
                playerY += 0.1f;
                EventBus::Publish(PlayerMovedEvent(playerX, playerY));
            }
            break;

        case GLFW_KEY_DOWN:
            if (gameState == GameStateEvent::State::Playing)
            {
                playerY -= 0.1f;
                EventBus::Publish(PlayerMovedEvent(playerX, playerY));
            }
            break;

        case GLFW_KEY_SPACE:
            if (gameState == GameStateEvent::State::Playing)
            {
                score += 10;
                EventBus::Publish(ScoreChangedEvent(score, 10));
            }
            break;

        case GLFW_KEY_P:
            if (gameState == GameStateEvent::State::Playing)
            {
                gameState = GameStateEvent::State::Paused;
                EventBus::Publish(GameStateEvent(GameStateEvent::State::Paused));
            }
            else if (gameState == GameStateEvent::State::Paused)
            {
                gameState = GameStateEvent::State::Playing;
                EventBus::Publish(GameStateEvent(GameStateEvent::State::Playing));
            }
            break;

        case GLFW_KEY_ESCAPE:
            gameState = GameStateEvent::State::GameOver;
            EventBus::Publish(GameStateEvent(GameStateEvent::State::GameOver));
            break;
        }
    }
};

int main()
{
    Logger::Init();

    ApplicationSpecification spec;
    spec.name = "kappa-core Events Example";
    spec.width = 800;
    spec.height = 600;
    spec.vsync = true;

    Application app(spec);
    app.PushLayer(std::make_shared<EventDemoLayer>());

    LOG_INFO("Starting EventBus demonstration");
    app.Run();

    return 0;
}
