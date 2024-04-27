#include "defines.h"

#ifdef PLATFORM_LINUX

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

#include "linux_platform.h"
#include "xdg-shell-protocol.h"
#include "logger/logger.h"

namespace Engine {
    void keyboard_mapping(void *data, wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz) {
    }

    void on_keyboard_enter(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *srfc, wl_array *keys) {
    }

    void on_keyboard_leave(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *srfc) {
    }

    void on_key_press(void *data, wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t state) {
        if (key == 1) {
            ((LinuxPlatform *)data)->IssueTerminateCommand();
        }

        if (state == 1)
            LDEBUG("Key down: %i", static_cast<char>(key))
        else
            LDEBUG("Key up: %i", static_cast<char>(key))
    }

    void keyboard_modifiers(void *data, wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t grp) {
    }

    void keyboard_repeat_info(void *data, wl_keyboard *kb, int32_t rate, int32_t del) {
    }

    void seat_capabilities(void *data, wl_seat *seat, uint32_t cap) {
        if (cap & WL_SEAT_CAPABILITY_KEYBOARD && !((LinuxPlatform *)data)->GetKeyboard()) {
            ((LinuxPlatform *)data)->SetKeyboard(wl_seat_get_keyboard(seat));
            wl_keyboard_add_listener(((LinuxPlatform *)data)->GetKeyboard(), ((LinuxPlatform *)data)->GetKeyboardListener(), data);
        }
    }

    int allocate_shared_memory(LinuxPlatform *platform, uint64_t sz) {
        int fd = shm_open(platform->mpWindowName, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);

        shm_unlink(platform->mpWindowName);
        ftruncate(fd, sz);

        return fd;
    }

    void draw(LinuxPlatform *platform) {
        memset(platform->GetPixels(), 100, platform->mWindowWidth * platform->mWindowHeight * 4);

        wl_surface_attach(platform->GetSurface(), platform->GetBuffer(), 0, 0);
        wl_surface_damage_buffer(platform->GetSurface(), 0, 0, platform->mWindowWidth, platform->mWindowHeight);
        wl_surface_commit(platform->GetSurface());
    }

