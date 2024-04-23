#pragma once

#include "defines.h"

#if defined(PLATFORM_WINDOWS)

#include "platform/platform.h"

namespace Engine {
    class WindowsPlatform : public Platform {};
}; // namespace Engine

#endif
