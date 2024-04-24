#pragma once

namespace Engine {
    enum struct LogLevel {
        Fatal = 0, // A variant that can be used to log fatal errors.
        Error = 1, // A variant that can be used to log errors.
        Warn = 2,  // A variant that can be used to log warnings.
        Info = 3,  // A variant that can be used to log helpful information.
        Debug = 4, // A variant that can be used to log debug information.
        Trace = 5, // A variant that can be used for trace logs.
    };
};
