#pragma once

namespace Engine
{
    /// @brief This `enum` defines status codes to represent various operation results.
    enum class StatusCode
    {
        Successful = 0,           // Operation Successful.
        WindowCreationFailed = 1, // Represents an error that could occur on window creation.
        WindowCloseFailed = 2,    // Represents an error that could occur while closing the window.
    };
}