    void resize_window(LinuxPlatform *platform) {
        int size = platform->mWindowHeight * platform->mWindowWidth * 4;
        int fd = allocate_shared_memory(platform, size);

        platform->SetPixels(mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        wl_shm_pool *pool = wl_shm_create_pool(platform->GetSharedMemory(), fd, size);

        platform->SetBuffer(wl_shm_pool_create_buffer(pool, 0, platform->mWindowWidth, platform->mWindowHeight, platform->mWindowWidth * 4,
                                                      WL_SHM_FORMAT_ARGB8888));

        wl_shm_pool_destroy(pool);

        close(fd);
    }

    void draw_new_frame(void *data, wl_callback *cb, uint32_t a) {
        wl_callback_destroy(cb);

        LinuxPlatform *platform = (LinuxPlatform *)data;

        cb = wl_surface_frame(platform->GetSurface());
        wl_callback_add_listener(cb, platform->GetCallbackListener(), platform);

        draw(platform);
    }

    void shell_ping(void *data, xdg_wm_base *sh, uint32_t ser) {
        xdg_wm_base_pong(sh, ser);
    }

    void seat_name(void *data, struct wl_seat *seat, const char *name) {
    }

    void on_global_object_available(void *data, wl_registry *reg, uint32_t name, const char *intf, uint32_t v) {
        if (strcmp(intf, wl_compositor_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetCompositor((wl_compositor *)wl_registry_bind(reg, name, &wl_compositor_interface, 4));
        } else if (strcmp(intf, wl_shm_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetSharedMemory((wl_shm *)wl_registry_bind(reg, name, &wl_shm_interface, 1));
        } else if (strcmp(intf, xdg_wm_base_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetWmBase((xdg_wm_base *)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(((LinuxPlatform *)data)->GetWmBase(), ((LinuxPlatform *)data)->GetWmBaseListener(), data);
        } else if (strcmp(intf, wl_seat_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetSeat((wl_seat *)wl_registry_bind(reg, name, &wl_seat_interface, 1));
            wl_seat_add_listener(((LinuxPlatform *)data)->GetSeat(), ((LinuxPlatform *)data)->GetSeatListener(), data);
        }
    }

    void on_global_object_removal(void *data, wl_registry *reg, uint32_t name) {
    }

    void configure_surface(void *data, xdg_surface *xdg_surface, uint32_t serial) {
        xdg_surface_ack_configure(xdg_surface, serial);

        LinuxPlatform *platform = (LinuxPlatform *)data;

        if (!platform->GetPixels()) resize_window(platform);

        draw(platform);
    }

    void configure_top_level_object(void *data, xdg_toplevel *top, int32_t nw, int32_t nh, wl_array *stat) {
        if (!nw && !nh) {
            return;
        }

        LinuxPlatform *platform = (LinuxPlatform *)data;

        if (platform->mWindowWidth != nw || platform->mWindowHeight != nh) {
            munmap(platform->GetPixels(), platform->mWindowWidth * platform->mWindowHeight * 4);

            platform->mWindowWidth = nw;
            platform->mWindowHeight = nh;

            resize_window(platform);
        }
    }

    void close_top_level_object(void *data, struct xdg_toplevel *top) {
        ((LinuxPlatform *)data)->IssueTerminateCommand();
    }

    LinuxPlatform::LinuxPlatform() {
        mCloseWindow = false;
        mInitialized = false;

        mRegistryListener = { .global = on_global_object_available, .global_remove = on_global_object_removal };
        mCallbackListener = { .done = draw_new_frame };
        mSeatListener = { .capabilities = seat_capabilities, .name = seat_name };
        mSurfaceListener = { .configure = configure_surface };
        mpXdgShellListener = { .ping = shell_ping };
        mTopLevelListener = { .configure = configure_top_level_object, .close = close_top_level_object };
        mKeyboardListener = { .keymap = keyboard_mapping,
                              .enter = on_keyboard_enter,
                              .leave = on_keyboard_leave,
                              .key = on_key_press,
                              .modifiers = keyboard_modifiers,
                              .repeat_info = keyboard_repeat_info };
    }

    StatusCode LinuxPlatform::CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height) {
        // Make sure the platform is not initialized already.
        if (mInitialized) return StatusCode::PlatformAlreadyInitialized;

        // Mark the platform as initialized and store the height,
        // width and name of the window to be created internally.
        mInitialized = true;
        mWindowWidth = width;
        mWindowHeight = height;
        mpWindowName = windowName;

        // Let's try and connect to the display.
        mpDisplay = wl_display_connect(NULL);

        // If we couldn't connect to the display then return with an error status.
        if (NULL == mpDisplay) return StatusCode::WaylandCannotConnectToDisplay;

        LTRACE("Connected to display")

        // Now, lets get the Wayland registry.
        wl_registry *registry = wl_display_get_registry(mpDisplay);

        // Add a listeners to the registry.
        wl_registry_add_listener(registry, &mRegistryListener, this);
        wl_display_dispatch(mpDisplay);
        wl_display_roundtrip(mpDisplay);

        // If we couldn't find a Wayland compositor then return with an error status.
        if (NULL == mpCompositor) return StatusCode::WaylandCannotFindCompositor;

        LTRACE("Wayland compositor found")

        // Now, lets create a surface.
        mpSurface = wl_compositor_create_surface(mpCompositor);

        // If surface creation failed then return with an error.
        if (mpSurface == NULL) return StatusCode::WaylandCannotCreateSurface;

        LTRACE("Created surface")

        wl_callback *callback = wl_surface_frame(mpSurface);
        wl_callback_add_listener(callback, &mCallbackListener, this);

        mpXdgSurface = xdg_wm_base_get_xdg_surface(mpWmBase, mpSurface);
        xdg_surface_add_listener(mpXdgSurface, &mSurfaceListener, this);
        mpXdgTopLevelObject = xdg_surface_get_toplevel(mpXdgSurface);
        xdg_toplevel_add_listener(mpXdgTopLevelObject, &mTopLevelListener, this);
        xdg_toplevel_set_title(mpXdgTopLevelObject, windowName);
        wl_surface_commit(mpSurface);

        while (wl_display_dispatch(mpDisplay)) {
            if (mCloseWindow) break;
        }

        return StatusCode::Successful;
    }

    StatusCode LinuxPlatform::CloseWindow() {
        // Disposing keyboard structure.
        if (mpKeyboard) wl_keyboard_destroy(mpKeyboard);

        LTRACE("Destroyed keyboard structure")

        wl_seat_release(mpSeat);

        LTRACE("Released seat resources")

        // Destroy buffer.
        if (mpBuffer) wl_buffer_destroy(mpBuffer);

        LTRACE("Destroyed buffer")

        // Destroy XDG top level object.
        xdg_toplevel_destroy(mpXdgTopLevelObject);

        LTRACE("Destroyed XDG top level object")

        // Destroy XDG surface.
        xdg_surface_destroy(mpXdgSurface);

        LTRACE("Destroyed XDG surface")

        // Destroy surface.
        wl_surface_destroy(mpSurface);

        LTRACE("Destroyed surface")

        // Disconnect from display.
        wl_display_disconnect(mpDisplay);

        LTRACE("Disconnected from display")

        // Set the initialized flag as false.
        mInitialized = false;

        // Finally, return a successful response.
        return StatusCode::Successful;
    }

    bool LinuxPlatform::PollForEvents() {
        return true;
    }

    void LinuxPlatform::SleepForDuration(u64 duration) {
    }
} // namespace Engine

#endif
