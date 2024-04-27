#include "logger/logger.h"
#include "defines.h"

namespace Engine {
    StatusCode initialize_logger() {
        return StatusCode::Successful;
    }

    void log_message(LogLevel logLevel, const char *message, ...) {
        const char *logLevelStrings[6] = {"[FETAL] ", "[ERROR] ", "[WARN] ", "[INFO] ", "[DEBUG] ", "[TRACE] "};
        const char *colorStrings[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "0;00"};

        // Impose a 32k character limit on each log entry,
        char outMessage[32000];
        char outMessage2[32000];
        memset(outMessage, 0, sizeof(outMessage));

        __builtin_va_list argPtr;
        va_start(argPtr, message);
        vsnprintf(outMessage, 32000, message, argPtr);
        va_end(argPtr);

        const u8 levelIndex = to_underlying(logLevel);
        sprintf(outMessage2, "%s%s\n", logLevelStrings[levelIndex], outMessage);
        printf("\033[%sm%s\033[0m", colorStrings[levelIndex], outMessage2);
    }

    StatusCode terminate_logger() {
        return StatusCode::Successful;
    }
}; // namespace Engine
