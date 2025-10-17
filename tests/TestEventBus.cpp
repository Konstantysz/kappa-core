#include "Event.h"
#include "EventBus.h"

#include <gtest/gtest.h>
#include <string>

using namespace Kappa;

// ============================================================================
// Test Events
// ============================================================================

class TestEvent : public Event
{
public:
    explicit TestEvent(int val) : value(val)
    {
    }
    int value;
};

class StringEvent : public Event
{
public:
    explicit StringEvent(std::string msg) : message(std::move(msg))
    {
    }
    std::string message;
};

class EmptyEvent : public Event
{
};

// ============================================================================
// EventBus Tests
// ============================================================================

class EventBusTest : public ::testing::Test
{
protected:
    EventBus eventBus;
};

// ============================================================================
// Subscription Tests
// ============================================================================

TEST_F(EventBusTest, SubscribeAndPublishSingleEvent)
{
    int receivedValue = 0;

    eventBus.Subscribe<TestEvent>([&receivedValue](const TestEvent &event) { receivedValue = event.value; });

    TestEvent event(42);
    eventBus.Publish(event);

    EXPECT_EQ(receivedValue, 42);
}

TEST_F(EventBusTest, SubscribeMultipleHandlersSameEvent)
{
    int count = 0;
    int sum = 0;

    eventBus.Subscribe<TestEvent>([&count](const TestEvent &) { count++; });

    eventBus.Subscribe<TestEvent>([&sum](const TestEvent &event) { sum += event.value; });

    TestEvent event(10);
    eventBus.Publish(event);

    EXPECT_EQ(count, 1);
    EXPECT_EQ(sum, 10);

    eventBus.Publish(event);

    EXPECT_EQ(count, 2);
    EXPECT_EQ(sum, 20);
}

TEST_F(EventBusTest, SubscribeDifferentEventTypes)
{
    int intValue = 0;
    std::string stringValue;

    eventBus.Subscribe<TestEvent>([&intValue](const TestEvent &event) { intValue = event.value; });

    eventBus.Subscribe<StringEvent>([&stringValue](const StringEvent &event) { stringValue = event.message; });

    TestEvent intEvent(123);
    StringEvent strEvent("Hello");

    eventBus.Publish(intEvent);
    eventBus.Publish(strEvent);

    EXPECT_EQ(intValue, 123);
    EXPECT_EQ(stringValue, "Hello");
}

TEST_F(EventBusTest, PublishWithoutSubscribers)
{
    TestEvent event(999);

    // Should not crash
    EXPECT_NO_THROW(eventBus.Publish(event));
}

TEST_F(EventBusTest, EmptyEventHandling)
{
    bool called = false;

    eventBus.Subscribe<EmptyEvent>([&called](const EmptyEvent &) { called = true; });

    EmptyEvent event;
    eventBus.Publish(event);

    EXPECT_TRUE(called);
}

// ============================================================================
// Multiple Publish Tests
// ============================================================================

TEST_F(EventBusTest, PublishMultipleEvents)
{
    std::vector<int> values;

    eventBus.Subscribe<TestEvent>([&values](const TestEvent &event) { values.push_back(event.value); });

    for (int i = 0; i < 10; ++i)
    {
        TestEvent event(i);
        eventBus.Publish(event);
    }

    ASSERT_EQ(values.size(), 10);
    for (size_t i = 0; i < values.size(); ++i)
    {
        EXPECT_EQ(values[i], static_cast<int>(i));
    }
}

// ============================================================================
// Lambda Capture Tests
// ============================================================================

TEST_F(EventBusTest, LambdaCaptureByValue)
{
    int capturedValue = 100;
    int receivedValue = 0;

    eventBus.Subscribe<TestEvent>(
        [capturedValue, &receivedValue](const TestEvent &event) { receivedValue = event.value + capturedValue; });

    TestEvent event(50);
    eventBus.Publish(event);

    EXPECT_EQ(receivedValue, 150);
}

TEST_F(EventBusTest, LambdaCaptureByReference)
{
    int counter = 0;

    eventBus.Subscribe<TestEvent>([&counter](const TestEvent &) { counter++; });

    for (int i = 0; i < 5; ++i)
    {
        TestEvent event(i);
        eventBus.Publish(event);
    }

    EXPECT_EQ(counter, 5);
}

// ============================================================================
// Event Ordering Tests
// ============================================================================

TEST_F(EventBusTest, HandlersCalledInSubscriptionOrder)
{
    std::vector<int> order;

    eventBus.Subscribe<TestEvent>([&order](const TestEvent &) { order.push_back(1); });

    eventBus.Subscribe<TestEvent>([&order](const TestEvent &) { order.push_back(2); });

    eventBus.Subscribe<TestEvent>([&order](const TestEvent &) { order.push_back(3); });

    TestEvent event(0);
    eventBus.Publish(event);

    ASSERT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
}

// ============================================================================
// Event Modification Tests
// ============================================================================

TEST_F(EventBusTest, EventIsConstInHandler)
{
    eventBus.Subscribe<TestEvent>([](const TestEvent &event) {
        // This should compile - event is const
        int val = event.value;
        (void)val; // Suppress unused warning
    });

    TestEvent event(100);
    EXPECT_NO_THROW(eventBus.Publish(event));
}

// ============================================================================
// Complex Event Data Tests
// ============================================================================

TEST_F(EventBusTest, EventWithComplexData)
{
    struct ComplexEvent : public Event
    {
        std::vector<int> numbers;
        std::string name;
        double coefficient;
    };

    std::vector<int> receivedNumbers;
    std::string receivedName;
    double receivedCoeff = 0.0;

    eventBus.Subscribe<ComplexEvent>([&](const ComplexEvent &event) {
        receivedNumbers = event.numbers;
        receivedName = event.name;
        receivedCoeff = event.coefficient;
    });

    ComplexEvent event;
    event.numbers = { 1, 2, 3, 4, 5 };
    event.name = "TestComplex";
    event.coefficient = 3.14159;

    eventBus.Publish(event);

    EXPECT_EQ(receivedNumbers, std::vector<int>({ 1, 2, 3, 4, 5 }));
    EXPECT_EQ(receivedName, "TestComplex");
    EXPECT_DOUBLE_EQ(receivedCoeff, 3.14159);
}
