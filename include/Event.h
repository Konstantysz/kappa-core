#pragma once

namespace Kappa
{
    /**
     * @brief Base class for all events in the application.
     */
    class Event
    {
    public:
        virtual ~Event() = default;
    };
} // namespace Kappa