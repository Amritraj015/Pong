#pragma once

namespace Engine {
    /// @brief This `enum` defines status codes to represent various operation results.
    enum class StatusCode {
        Successful,                 // Operation Successful.
        UnsupportedPlatform,        // Unsupported platform error.
        InvalidApplication,         // Invalid application error.
        PlatformAlreadyInitialized, // Platform already initialized error.
        WindowCreationFailed,       // Window creation failed error.
        WindowCloseFailed,          // Window close failed error.
    };
} // namespace Engine
