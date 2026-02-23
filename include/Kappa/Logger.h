#pragma once

#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

namespace Kappa
{
    /**
     * @brief Logging levels.
     */
    enum class LogLevel
    {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warn = 3,
        Error = 4,
        Critical = 5,
        Off = 6
    };

    /**
     * @brief Type-safe logging wrapper around spdlog.
     */
    class Logger
    {
    public:
        /**
         * @brief Returns the logger instance.
         * @return Logger instance
         */
        static Logger &Get();

        /**
         * @brief Sets the logger name (must be called before first Get()).
         * @param name Logger name to display in logs
         */
        static void SetLoggerName(const std::string &name);

    public:
        /**
         * @brief Trace logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args> void Trace(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Trace, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Debug logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args> void Debug(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Debug, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Info logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args> void Info(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Info, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Warning logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args> void Warn(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Warn, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Error logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args> void Error(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Error, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Critical logging.
         * @tparam Args Format argument types
         * @param loc Source location
         * @param format Format string
         * @param args Format arguments
         */
        template<typename... Args>
        void Critical(const std::source_location &loc, std::string_view format, Args &&...args)
        {
            LogInternal(LogLevel::Critical, loc, format, std::make_format_args(args...));
        }

        /**
         * @brief Flushes the logger.
         */
        void Flush();

        /**
         * @brief Sets the log level.
         * @param level Log level
         */
        void SetLevel(LogLevel level);

    private:
        Logger();
        ~Logger();

        void LogInternal(LogLevel level,
            const std::source_location &loc,
            std::string_view format,
            std::format_args args);

        static std::string &GetLoggerName();
        static std::string_view GetFileName(const std::source_location &loc);

        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
} // namespace Kappa

/**
 * @brief Trace logging macro.
 * @param ... Format string and arguments
 */
#define LOG_TRACE(...) ::Kappa::Logger::Get().Trace(std::source_location::current(), __VA_ARGS__)

/**
 * @brief Debug logging macro.
 * @param ... Format string and arguments
 */
#define LOG_DEBUG(...) ::Kappa::Logger::Get().Debug(std::source_location::current(), __VA_ARGS__)

/**
 * @brief Info logging macro.
 * @param ... Format string and arguments
 */
#define LOG_INFO(...) ::Kappa::Logger::Get().Info(std::source_location::current(), __VA_ARGS__)

/**
 * @brief Warning logging macro.
 * @param ... Format string and arguments
 */
#define LOG_WARN(...) ::Kappa::Logger::Get().Warn(std::source_location::current(), __VA_ARGS__)

/**
 * @brief Error logging macro.
 * @param ... Format string and arguments
 */
#define LOG_ERROR(...) ::Kappa::Logger::Get().Error(std::source_location::current(), __VA_ARGS__)

/**
 * @brief Critical logging macro.
 * @param ... Format string and arguments
 */
#define LOG_CRITICAL(...) ::Kappa::Logger::Get().Critical(std::source_location::current(), __VA_ARGS__)
