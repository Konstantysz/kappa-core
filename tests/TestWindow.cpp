#include "Window.h"
#include "Logger.h"

#include <gtest/gtest.h>

using namespace Kappa;

// ============================================================================
// Window Specification Tests
// ============================================================================

class WindowSpecificationTest : public ::testing::Test
{
protected:
};

TEST_F(WindowSpecificationTest, DefaultValues)
{
    WindowSpecification spec;

    EXPECT_EQ(spec.title, "");
    EXPECT_EQ(spec.width, 1280);
    EXPECT_EQ(spec.height, 720);
    EXPECT_TRUE(spec.isResizable);
    EXPECT_TRUE(spec.vSync);
}

TEST_F(WindowSpecificationTest, CustomValues)
{
    WindowSpecification spec;
    spec.title = "Custom Window";
    spec.width = 1920;
    spec.height = 1080;
    spec.isResizable = false;
    spec.vSync = false;

    EXPECT_EQ(spec.title, "Custom Window");
    EXPECT_EQ(spec.width, 1920);
    EXPECT_EQ(spec.height, 1080);
    EXPECT_FALSE(spec.isResizable);
    EXPECT_FALSE(spec.vSync);
}

TEST_F(WindowSpecificationTest, ZeroDimensions)
{
    WindowSpecification spec;
    spec.width = 0;
    spec.height = 0;

    EXPECT_EQ(spec.width, 0);
    EXPECT_EQ(spec.height, 0);
}

TEST_F(WindowSpecificationTest, LargeDimensions)
{
    WindowSpecification spec;
    spec.width = 7680;  // 8K width
    spec.height = 4320; // 8K height

    EXPECT_EQ(spec.width, 7680);
    EXPECT_EQ(spec.height, 4320);
}

TEST_F(WindowSpecificationTest, EmptyTitle)
{
    WindowSpecification spec;
    spec.title = "";

    EXPECT_EQ(spec.title, "");
}

TEST_F(WindowSpecificationTest, LongTitle)
{
    WindowSpecification spec;
    spec.title = std::string(1000, 'A');

    EXPECT_EQ(spec.title.length(), 1000);
}

TEST_F(WindowSpecificationTest, SpecialCharactersInTitle)
{
    WindowSpecification spec;
    spec.title = "Test™ Window® with Émojis 🚀";

    EXPECT_EQ(spec.title, "Test™ Window® with Émojis 🚀");
}

// ============================================================================
// Window State Tests
// ============================================================================

class WindowStateTest : public ::testing::Test
{
protected:
};

TEST_F(WindowStateTest, DefaultValues)
{
    WindowState state;

    EXPECT_EQ(state.posX, -1);
    EXPECT_EQ(state.posY, -1);
    EXPECT_EQ(state.width, 1280);
    EXPECT_EQ(state.height, 720);
    EXPECT_FALSE(state.isMaximized);
}

TEST_F(WindowStateTest, CustomValues)
{
    WindowState state;
    state.posX = 100;
    state.posY = 200;
    state.width = 1920;
    state.height = 1080;
    state.isMaximized = true;

    EXPECT_EQ(state.posX, 100);
    EXPECT_EQ(state.posY, 200);
    EXPECT_EQ(state.width, 1920);
    EXPECT_EQ(state.height, 1080);
    EXPECT_TRUE(state.isMaximized);
}

TEST_F(WindowStateTest, NegativePositions)
{
    WindowState state;
    state.posX = -100;
    state.posY = -50;

    EXPECT_EQ(state.posX, -100);
    EXPECT_EQ(state.posY, -50);
}

TEST_F(WindowStateTest, MaximizedState)
{
    WindowState state;
    state.isMaximized = true;

    EXPECT_TRUE(state.isMaximized);

    state.isMaximized = false;

    EXPECT_FALSE(state.isMaximized);
}

// Note: Full Window tests with GLFW initialization would require
// a graphical context and are better suited for integration tests
// rather than unit tests. These tests cover the data structures only.
