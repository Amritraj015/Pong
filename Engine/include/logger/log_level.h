#pragma once

namespace Engine {
    enum struct LogLevel {
        // A variant that can be used to log fetal errors.
        Fatal = 0,

        // A variant that can be used to log errors.
        Error = 1,

        // A variant that can be used to log warnings.
        Warn = 2,

        // A variant that can be used to log helpful information.
        Info = 3,

        // A variant that can be used to log debug information.
        Debug = 4,

        // A variant that can be used for trace logs.
        Trace = 5
    };
};
