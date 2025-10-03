# kappa-core Architecture

This document describes the architecture and design patterns used in kappa-core.

## Overview

kappa-core is a lightweight C++20 application framework built around a layer-based architecture. It provides the fundamental building blocks for creating desktop applications with OpenGL rendering.

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                        Application                          │
│  ┌───────────────────────────────────────────────────────┐  │
│  │                    Layer Stack                        │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │  │
│  │  │   Layer 1   │  │   Layer 2   │  │   Layer N   │  │  │
│  │  │             │  │             │  │             │  │  │
│  │  │ OnUpdate()  │  │ OnUpdate()  │  │ OnUpdate()  │  │  │
│  │  │ OnRender()  │  │ OnRender()  │  │ OnRender()  │  │  │
│  │  │ OnEvent()   │  │ OnEvent()   │  │ OnEvent()   │  │  │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  │  │
│  └───────────────────────────────────────────────────────┘  │
│                            ▲                                 │
│                            │                                 │
│  ┌─────────────────────────┴─────────────────────────────┐  │
│  │                      EventBus                         │  │
│  │         (Decoupled event communication)               │  │
│  └───────────────────────────────────────────────────────┘  │
│                            ▲                                 │
│                            │                                 │
│  ┌─────────────────────────┴─────────────────────────────┐  │
│  │                       Window                          │  │
│  │  ┌─────────────────────────────────────────────────┐  │  │
│  │  │              GLFW Integration                   │  │  │
│  │  │  - Window creation & management                 │  │  │
│  │  │  - OpenGL context                               │  │  │
│  │  │  - Input handling                               │  │  │
│  │  └─────────────────────────────────────────────────┘  │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### Application

The `Application` class is the heart of the framework. It:
- Manages the main application loop
- Maintains the layer stack
- Handles window creation and lifecycle
- Calculates frame delta time
- Coordinates updates and rendering

**Key responsibilities:**
- Initialize and run the application
- Dispatch events to layers
- Coordinate frame updates and rendering

### Layer System

Layers represent logical application components that can be stacked and updated independently.

```
┌─────────────────────────┐
│      Layer Stack        │
├─────────────────────────┤
│  UI Layer (top)         │ ← Rendered last, receives events first
├─────────────────────────┤
│  Game Logic Layer       │
├─────────────────────────┤
│  Rendering Layer        │
├─────────────────────────┤
│  Background (bottom)    │ ← Rendered first, receives events last
└─────────────────────────┘
```

Each layer implements:
- `OnAttach()` - Called when added to stack
- `OnDetach()` - Called when removed
- `OnUpdate(deltaTime)` - Called every frame
- `OnRender()` - Called for rendering
- `OnEvent(event)` - Called for event handling

### Event System

The EventBus provides decoupled communication between components:

```cpp
// Publisher
EventBus::Publish(MyEvent{ data });

// Subscriber
EventBus::Subscribe<MyEvent>([](const MyEvent& e) {
    // Handle event
});
```

**Benefits:**
- Loose coupling between components
- Type-safe event handling
- No need for explicit observer registration

### Window Management

The `Window` class wraps GLFW functionality:
- Window creation with customizable specifications
- OpenGL context management
- V-Sync control
- Input event generation
- Fullscreen/windowed modes

### Resource Management

**Texture:**
- Image loading via stb_image
- OpenGL texture object management
- RAII-based cleanup

**Logger:**
- Built on spdlog
- Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL)
- Configurable formatting

## Design Patterns

### 1. Layer Pattern
Organizes application logic into independent, stackable components.

**Advantages:**
- Clear separation of concerns
- Easy to add/remove features
- Predictable update and render order

### 2. Event-Driven Architecture
Uses EventBus for decoupled communication.

**Advantages:**
- Reduced coupling
- Easier testing
- Flexible component interaction

### 3. RAII (Resource Acquisition Is Initialization)
All resources (windows, textures, etc.) use RAII for automatic cleanup.

**Advantages:**
- No manual resource management
- Exception-safe
- Prevents resource leaks

### 4. Application Specification Pattern
Configuration through data structures rather than constructor parameters.

```cpp
ApplicationSpecification spec;
spec.name = "My App";
spec.width = 1280;
spec.height = 720;
Application app(spec);
```

## Data Flow

### Update Cycle

```
Frame Start
    ↓
Calculate deltaTime
    ↓
For each Layer (bottom to top):
    Layer::OnUpdate(deltaTime)
    ↓
Clear screen
    ↓
For each Layer (bottom to top):
    Layer::OnRender()
    ↓
Swap buffers
    ↓
Poll events
    ↓
Dispatch events to layers (top to bottom)
    ↓
Frame End
```

### Event Flow

```
GLFW Event (keyboard, mouse, etc.)
    ↓
Window translates to Core::Event
    ↓
EventBus::Publish(event)
    ↓
Subscribers notified
    ↓
Layers process event (top to bottom)
    ↓
Event consumed or propagated
```

## Extension Points

### Creating Custom Layers

```cpp
class MyLayer : public Core::Layer
{
    void OnAttach() override {
        // Initialize resources
    }

    void OnUpdate(float deltaTime) override {
        // Update logic
    }

    void OnRender() override {
        // Rendering code
    }

    void OnEvent(const Core::Event& event) override {
        // Handle events
    }
};
```

### Custom Events

```cpp
struct MyCustomEvent : public Core::Event
{
    int data;
    std::string message;
};

// Publish
EventBus::Publish(MyCustomEvent{ 42, "Hello" });

// Subscribe
EventBus::Subscribe<MyCustomEvent>([](const MyCustomEvent& e) {
    std::cout << e.message << ": " << e.data << std::endl;
});
```

## Thread Safety

**Current implementation:**
- Single-threaded main loop
- EventBus is **not** thread-safe
- All layer operations must occur on main thread

**Future considerations:**
- Thread-safe EventBus with mutex protection
- Worker thread pool for background tasks
- Async resource loading

## Performance Considerations

1. **Layer Updates:** Called every frame - keep `OnUpdate()` lightweight
2. **Event Subscriptions:** Use lambdas or function pointers, avoid heavy captures
3. **Rendering:** Batch draw calls, minimize state changes
4. **Resource Loading:** Load textures during initialization, not in render loop

## Dependencies

- **GLFW:** Window and input management
- **glad:** OpenGL loading
- **glm:** Mathematics library
- **spdlog:** Logging
- **stb_image:** Image loading (header-only)

## Future Enhancements

- Input abstraction layer
- Asset management system
- Scene graph
- Entity-Component-System (ECS)
- Audio system
- Networking support
