#include "WindowStatePersistence.h"
#include "Logger.h"

#include <fstream>
#include <nlohmann/json.hpp>

namespace Kappa
{
    WindowState WindowStatePersistence::Load(const std::string &filePath)
    {
        WindowState state;

        try
        {
            std::ifstream file(filePath);
            if (!file.is_open())
            {
                LOG_INFO("WindowStatePersistence: No state file found at '{}', using defaults", filePath);
                return state;
            }

            nlohmann::json json;
            file >> json;

            if (json.contains("posX"))
                state.posX = json["posX"];
            if (json.contains("posY"))
                state.posY = json["posY"];
            if (json.contains("width"))
                state.width = json["width"];
            if (json.contains("height"))
                state.height = json["height"];
            if (json.contains("isMaximized"))
                state.isMaximized = json["isMaximized"];

            LOG_INFO("WindowStatePersistence: Loaded window state from '{}'", filePath);
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("WindowStatePersistence: Failed to load state from '{}': {}", filePath, e.what());
        }

        return state;
    }

    bool WindowStatePersistence::Save(const std::string &filePath, const WindowState &state)
    {
        try
        {
            nlohmann::json json;
            json["posX"] = state.posX;
            json["posY"] = state.posY;
            json["width"] = state.width;
            json["height"] = state.height;
            json["isMaximized"] = state.isMaximized;

            std::ofstream file(filePath);
            if (!file.is_open())
            {
                LOG_ERROR("WindowStatePersistence: Failed to open '{}' for writing", filePath);
                return false;
            }

            file << json.dump(4);
            LOG_INFO("WindowStatePersistence: Saved window state to '{}'", filePath);
            return true;
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("WindowStatePersistence: Failed to save state to '{}': {}", filePath, e.what());
            return false;
        }
    }

    bool WindowStatePersistence::LoadAndApply(Window &window, const std::string &filePath)
    {
        WindowState state = Load(filePath);

        bool isDefaultState = (state.posX == -1 && state.posY == -1 && state.width == 1280 && state.height == 720);

        window.SetState(state);

        return !isDefaultState;
    }

    bool WindowStatePersistence::CaptureAndSave(const Window &window, const std::string &filePath)
    {
        WindowState state = window.GetState();
        return Save(filePath, state);
    }
} // namespace Kappa
