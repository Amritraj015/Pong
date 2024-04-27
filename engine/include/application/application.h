#pragma once

namespace Engine {
    class Application {
        public:
            virtual ~Application() = default;

            // Starting position of the window on x-axis.
            int windowStartX = 0;

            // Starting position of the window on x-axis.
            int windowStartY = 0;

            // Starting width of the window.
            int windowHeight = 0;

            // Starting width of the window.
            int windowWidth = 0;

            // The application name on the window.
            const char *name;
    };
} // namespace Engine
