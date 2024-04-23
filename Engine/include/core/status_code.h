#pragma once

namespace Engine {
    /// @brief This `enum` defines status codes to represent various operation results.
    enum class StatusCode {
        Successful,                 // Operation Successful.
        PlatformAlreadyInitialized, // Platform already initialized error.
        WindowCreationFailed,       // Window creation failed error.
        WindowCloseFailed,          // Window close failed error.
    };
} // namespace Engine
