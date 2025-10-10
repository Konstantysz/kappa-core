#pragma once

#define KAPPA_CORE_VERSION_MAJOR 0
#define KAPPA_CORE_VERSION_MINOR 3
#define KAPPA_CORE_VERSION_PATCH 0

#define KAPPA_CORE_VERSION_STRING "0.3.0"

namespace Kappa
{
    /**
     * @brief Version information for kappa-core.
     */
    struct Version
    {
        static constexpr int Major = KAPPA_CORE_VERSION_MAJOR;
        static constexpr int Minor = KAPPA_CORE_VERSION_MINOR;
        static constexpr int Patch = KAPPA_CORE_VERSION_PATCH;
        static constexpr const char *String = KAPPA_CORE_VERSION_STRING;
    };
} // namespace Kappa
