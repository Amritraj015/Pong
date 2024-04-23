#pragma once
#include "defines.h"

#if defined(PLATFORM_LINUX)

#include "platform.h"
#include "xdg-shell-protocol.h"

namespace Engine {
    class LinuxPlatform : public Platform {
        private:
            wl_display *display = NULL;
            wl_compositor *compositor = NULL;
            wl_surface *surface;
            wl_shell *shell;
            wl_shell_surface *shell_surface;
            wl_shm *shm;
            wl_buffer *buffer;
            wl_callback *frame_callback;
            bool initialized;

        public:
            StatusCode CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height);

            StatusCode CloseWindow();

            bool PollForEvents();

            void SleepForDuration(u64 duration);
    };
}; // namespace Engine

#endif
