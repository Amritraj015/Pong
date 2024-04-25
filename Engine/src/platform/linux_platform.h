#pragma once
#include "defines.h"

#if defined(PLATFORM_LINUX)

#include "platform.h"
#include "xdg-shell-protocol.h"

namespace Engine {
    class LinuxPlatform : public Platform {
        private:
            bool mInitialized;
            bool mCloseWindow;

            wl_display *mpDisplay;
            wl_surface *mpSurface;
            wl_compositor *mpCompositor = NULL;
            wl_shm *mpSharedMemory;
            wl_seat *mpSeat;
            xdg_wm_base *mpWmBase;
            xdg_toplevel *mpXdgTopLevelObject;
            xdg_surface *mpXdgSurface;

            wl_registry_listener mRegistryListener;
            wl_callback_listener mCallbackListener;
            wl_seat_listener mSeatListener;
            xdg_surface_listener mSurfaceListener;
            xdg_wm_base_listener mpXdgShellListener;
            xdg_toplevel_listener mTopLevelListener;

            // void OnGlobalObjectAvailable(void *data, wl_registry *reg, uint32_t name, const char *intf, uint32_t v);
            // void OnGlobalObjectRemoval(void *data, struct wl_registry *reg, uint32_t name);

        public:
            LinuxPlatform(const Platform &) = delete;
            void operator=(LinuxPlatform const &) = delete;

            LinuxPlatform();

            StatusCode CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height);

            StatusCode CloseWindow();

            bool PollForEvents();

            void SleepForDuration(u64 duration);
    };
}; // namespace Engine

#endif
