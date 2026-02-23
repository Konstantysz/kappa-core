#include "Kappa/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Kappa
{
    struct Logger::Impl
    {
        std::shared_ptr<spdlog::logger> logger;
    };

    Logger::Logger() : impl_(std::make_unique<Impl>())
    {
        impl_->logger = spdlog::stdout_color_mt(GetLoggerName());
        // Default to info level
        impl_->logger->set_level(spdlog::level::info);
    }

    Logger::~Logger() = default;

    Logger &Logger::Get()
    {
        static Logger instance;
        return instance;
    }

    void Logger::SetLoggerName(const std::string &name)
    {
        GetLoggerName() = name;
    }

    std::string &Logger::GetLoggerName()
    {
        static std::string loggerName = "Kappa";
        return loggerName;
    }

    void Logger::SetLevel(LogLevel level)
    {
        impl_->logger->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    void Logger::Flush()
    {
        impl_->logger->flush();
    }

    void Logger::LogInternal(LogLevel level, const std::source_location &loc, std::string_view format, std::format_args args)
    {
        auto message = std::vformat(format, args);
        impl_->logger->log(
            static_cast<spdlog::level::level_enum>(level), "[{}:{}] {}", GetFileName(loc), loc.line(), message);
    }

    std::string_view Logger::GetFileName(const std::source_location &loc)
    {
        const char *path = loc.file_name();
        const char *filename = path;

        for (const char *p = path; *p; ++p)
        {
            if (*p == '/' || *p == '\\')
            {
                filename = p + 1;
            }
        }
        return filename;
    }
} // namespace Kappa
