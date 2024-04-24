#pragma once
#include "log_level.h"
#include "core/status_code.h"

#if defined(_DEBUG)
#define LOG_WARN_ENABLED
#define LOG_INFO_ENABLED
#define LOG_DEBUG_ENABLED
#define LOG_TRACE_ENABLED
#endif

namespace Engine {
    StatusCode initialize_logger();
    void log_message(LogLevel logLevel, const char *message, ...);
    StatusCode terminate_logger();

// Logs a fatal message.
#define LFATAL(message, ...) log_message(Engine::LogLevel::Fatal, message, ##__VA_ARGS__);

// Logs an error message.
#define LERROR(message, ...) log_message(Engine::LogLevel::Error, message, ##__VA_ARGS__);

#ifdef LOG_WARN_ENABLED
// Logs a warning message.
#define LWARN(message, ...) log_message(Engine::LogLevel::Warn, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define LWARN(message, ...)
#endif

#ifdef LOG_INFO_ENABLED
/* Logs a info message. */
#define LINFO(message, ...) log_message(Engine::LogLevel::Info, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_INFO_ENABLED != 1
#define LINFO(message, ...)
#endif

#ifdef LOG_DEBUG_ENABLED
// Logs a debug message.
#define LDEBUG(message, ...) log_message(Engine::LogLevel::Debug, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_DEBUG_ENABLED != 1
#define LDEBUG(message, ...)
#endif

#ifdef LOG_TRACE_ENABLED
// Logs a debug message.
#define LTRACE(message, ...) log_message(Engine::LogLevel::Trace, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_DEBUG_ENABLED != 1
#define LTRACE(message, ...)
#endif
}; // namespace Engine
