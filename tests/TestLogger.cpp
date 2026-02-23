#include <Kappa/Logger.h>
#include <gtest/gtest.h>

using namespace Kappa;

TEST(LoggerTest, SetLevel)
{
    // Default level should be Info
    LOG_INFO("This is an info message");

    Logger::Get().SetLevel(LogLevel::Debug);
    LOG_DEBUG("This is a debug message - should be visible");

    Logger::Get().SetLevel(LogLevel::Warn);
    LOG_DEBUG("This is a debug message - should NOT be visible");
    LOG_WARN("This is a warning message - should be visible");
}

TEST(LoggerTest, AllLevels)
{
    Logger::Get().SetLevel(LogLevel::Trace);
    LOG_TRACE("Trace");
    LOG_DEBUG("Debug");
    LOG_INFO("Info");
    LOG_WARN("Warn");
    LOG_ERROR("Error");
    LOG_CRITICAL("Critical");
}

TEST(LoggerTest, Formatting)
{
    LOG_INFO("Hello {}!", "World");
    LOG_INFO("Value: {}", 42);
    LOG_INFO("Hex: {:x}", 255);
}
