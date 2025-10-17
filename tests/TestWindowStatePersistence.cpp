#include "Window.h"
#include "WindowStatePersistence.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace Kappa;

// ============================================================================
// WindowStatePersistence Tests
// ============================================================================

class WindowStatePersistenceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        testFilePath = "test_window_state.json";

        // Clean up any existing test file
        if (std::filesystem::exists(testFilePath))
        {
            std::filesystem::remove(testFilePath);
        }
    }

    void TearDown() override
    {
        // Clean up test file after each test
        if (std::filesystem::exists(testFilePath))
        {
            std::filesystem::remove(testFilePath);
        }
    }

    std::string testFilePath;
};

// ============================================================================
// Save Tests
// ============================================================================

TEST_F(WindowStatePersistenceTest, SaveCreatesFile)
{
    WindowState state;
    state.posX = 100;
    state.posY = 200;
    state.width = 1920;
    state.height = 1080;
    state.isMaximized = false;

    WindowStatePersistence::Save(testFilePath, state);

    EXPECT_TRUE(std::filesystem::exists(testFilePath));
}

TEST_F(WindowStatePersistenceTest, SaveWithDefaultState)
{
    WindowState state; // All zeros, not maximized

    EXPECT_NO_THROW(WindowStatePersistence::Save(testFilePath, state));
    EXPECT_TRUE(std::filesystem::exists(testFilePath));
}

TEST_F(WindowStatePersistenceTest, SaveWithMaximizedState)
{
    WindowState state;
    state.posX = 50;
    state.posY = 50;
    state.width = 800;
    state.height = 600;
    state.isMaximized = true;

    EXPECT_NO_THROW(WindowStatePersistence::Save(testFilePath, state));
    EXPECT_TRUE(std::filesystem::exists(testFilePath));
}

TEST_F(WindowStatePersistenceTest, SaveWithNegativePositions)
{
    WindowState state;
    state.posX = -100;
    state.posY = -50;
    state.width = 1280;
    state.height = 720;

    EXPECT_NO_THROW(WindowStatePersistence::Save(testFilePath, state));
}

TEST_F(WindowStatePersistenceTest, SaveWithLargeDimensions)
{
    WindowState state;
    state.posX = 0;
    state.posY = 0;
    state.width = 7680;  // 8K
    state.height = 4320; // 8K

    EXPECT_NO_THROW(WindowStatePersistence::Save(testFilePath, state));
}

// ============================================================================
// Load Tests
// ============================================================================

TEST_F(WindowStatePersistenceTest, LoadNonExistentFileReturnsDefault)
{
    auto state = WindowStatePersistence::Load("nonexistent_file.json");

    EXPECT_EQ(state.posX, -1);
    EXPECT_EQ(state.posY, -1);
    EXPECT_EQ(state.width, 1280);
    EXPECT_EQ(state.height, 720);
    EXPECT_FALSE(state.isMaximized);
}

TEST_F(WindowStatePersistenceTest, LoadAfterSaveReturnsCorrectState)
{
    WindowState originalState;
    originalState.posX = 150;
    originalState.posY = 250;
    originalState.width = 1600;
    originalState.height = 900;
    originalState.isMaximized = true;

    WindowStatePersistence::Save(testFilePath, originalState);
    auto loadedState = WindowStatePersistence::Load(testFilePath);

    EXPECT_EQ(loadedState.posX, originalState.posX);
    EXPECT_EQ(loadedState.posY, originalState.posY);
    EXPECT_EQ(loadedState.width, originalState.width);
    EXPECT_EQ(loadedState.height, originalState.height);
    EXPECT_EQ(loadedState.isMaximized, originalState.isMaximized);
}

TEST_F(WindowStatePersistenceTest, LoadWithNegativePositions)
{
    WindowState originalState;
    originalState.posX = -200;
    originalState.posY = -100;
    originalState.width = 1280;
    originalState.height = 720;
    originalState.isMaximized = false;

    WindowStatePersistence::Save(testFilePath, originalState);
    auto loadedState = WindowStatePersistence::Load(testFilePath);

    EXPECT_EQ(loadedState.posX, originalState.posX);
    EXPECT_EQ(loadedState.posY, originalState.posY);
}

