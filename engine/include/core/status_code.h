#pragma once

namespace Engine {
    /// @brief This `enum` defines status codes to represent various operation results.
    enum class StatusCode {
        Successful,                        // Operation Successful.
        UnsupportedPlatform,               // Unsupported platform error.
        InvalidApplication,                // Invalid application error.
        PlatformAlreadyInitialized,        // Platform already initialized error.
        WaylandCannotConnectToDisplay,     // Linux (Wayland) connect to display.
        WaylandCannotFindCompositor,       // Linux (Wayland) cannot find compositor.
        WaylandCannotCreateSurface,        // Linux (Wayland) cannot create surface.
        WaylandCannotCreateXdgSurface,     // Linux (Wayland) cannot create XDG surface.
        WaylandCannotCreateTopLevelWindow, // Linux (Wayland) cannot create top level window.
    };
} // namespace Engine
