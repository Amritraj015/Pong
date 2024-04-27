#include "defines.h"
#include "platform/linux/wayland_platform.h"
#include "platform/windows/windows_platform.h"
#include "platform_factory.h"

namespace Engine {
    std::shared_ptr<Engine::Platform> get_platform() {

#if defined(PLATFORM_LINUX)

        // If the current platform is Linux then return an instance of WaylandPlatform .
        return std::make_shared<Engine::WaylandPlatform>();

#elif defined(PLATFORM_WINDOWS)

        // If the current platform is Windows then return an instance of WindowsPlatform.
        return std::make_shared<Engine::WindowsPlatform>();

#endif

        return nullptr;
    }
} // namespace Engine
