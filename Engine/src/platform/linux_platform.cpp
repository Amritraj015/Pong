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
    void kb_map(void *data, wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz) {
    }

    void on_keyboard_enter(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *srfc, wl_array *keys) {
    }

    void on_keyboard_leave(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *srfc) {
    }

    void on_key_press(void *data, wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t stat) {
        if (key == 1) {
            ((LinuxPlatform *)data)->IssueTerminateCommand();
        } else if (key == 30) {
            printf("a\n");
        } else if (key == 32) {
            printf("d\n");
        }
    }

    void kb_mod(void *data, wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t grp) {
    }

    void kb_rep(void *data, wl_keyboard *kb, int32_t rate, int32_t del) {
    }

    void SeatCapabilities(void *data, wl_seat *seat, uint32_t cap) {
        if (cap & WL_SEAT_CAPABILITY_KEYBOARD && !((LinuxPlatform *)data)->mpKeyboard) {
            ((LinuxPlatform *)data)->mpKeyboard = wl_seat_get_keyboard(seat);
            wl_keyboard_add_listener(((LinuxPlatform *)data)->mpKeyboard, &((LinuxPlatform *)data)->mKeyboardListener, 0);
        }
    }

    int AllocateSharedMemory(LinuxPlatform *platform, uint64_t sz) {
        int fd = shm_open(platform->mpWindowName, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);

        shm_unlink(platform->mpWindowName);
        ftruncate(fd, sz);

        return fd;
    }

    void Draw(LinuxPlatform *platform) {
        memset(platform->mpPixel, 255, platform->mWindowWidth * platform->mWindowHeight * 4);

        wl_surface_attach(platform->mpSurface, platform->mpBuffer, 0, 0);
        wl_surface_damage_buffer(platform->mpSurface, 0, 0, platform->mWindowWidth, platform->mWindowHeight);
        wl_surface_commit(platform->mpSurface);
    }

    void ResizeWindow(LinuxPlatform *platform) {
        int size = platform->mWindowHeight * platform->mWindowWidth * 4;
        int fd = AllocateSharedMemory(platform, size);

        platform->mpPixel = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        wl_shm_pool *pool = wl_shm_create_pool(platform->mpSharedMemory, fd, size);

        platform->mpBuffer = wl_shm_pool_create_buffer(pool, 0, platform->mWindowWidth, platform->mWindowHeight, platform->mWindowWidth * 4,
                                                       WL_SHM_FORMAT_ARGB8888);

        wl_shm_pool_destroy(pool);

        close(fd);
    }

    void DrawNewFrame(void *data, wl_callback *cb, uint32_t a) {
        wl_callback_destroy(cb);

        LinuxPlatform *platform = (LinuxPlatform *)data;

        cb = wl_surface_frame(platform->mpSurface);
        wl_callback_add_listener(cb, &platform->mCallbackListener, 0);

        Draw(platform);
    }

    void ShellPing(void *data, xdg_wm_base *sh, uint32_t ser) {
        xdg_wm_base_pong(sh, ser);
    }

    void SeatName(void *data, struct wl_seat *seat, const char *name) {
    }

    void OnGlobalObjectAvailable(void *data, wl_registry *reg, uint32_t name, const char *intf, uint32_t v) {
        if (strcmp(intf, wl_compositor_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetCompositor((wl_compositor *)wl_registry_bind(reg, name, &wl_compositor_interface, 4));
        } else if (strcmp(intf, wl_shm_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetSharedMemory((wl_shm *)wl_registry_bind(reg, name, &wl_shm_interface, 1));
        } else if (strcmp(intf, xdg_wm_base_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetWmBase((xdg_wm_base *)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(((LinuxPlatform *)data)->GetWmBase(), ((LinuxPlatform *)data)->GetWmBaseListener(), 0);
        } else if (strcmp(intf, wl_seat_interface.name) == 0) {
            ((LinuxPlatform *)data)->SetSeat((wl_seat *)wl_registry_bind(reg, name, &wl_seat_interface, 1));
            wl_seat_add_listener(((LinuxPlatform *)data)->GetSeat(), ((LinuxPlatform *)data)->GetSeatListener(), 0);
        }
    }

    void OnGlobalObjectRemoval(void *data, wl_registry *reg, uint32_t name) {
    }

    void ConfigureSurface(void *data, xdg_surface *xdg_surface, uint32_t serial) {
        xdg_surface_ack_configure(xdg_surface, serial);

        LinuxPlatform *platform = (LinuxPlatform *)data;

        if (!platform->mpPixel) ResizeWindow(platform);

        Draw(platform);
    }

    void ConfigureTopLevelObject(void *data, xdg_toplevel *top, int32_t nw, int32_t nh, wl_array *stat) {
        if (!nw && !nh) {
            return;
        }

        LinuxPlatform *platform = (LinuxPlatform *)data;

        if (platform->mWindowWidth != nw || platform->mWindowHeight != nh) {
            munmap(platform->mpPixel, platform->mWindowWidth * platform->mWindowHeight * 4);

            platform->mWindowWidth = nw;
            platform->mWindowHeight = nh;

            ResizeWindow(platform);
        }
    }

    void CloseTopLevelObject(void *data, struct xdg_toplevel *top) {
        ((LinuxPlatform *)data)->IssueTerminateCommand();
    }

    LinuxPlatform::LinuxPlatform() {
        mCloseWindow = false;
        mInitialized = false;

        mRegistryListener = { .global = OnGlobalObjectAvailable, .global_remove = OnGlobalObjectRemoval };
        mCallbackListener = { .done = DrawNewFrame };
        mSeatListener = { .capabilities = SeatCapabilities, .name = SeatName };
        mSurfaceListener = { .configure = ConfigureSurface };
        mpXdgShellListener = { .ping = ShellPing };
        mTopLevelListener = { .configure = ConfigureTopLevelObject, .close = CloseTopLevelObject };
        // mKeyboardListener = { .keymap = kb_map,
        //                       .enter = on_keyboard_enter,
        //                       .leave = on_keyboard_leave,
        //                       .key = kb_key,
        //                       .modifiers = kb_mod,
        //                       .repeat_info = kb_rep };
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
        if (mpKeyboard) {
            wl_keyboard_destroy(mpKeyboard);
        }

        LTRACE("Destroyed keyboard structure")

        wl_seat_release(mpSeat);

        LTRACE("Released seat resources")

        // Destroy buffer.
        if (mpBuffer) {
            wl_buffer_destroy(mpBuffer);
        }

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

// =====================================================================================
// #include <cstring>
// #include <fcntl.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <wayland-client.h>
//
// #include "platform/xdg-shell-protocol.h"
//
// struct wl_compositor *comp;
// struct wl_surface *srfc;
// struct wl_buffer *bfr;
// struct wl_shm *shm;
// struct xdg_wm_base *sh;
// struct xdg_toplevel *top;
// struct wl_seat *seat;
// struct wl_keyboard *kb;
// void *pixl;
// int w = 500;
// int h = 500;
// uint8_t cls;
//
// int alc_shm(uint64_t sz) {
//     const char *name = "/testwin";
//
//     int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
//
//     shm_unlink(name);
//     ftruncate(fd, sz);
//
//     return fd;
// }
//
// void resz() {
//     int fd = alc_shm(w * h * 4);
//
//     pixl = mmap(0, w * h * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//
//     struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, w * h * 4);
//     bfr = wl_shm_pool_create_buffer(pool, 0, w, h, w * 4, WL_SHM_FORMAT_ARGB8888);
//     wl_shm_pool_destroy(pool);
//     close(fd);
// }
//
// void draw() {
//     memset(pixl, 255, w * h * 4);
//
//     wl_surface_attach(srfc, bfr, 0, 0);
//     wl_surface_damage_buffer(srfc, 0, 0, w, h);
//     wl_surface_commit(srfc);
// }
//
// struct wl_callback_listener cb_list;
//
// void frame_new(void *data, struct wl_callback *cb, uint32_t a) {
//     wl_callback_destroy(cb);
//     cb = wl_surface_frame(srfc);
//     wl_callback_add_listener(cb, &cb_list, 0);
//
//     draw();
// }
//
// // wl_callback_listener cb_list = {.done = frame_new};
//
// void xrfc_conf(void *data, struct xdg_surface *xrfc, uint32_t ser) {
//     xdg_surface_ack_configure(xrfc, ser);
//     if (!pixl) {
//         resz();
//     }
//
//     draw();
// }
//
// struct xdg_surface_listener xrfc_list = { .configure = xrfc_conf };
//
// void top_conf(void *data, struct xdg_toplevel *top, int32_t nw, int32_t nh, struct wl_array *stat) {
//     if (!nw && !nh) {
//         return;
//     }
//
//     if (w != nw || h != nh) {
//         munmap(pixl, w * h * 4);
//         w = nw;
//         h = nh;
//         resz();
//     }
// }
//
// void top_cls(void *data, struct xdg_toplevel *top) {
//     cls = 1;
// }
//
// struct xdg_toplevel_listener top_list = { .configure = top_conf, .close = top_cls };
//
// void sh_ping(void *data, struct xdg_wm_base *sh, uint32_t ser) {
//     xdg_wm_base_pong(sh, ser);
// }
//
// struct xdg_wm_base_listener sh_list = { .ping = sh_ping };
//
// void kb_map(void *data, struct wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz) {
// }
//
// void kb_enter(void *data, struct wl_keyboard *kb, uint32_t ser, struct wl_surface *srfc, struct wl_array *keys) {
// }
//
// void kb_leave(void *data, struct wl_keyboard *kb, uint32_t ser, struct wl_surface *srfc) {
// }
//
// void kb_key(void *data, struct wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t stat) {
//     if (key == 1) {
//         cls = 1;
//     } else if (key == 30) {
//         printf("a\n");
//     } else if (key == 32) {
//         printf("d\n");
//     }
// }
//
// void kb_mod(void *data, struct wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t
// grp) {
// }
//
// void kb_rep(void *data, struct wl_keyboard *kb, int32_t rate, int32_t del) {
// }
//
// struct wl_keyboard_listener kb_list = {
//     .keymap = kb_map, .enter = kb_enter, .leave = kb_leave, .key = kb_key, .modifiers = kb_mod, .repeat_info = kb_rep
// };
//
// void seat_cap(void *data, struct wl_seat *seat, uint32_t cap) {
//     if (cap & WL_SEAT_CAPABILITY_KEYBOARD && !kb) {
//         kb = wl_seat_get_keyboard(seat);
//         wl_keyboard_add_listener(kb, &kb_list, 0);
//     }
// }
//
// void seat_name(void *data, struct wl_seat *seat, const char *name) {
// }
//
// struct wl_seat_listener seat_list = { .capabilities = seat_cap, .name = seat_name };
//
// void reg_glob(void *data, struct wl_registry *reg, uint32_t name, const char *intf, uint32_t v) {
//     if (!strcmp(intf, wl_compositor_interface.name)) {
//         comp = (wl_compositor *)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
//     } else if (!strcmp(intf, wl_shm_interface.name)) {
//         shm = (wl_shm *)wl_registry_bind(reg, name, &wl_shm_interface, 1);
//     } else if (!strcmp(intf, xdg_wm_base_interface.name)) {
//         sh = (xdg_wm_base *)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1);
//         xdg_wm_base_add_listener(sh, &sh_list, 0);
//     } else if (!strcmp(intf, wl_seat_interface.name)) {
//         seat = (wl_seat *)wl_registry_bind(reg, name, &wl_seat_interface, 1);
//         wl_seat_add_listener(seat, &seat_list, 0);
//     }
// }
//
// void reg_glob_rem(void *data, struct wl_registry *reg, uint32_t name) {
// }
//
// struct wl_registry_listener reg_list = { .global = reg_glob, .global_remove = reg_glob_rem };
//
// int main() {
//     cb_list = { .done = frame_new };
//     struct wl_display *disp = wl_display_connect(0);
//     struct wl_registry *reg = wl_display_get_registry(disp);
//     wl_registry_add_listener(reg, &reg_list, 0);
//     wl_display_roundtrip(disp);
//
//     srfc = wl_compositor_create_surface(comp);
//     struct wl_callback *cb = wl_surface_frame(srfc);
//     wl_callback_add_listener(cb, &cb_list, 0);
//
//     struct xdg_surface *xrfc = xdg_wm_base_get_xdg_surface(sh, srfc);
//     xdg_surface_add_listener(xrfc, &xrfc_list, 0);
//     top = xdg_surface_get_toplevel(xrfc);
//     xdg_toplevel_add_listener(top, &top_list, 0);
//     xdg_toplevel_set_title(top, "wayland client");
//     wl_surface_commit(srfc);
//
//     while (wl_display_dispatch(disp)) {
//         if (cls) break;
//     }
//
//     if (kb) {
//         wl_keyboard_destroy(kb);
//     }
//
//     wl_seat_release(seat);
//
//     if (bfr) {
//         wl_buffer_destroy(bfr);
//     }
//
//     xdg_toplevel_destroy(top);
//     xdg_surface_destroy(xrfc);
//     wl_surface_destroy(srfc);
//     wl_display_disconnect(disp);
//
//     return 0;
// }
