#pragma once

#include "Event.h"

#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Kappa
{
    /**
     * @brief Event bus for publish-subscribe communication between layers.
     */
    class EventBus
    {
    public:
        /**
         * @brief Subscribes to events of a specific type.
         * @tparam TEvent Event type (must derive from Event)
         * @param callback Callback function
         */
        template<typename TEvent>
            requires std::is_base_of_v<Event, TEvent>
        void Subscribe(std::function<void(const TEvent &)> callback)
        {
            std::lock_guard<std::mutex> lock(subscribersMutex);
            const auto typeIndex = std::type_index(typeid(TEvent));
            auto wrapper = [callback](const Event &event) {
                if (const auto *specEvent = dynamic_cast<const TEvent *>(&event))
                {
                    callback(*specEvent);
                }
            };
            subscribers[typeIndex].push_back(wrapper);
        }

        /**
         * @brief Publishes an event to all subscribers.
         * @tparam TEvent Event type (must derive from Event)
         * @param event Event to publish
         */
        template<typename TEvent>
            requires std::is_base_of_v<Event, TEvent>
        void Publish(const TEvent &event)
        {
            std::vector<EventCallback> handlers;
            {
                std::lock_guard<std::mutex> lock(subscribersMutex);
                const auto typeIndex = std::type_index(typeid(TEvent));
                if (const auto it = subscribers.find(typeIndex); it != subscribers.end())
                {
                    handlers = it->second;
                }
            }

            for (const auto &callback : handlers)
            {
                callback(event);
            }
        }

        /**
         * @brief Clears all subscribers.
         */
        void Clear()
        {
            std::lock_guard<std::mutex> lock(subscribersMutex);
            subscribers.clear();
        }

    private:
        using EventCallback = std::function<void(const Event &)>;
        std::unordered_map<std::type_index, std::vector<EventCallback>> subscribers;
        mutable std::mutex subscribersMutex;
    };
} // namespace Kappa
