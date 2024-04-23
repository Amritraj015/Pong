#include "defines.h"
#include "linux_platform.h"
#include "windows_platform.h"
#include "platform_factory.h"

namespace Engine {
    std::shared_ptr<Engine::Platform> get_platform() {

#if defined(PLATFORM_LINUX)

        // If the current platform is Linux then return an instance of LinuxPlatform.
        return std::make_shared<Engine::LinuxPlatform>();

#elif defined(PLATFORM_WINDOWS)

        // If the current platform is Windows then return an instance of WindowsPlatform.
        return std::make_shared<Engine::WindowsPlatform>();

#endif

        return nullptr;
    }
} // namespace Engine
