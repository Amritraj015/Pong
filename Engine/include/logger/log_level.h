namespace Engine {
    enum LogLevel {
        // A variant that can be used to log fetal errors.
        FETAL = 1,

        // A variant that can be used to log errors.
        ERROR = 2,

        // A variant that can be used to log warnings.
        WARN = 3,

        // A variant that can be used to log helpful information.
        INFO = 4,

        // A variant that can be used to log debug information.
        DEBUG = 5,

        // A variant that can be used for trace logs.
        TRACE = 6
    };
};
