#pragma once
#include "log_level.h"
#include "core/status_code.h"

#if defined(_DEBUG)
#define LOG_WARN_ENABLED = 1
#define LOG_INFO_ENABLED = 1
#define LOG_DEBUG_ENABLED = 1
#define LOG_TRACE_ENABLED = 1
#else
#define LOG_DEBUG_ENABLED = 0
#define LOG_TRACE_ENABLED = 0
#endif

namespace Engine {
    class Logger {
        public:
            Engine::StatusCode initialize();
            void log_message();
            Engine::StatusCode terminate();
    };
}; // namespace Engine
