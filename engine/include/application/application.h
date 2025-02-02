#pragma once
#include "defines.h"

namespace Engine {
    class Application {
        public:
            virtual ~Application() = default;

            // Starting position of the window on x-axis.
            u32 windowStartX = 0;

            // Starting position of the window on x-axis.
            u32 windowStartY = 0;

            // Starting width of the window.
            u32 windowHeight = 0;

            // Starting width of the window.
            u32 windowWidth = 0;

            // The application name on the window.
            const char *name;
    };
} // namespace Engine
