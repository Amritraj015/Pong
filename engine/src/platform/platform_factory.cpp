#include "defines.h"
#include "platform/linux/wayland_platform.h"
#include "platform/windows/windows_platform.h"
#include "platform_factory.h"

namespace Engine {
    Engine::Platform *get_platform() {

#if defined(PLATFORM_LINUX)

        // If the current platform is Linux then return an instance of WaylandPlatform .
        static WaylandPlatform platform;
        return &platform;

#elif defined(PLATFORM_WINDOWS)

        // If the current platform is Windows then return an instance of WindowsPlatform.
        static WindowsPlatform platform;
        return &platform;

#endif

        return nullptr;
    }
} // namespace Engine
