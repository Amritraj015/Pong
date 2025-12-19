#include "defines.h"
#include "platform_factory.h"

#if defined(PLATFORM_LINUX)

#include "platform/linux/wayland_platform.h"

namespace Engine {
    Engine::Platform *get_platform() {
        // If the current platform is Linux then return an instance of WaylandPlatform .
        static WaylandPlatform platform;
        return &platform;
    }
} // namespace Engine

#elif defined(PLATFORM_WINDOWS)

#include "platform/windows/windows_platform.h"

namespace Engine {
    Engine::Platform *get_platform() {
        // If the current platform is Windows then return an instance of WindowsPlatform.
        static WindowsPlatform platform;
        return &platform;
    }
} // namespace Engine

#else 

namespace Engine {
    Engine::Platform *get_platform() {
        return nullptr;
    }
} // namespace Engine

#endif
