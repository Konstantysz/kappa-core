#include "Event.h"
#include "Layer.h"

#include <gtest/gtest.h>

using namespace Kappa;

// ============================================================================
// Test Layers
// ============================================================================

class SimpleLayer : public Layer
{
public:
    SimpleLayer() = default;

    void OnEvent(Event &event) override
    {
        eventCount++;
    }

    void OnUpdate(float deltaTime) override
    {
        updateCount++;
        lastDeltaTime = deltaTime;
    }

    void OnRender() override
    {
        renderCount++;
    }

    int eventCount = 0;
    int updateCount = 0;
    int renderCount = 0;
    float lastDeltaTime = 0.0f;
};

class ParameterizedLayer : public Layer
{
public:
    explicit ParameterizedLayer(std::string name, int priority) : layerName(std::move(name)), layerPriority(priority)
    {
    }

    void OnEvent(Event &event) override
    {
    }
    void OnUpdate(float deltaTime) override
    {
    }
    void OnRender() override
    {
    }

    std::string layerName;
    int layerPriority;
};

// ============================================================================
// Layer Tests
// ============================================================================

class LayerTest : public ::testing::Test
{
protected:
};

// ============================================================================
// Basic Layer Tests
// ============================================================================

TEST_F(LayerTest, LayerCanBeCreated)
{
    EXPECT_NO_THROW(SimpleLayer layer);
}

TEST_F(LayerTest, LayerDefaultConstructor)
{
    SimpleLayer layer;

    EXPECT_EQ(layer.eventCount, 0);
    EXPECT_EQ(layer.updateCount, 0);
    EXPECT_EQ(layer.renderCount, 0);
}

TEST_F(LayerTest, LayerParameterizedConstructor)
{
    auto pLayer = ParameterizedLayer("TestLayer", 10);
    EXPECT_EQ(pLayer.layerName, "TestLayer");
    EXPECT_EQ(pLayer.layerPriority, 10);
}

// ============================================================================
// OnEvent Tests
// ============================================================================

class TestEvent : public Event
{
};

TEST_F(LayerTest, OnEventIncreasesCounter)
{
    SimpleLayer layer;
    TestEvent event;

    layer.OnEvent(event);

    EXPECT_EQ(layer.eventCount, 1);
}

TEST_F(LayerTest, OnEventMultipleCalls)
{
    SimpleLayer layer;
    TestEvent event;

    for (int i = 0; i < 5; ++i)
    {
        layer.OnEvent(event);
    }

    EXPECT_EQ(layer.eventCount, 5);
}

// ============================================================================
// OnUpdate Tests
// ============================================================================

TEST_F(LayerTest, OnUpdateIncreasesCounter)
{
    SimpleLayer layer;

    layer.OnUpdate(0.016f);

    EXPECT_EQ(layer.updateCount, 1);
    EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.016f);
}

TEST_F(LayerTest, OnUpdateWithZeroDeltaTime)
{
    SimpleLayer layer;

    layer.OnUpdate(0.0f);

    EXPECT_EQ(layer.updateCount, 1);
    EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.0f);
}

TEST_F(LayerTest, OnUpdateWithVariableDeltaTimes)
{
    SimpleLayer layer;

    layer.OnUpdate(0.016f);
    EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.016f);

    layer.OnUpdate(0.033f);
    EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.033f);

    layer.OnUpdate(0.008f);
    EXPECT_FLOAT_EQ(layer.lastDeltaTime, 0.008f);

    EXPECT_EQ(layer.updateCount, 3);
}

TEST_F(LayerTest, OnUpdateMultipleCalls)
{
    SimpleLayer layer;

    for (int i = 0; i < 100; ++i)
    {
        layer.OnUpdate(0.016f);
    }

    EXPECT_EQ(layer.updateCount, 100);
}

// ============================================================================
// OnRender Tests
// ============================================================================

TEST_F(LayerTest, OnRenderIncreasesCounter)
{
    SimpleLayer layer;

    layer.OnRender();

    EXPECT_EQ(layer.renderCount, 1);
}

TEST_F(LayerTest, OnRenderMultipleCalls)
{
    SimpleLayer layer;

    for (int i = 0; i < 60; ++i)
    {
        layer.OnRender();
    }

    EXPECT_EQ(layer.renderCount, 60);
}

// ============================================================================
// Combined Method Tests
// ============================================================================

TEST_F(LayerTest, AllMethodsWorkTogether)
{
    SimpleLayer layer;
    TestEvent event;

    layer.OnUpdate(0.016f);
    layer.OnRender();
    layer.OnEvent(event);

    EXPECT_EQ(layer.updateCount, 1);
    EXPECT_EQ(layer.renderCount, 1);
    EXPECT_EQ(layer.eventCount, 1);
}

TEST_F(LayerTest, SimulateGameLoop)
{
    SimpleLayer layer;
    TestEvent event;

    // Simulate 60 frames at 60 FPS
    for (int frame = 0; frame < 60; ++frame)
    {
        layer.OnUpdate(0.0167f); // ~60 FPS
        layer.OnRender();

        if (frame % 10 == 0)
        {
            layer.OnEvent(event);
        }
    }

    EXPECT_EQ(layer.updateCount, 60);
    EXPECT_EQ(layer.renderCount, 60);
    EXPECT_EQ(layer.eventCount, 6); // Every 10th frame
}

// ============================================================================
// Polymorphism Tests
// ============================================================================

TEST_F(LayerTest, LayerPolymorphism)
{
    std::unique_ptr<Layer> layer = std::make_unique<SimpleLayer>();
    TestEvent event;

    EXPECT_NO_THROW(layer->OnUpdate(0.016f));
    EXPECT_NO_THROW(layer->OnRender());
    EXPECT_NO_THROW(layer->OnEvent(event));
}

TEST_F(LayerTest, LayerVirtualDestructor)
{
    Layer *layer = new SimpleLayer();

    EXPECT_NO_THROW(delete layer);
}
