#pragma once

#include "Window.h"
#include <string>

namespace Kappa
{
    /**
     * @brief Utility for persisting and loading window state to/from disk.
     */
    class WindowStatePersistence
    {
    public:
        /**
         * @brief Loads window state from a JSON file.
         * @param filePath Path to the JSON file
         * @return Window state (returns default state if file doesn't exist or is invalid)
         */
        [[nodiscard]] static WindowState Load(const std::string &filePath);

        /**
         * @brief Saves window state to a JSON file.
         * @param filePath Path to the JSON file
         * @param state Window state to save
         * @return True if saved successfully, false otherwise
         */
        static bool Save(const std::string &filePath, const WindowState &state);

        /**
         * @brief Loads and applies window state to a window.
         * @param window Window to apply state to
         * @param filePath Path to the JSON file
         * @return True if state was loaded and applied, false if using defaults
         */
        static bool LoadAndApply(Window &window, const std::string &filePath);

        /**
         * @brief Captures and saves current window state.
         * @param window Window to capture state from
         * @param filePath Path to the JSON file
         * @return True if saved successfully, false otherwise
         */
        static bool CaptureAndSave(const Window &window, const std::string &filePath);
    };
} // namespace Kappa