// ============================================================================
// Round-Trip Tests
// ============================================================================

TEST_F(WindowStatePersistenceTest, MultipleRoundTrips)
{
    for (int i = 0; i < 5; ++i)
    {
        WindowState state;
        state.posX = i * 100;
        state.posY = i * 50;
        state.width = 1920 - i * 100;
        state.height = 1080 - i * 50;
        state.isMaximized = (i % 2 == 0);

        WindowStatePersistence::Save(testFilePath, state);
        auto loadedState = WindowStatePersistence::Load(testFilePath);

        EXPECT_EQ(loadedState.posX, state.posX);
        EXPECT_EQ(loadedState.posY, state.posY);
        EXPECT_EQ(loadedState.width, state.width);
        EXPECT_EQ(loadedState.height, state.height);
        EXPECT_EQ(loadedState.isMaximized, state.isMaximized);
    }
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST_F(WindowStatePersistenceTest, SaveToPathWithSpaces)
{
    std::string pathWithSpaces = "test folder with spaces/window_state.json";

    // Create directory if needed
    std::filesystem::create_directories("test folder with spaces");

    WindowState state;
    state.posX = 100;
    state.posY = 100;
    state.width = 800;
    state.height = 600;

    EXPECT_NO_THROW(WindowStatePersistence::Save(pathWithSpaces, state));
    EXPECT_TRUE(std::filesystem::exists(pathWithSpaces));

    // Cleanup
    std::filesystem::remove(pathWithSpaces);
    std::filesystem::remove("test folder with spaces");
}

TEST_F(WindowStatePersistenceTest, OverwriteExistingFile)
{
    WindowState state1;
    state1.posX = 100;
    state1.posY = 100;

    WindowState state2;
    state2.posX = 200;
    state2.posY = 200;

    WindowStatePersistence::Save(testFilePath, state1);
    WindowStatePersistence::Save(testFilePath, state2);

    auto loadedState = WindowStatePersistence::Load(testFilePath);

    EXPECT_EQ(loadedState.posX, state2.posX);
    EXPECT_EQ(loadedState.posY, state2.posY);
}

TEST_F(WindowStatePersistenceTest, ZeroDimensionsRoundTrip)
{
    WindowState state;
    state.posX = 0;
    state.posY = 0;
    state.width = 0;
    state.height = 0;
    state.isMaximized = false;

    WindowStatePersistence::Save(testFilePath, state);
    auto loadedState = WindowStatePersistence::Load(testFilePath);

    EXPECT_EQ(loadedState.width, 0);
    EXPECT_EQ(loadedState.height, 0);
}

// ============================================================================
// Invalid JSON Handling Tests
// ============================================================================

TEST_F(WindowStatePersistenceTest, LoadInvalidJSONReturnsDefault)
{
    // Create file with invalid JSON
    std::ofstream file(testFilePath);
    file << "{ invalid json }";
    file.close();

    auto state = WindowStatePersistence::Load(testFilePath);

    // Should return default state on parse error
    EXPECT_EQ(state.posX, -1);
    EXPECT_EQ(state.posY, -1);
    EXPECT_EQ(state.width, 1280);
    EXPECT_EQ(state.height, 720);
    EXPECT_FALSE(state.isMaximized);
}

TEST_F(WindowStatePersistenceTest, LoadEmptyFileReturnsDefault)
{
    // Create empty file
    std::ofstream file(testFilePath);
    file.close();

    auto state = WindowStatePersistence::Load(testFilePath);

    EXPECT_EQ(state.posX, -1);
    EXPECT_EQ(state.posY, -1);
    EXPECT_EQ(state.width, 1280);
    EXPECT_EQ(state.height, 720);
    EXPECT_FALSE(state.isMaximized);
}

// Note: CaptureAndSave and LoadAndApply tests require an actual Window instance
// which needs GLFW context. These are better suited for integration tests.
