#pragma once
#include "defines.h"

#if defined(PLATFORM_LINUX)

#include <cstring>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wayland-client.h>

#include "platform.h"
#include "xdg-shell-protocol.h"

namespace Engine {
    class LinuxPlatform : public Platform {
        public:
            LinuxPlatform(const Platform &) = delete;
            void operator=(LinuxPlatform const &) = delete;

            LinuxPlatform();

            StatusCode CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height);
            StatusCode CloseWindow();
            bool PollForEvents();
            void SleepForDuration(u64 duration);

            const inline void IssueTerminateCommand() {
                mCloseWindow = true;
            }

            const inline void SetCompositor(wl_compositor *compositor) {
                mpCompositor = compositor;
            }

            const inline void SetSharedMemory(wl_shm *shm) {
                mpSharedMemory = shm;
            }

            const inline void SetSeat(wl_seat *seat) {
                mpSeat = seat;
            }

            const inline void SetWmBase(xdg_wm_base *wmBase) {
                mpWmBase = wmBase;
            }

            inline wl_seat *GetSeat() const {
                return mpSeat;
            }

            inline const wl_seat_listener *GetSeatListener() {
                return &mSeatListener;
            }

            inline xdg_wm_base *GetWmBase() const {
                return mpWmBase;
            }

            inline const xdg_wm_base_listener *GetWmBaseListener() {
                return &mpXdgShellListener;
            }

            bool mCloseWindow;
            int mWindowWidth;
            int mWindowHeight;
            const char *mpWindowName;

            // private:
            bool mInitialized;

            wl_display *mpDisplay;
            wl_surface *mpSurface;
            wl_compositor *mpCompositor = NULL;
            wl_shm *mpSharedMemory;
            wl_seat *mpSeat;
            xdg_wm_base *mpWmBase;
            xdg_toplevel *mpXdgTopLevelObject;
            xdg_surface *mpXdgSurface;
            wl_keyboard *mpKeyboard;
            wl_buffer *mpBuffer;
            void *mpPixel;

            wl_registry_listener mRegistryListener;
            wl_callback_listener mCallbackListener;
            wl_seat_listener mSeatListener;
            xdg_surface_listener mSurfaceListener;
            xdg_wm_base_listener mpXdgShellListener;
            xdg_toplevel_listener mTopLevelListener;
            wl_keyboard_listener mKeyboardListener;

            // void (*OnGlobalObjectAvailable)(void *data, wl_registry *reg, uint32_t name, const char *intf, uint32_t
            // v); void (*OnGlobalObjectRemoval)(void *data, struct wl_registry *reg, uint32_t name); void
            // (*DrawNewFrame)(void *data, wl_callback *cb, uint32_t a); void (*SeatCapabilities)(void *data, struct
            // wl_seat *seat, uint32_t cap); void (*SeatName)(void *data, struct wl_seat *seat, const char *name); void
            // (*ConfigureSurface)(void *data, xdg_surface *xdg_surface, uint32_t serial); void (*ShellPing)(void *data,
            // struct xdg_wm_base *sh, uint32_t ser); void (*ConfigureTopLevelObject)(void *data, xdg_toplevel *top,
            // int32_t nw, int32_t nh, wl_array *stat); void (*CloseTopLevelObject)(void *data, struct xdg_toplevel
            // *top); void (*ResizeWindow)(); void (*Draw)(); int (*AllocateSharedMemory)(uint64_t sz);
    };
}; // namespace Engine

#endif
