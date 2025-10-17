#pragma once
#include <string>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <glm/glm.hpp>

namespace Kappa
{
    /**
     * @brief Window state information for persistence.
     */
    struct WindowState
    {
        int posX = -1;            ///< Window X position (-1 means use default/centered)
        int posY = -1;            ///< Window Y position (-1 means use default/centered)
        int width = 1280;         ///< Window width in pixels
        int height = 720;         ///< Window height in pixels
        bool isMaximized = false; ///< Whether window is maximized
    };

    /**
     * @brief Configuration for creating a window.
     */
    struct WindowSpecification
    {
        std::string title;         ///< Window title displayed in the title bar
        unsigned int width = 1280; ///< Window width in pixels
        unsigned int height = 720; ///< Window height in pixels
        bool isResizable = true;   ///< Whether the window can be resized by the user
        bool vSync = true;         ///< Whether vertical sync is enabled
    };

    /**
     * @brief GLFW window wrapper with OpenGL context.
     */
    class Window
    {
    public:
        /**
         * @brief Constructs window.
         * @param spec Window configuration
         */
        explicit Window(const WindowSpecification &spec);

        /**
         * @brief Destructor that ensures proper cleanup of window resources.
         */
        ~Window();

        /**
         * @brief Creates the GLFW window and initializes OpenGL.
         */
        void Create();

        /**
         * @brief Destroys the window.
         */
        void Destroy();

        /**
         * @brief Updates the window and swaps buffers.
         */
        void Update();

        /**
         * @brief Returns the framebuffer size.
         * @return Framebuffer size
         */
        [[nodiscard]] glm::vec2 GetFrameBufferSize() const;

        /**
         * @brief Checks if the window should close.
         * @return True if should close
         */
        [[nodiscard]] bool ShouldClose() const;

        /**
         * @brief Returns the GLFW window handle.
         * @return GLFW window handle
         */
        [[nodiscard]] GLFWwindow *GetHandle() const;

        /**
         * @brief Captures current window state.
         * @return Current window state (position, size, maximized status)
         */
        [[nodiscard]] WindowState GetState() const;

        /**
         * @brief Applies window state.
         * @param state State to apply
         * @note Should be called after Create() to properly position and size the window
         */
        void SetState(const WindowState &state);

        /**
         * @brief Gets window position.
         * @param outX Output X position
         * @param outY Output Y position
         */
        void GetPosition(int &outX, int &outY) const;

        /**
         * @brief Sets window position.
         * @param x X position
         * @param y Y position
         */
        void SetPosition(int x, int y);

        /**
         * @brief Gets window size.
         * @param outWidth Output width
         * @param outHeight Output height
         */
        void GetSize(int &outWidth, int &outHeight) const;

        /**
         * @brief Sets window size.
         * @param width Width in pixels
         * @param height Height in pixels
         */
        void SetSize(int width, int height);

        /**
         * @brief Checks if window is maximized.
         * @return True if maximized
         */
        [[nodiscard]] bool IsMaximized() const;

        /**
         * @brief Maximizes the window.
         */
        void Maximize();

        /**
         * @brief Restores the window from maximized state.
         */
        void Restore();

        /**
         * @brief Centers window on primary monitor.
         */
        void Center();

    private:
        WindowSpecification specification; ///< Window configuration
        GLFWwindow *handle;                ///< GLFW window handle
    };
} // namespace Kappa
