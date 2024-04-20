namespace Engine {
    enum struct LogLevel {
        // A variant that can be used to log fetal errors.
        FETAL = 0,

        // A variant that can be used to log errors.
        ERROR = 1,

        // A variant that can be used to log warnings.
        WARN = 2,

        // A variant that can be used to log helpful information.
        INFO = 3,

        // A variant that can be used to log debug information.
        DEBUG = 4,

        // A variant that can be used for trace logs.
        TRACE = 5
    };
};
