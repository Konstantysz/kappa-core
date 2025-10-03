# kappa-core Examples

This document describes the example applications included with kappa-core.

## Building Examples

Examples are built by default. To disable them:

```bash
cmake -B build -DBUILD_EXAMPLES=OFF
```

To build examples:

```bash
cmake --build build
```

## Available Examples

### 1. Minimal Example (`examples/minimal`)

**Location:** `examples/minimal/main.cpp`

**Purpose:** Demonstrates the absolute minimum code needed to create a kappa-core application.

**Features:**
- Single layer application
- Basic rendering (colored background)
- Shows layer lifecycle (OnAttach, OnUpdate, OnRender)

**Run:**
```bash
./build/examples/minimal/minimal_example
```

**Code Highlights:**
```cpp
class ExampleLayer : public Core::Layer
{
    void OnRender() override
    {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
```

---

### 2. Layers Example (`examples/layers`)

**Location:** `examples/layers/main.cpp`

**Purpose:** Demonstrates the layer system with multiple stacked layers.

**Features:**
- Three-layer architecture (Background, Game, UI)
- Layer update ordering
- Layer rendering order
- Event handling across layers
- Frame-independent updates with deltaTime

**Layers:**
1. **BackgroundLayer** - Renders solid background color
2. **GameLayer** - Renders animated colored quad, handles input
3. **UILayer** - Renders FPS indicator, overlay UI

**Controls:**
- `R` - Reset color animation
- `F` - Show FPS in console

**Run:**
```bash
./build/examples/layers/layers_example
```

**Key Concepts:**
- Layer stacking (bottom-to-top rendering)
- Event propagation (top-to-bottom)
- Separation of concerns (background, logic, UI)

---

### 3. Events Example (`examples/events`)

**Location:** `examples/events/main.cpp`

**Purpose:** Comprehensive demonstration of the EventBus system with custom events.

**Features:**
- Custom event types
- EventBus publish/subscribe pattern
- Event-driven state management
- Decoupled component communication

**Custom Events:**
- `PlayerMovedEvent` - Triggered on player movement
- `ScoreChangedEvent` - Triggered when score changes
- `GameStateEvent` - Triggered on game state changes (Playing/Paused/GameOver)

**Controls:**
- `Arrow Keys` - Move player (triggers PlayerMovedEvent)
- `Space` - Add 10 points (triggers ScoreChangedEvent)
- `P` - Toggle pause (triggers GameStateEvent)
- `Escape` - Game over (triggers GameStateEvent)

**Run:**
```bash
./build/examples/events/events_example
```

**Key Concepts:**
```cpp
// Define custom event
struct PlayerMovedEvent : public Event
{
    float x, y;
};

// Subscribe to event
EventBus::Subscribe<PlayerMovedEvent>([](const PlayerMovedEvent& e) {
    LOG_INFO("Player at ({}, {})", e.x, e.y);
});

// Publish event
EventBus::Publish(PlayerMovedEvent(x, y));
```

---

## Creating Your Own Examples

### Template Structure

```cpp
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Logger.h"

class MyLayer : public Core::Layer
{
    void OnAttach() override
    {
        // Initialize resources
    }

    void OnUpdate(float deltaTime) override
    {
        // Update logic
    }

    void OnRender() override
    {
        // Rendering code
    }

    void OnEvent(const Core::Event& event) override
    {
        // Handle events
    }
};

int main()
{
    Core::Logger::Init();

    Core::ApplicationSpecification spec;
    spec.name = "My Application";
    spec.width = 1280;
    spec.height = 720;

    Core::Application app(spec);
    app.PushLayer(std::make_shared<MyLayer>());
    app.Run();

    return 0;
}
```

### CMakeLists.txt Template

```cmake
cmake_minimum_required(VERSION 3.26)
project(my_example)

add_executable(my_example main.cpp)
target_link_libraries(my_example PRIVATE Core)
target_compile_features(my_example PRIVATE cxx_std_20)
```

## Common Patterns

### Pattern 1: Multiple Layers

```cpp
app.PushLayer(std::make_shared<BackgroundLayer>());
app.PushLayer(std::make_shared<GameLayer>());
app.PushLayer(std::make_shared<UILayer>());
```

Rendering order: Background → Game → UI (bottom to top)
Event handling: UI → Game → Background (top to bottom)

### Pattern 2: Event Communication

```cpp
// Define event
struct MyEvent : public Core::Event { int data; };

// Subscribe (in OnAttach)
EventBus::Subscribe<MyEvent>([this](const MyEvent& e) {
    HandleMyEvent(e);
});

// Publish (anywhere)
EventBus::Publish(MyEvent{ 42 });
```

### Pattern 3: Frame-Independent Movement

```cpp
void OnUpdate(float deltaTime) override
{
    position += velocity * deltaTime;
}
```

### Pattern 4: Input Handling

```cpp
void OnEvent(const Event& event) override
{
    if (auto* keyEvent = dynamic_cast<const KeyPressedEvent*>(&event))
    {
        if (keyEvent->key == GLFW_KEY_SPACE)
        {
            // Handle space key
        }
    }
}
```

## Next Steps

After reviewing the examples:

1. **Read the Architecture Documentation** - `docs/ARCHITECTURE.md`
2. **Browse the API Reference** - Generate with Doxygen
3. **Create Your Own Application** - Start with the minimal template
4. **Explore Advanced Features** - Textures, custom events, complex layer hierarchies

## Troubleshooting

**Examples don't build:**
- Ensure vcpkg dependencies are installed
- Check CMake output for missing packages
- Verify BUILD_EXAMPLES is ON

**Examples crash on startup:**
- Check GPU supports OpenGL 4.3+
- Verify graphics drivers are up to date
- Check console output for error messages

**Performance issues:**
- Disable V-Sync in ApplicationSpecification
- Profile your OnUpdate/OnRender code
- Batch rendering operations
