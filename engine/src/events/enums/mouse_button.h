#pragma once

#include "defines.h"

namespace Engine {
    enum class MouseButton : u8 {
        Left = 0,            // The left mouse button.
        ScrollWheel = 1,     // The middle mouse button.
        Right = 2,           // The right mouse button.
        ScrollWheelUp = 3,   // Scroll wheel up
        ScrollWheelDown = 4, // Scroll wheel down
        Unknown = 5          // Unknown/Unsupported button
    };
}
