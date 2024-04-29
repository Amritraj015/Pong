#include "defines.h"

#ifdef PLATFORM_LINUX

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "wayland_platform.h"
#include "xdg-shell-protocol.h"

namespace Engine {
    void on_mouse_enter(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
    void on_mouse_leave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface);
    void on_mouse_motion(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
    void on_mouse_input(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
    void on_mouse_axis(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
    void on_mouse_frame(void *data, wl_pointer *pointer);
    void on_mouse_axis_source(void *data, wl_pointer *pointer, uint32_t axis_source);
    void on_mouse_axis_stop(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis);
    void on_mouse_axis_discrete(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete);
    void on_mouse_axis_value120(void *data, wl_pointer *pointer, uint32_t axis, int32_t value120);
    void on_mouse_axis_relative_direction(void *data, wl_pointer *pointer, uint32_t axis, uint32_t direction);

    void keyboard_repeat_info(void *data, wl_keyboard *kb, int32_t rate, int32_t del);
    void keyboard_modifiers(void *data, wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t grp);
    void on_key_press(void *data, wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t state);
    void on_keyboard_leave(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface);
    void on_keyboard_enter(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface, wl_array *keys);
    void keyboard_mapping(void *data, wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz);

    void configure_top_level_object(void *data, xdg_toplevel *top, int32_t newWidth, int32_t newHeight, wl_array *stat);
    void close_top_level_object(void *data, xdg_toplevel *top);
    void configure_surface(void *data, xdg_surface *xdg_surface, uint32_t serial);
    void on_global_object_removal(void *data, wl_registry *registry, uint32_t name);
    void on_global_object_available(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t v);
    void seat_name(void *data, wl_seat *seat, const char *name);
    void shell_ping(void *data, xdg_wm_base *wmBase, uint32_t serial);
    void draw_new_frame(void *data, wl_callback *cb, uint32_t a);
    void resize_window(WaylandPlatform *platform);
    void draw(WaylandPlatform *platform);
    i32 allocate_shared_memory(WaylandPlatform *platform, uint64_t sz);
    void seat_capabilities(void *data, wl_seat *seat, uint32_t capabilities);
    void translate_keycode();

    WaylandPlatform::WaylandPlatform() {
        mCloseWindow = false;
        mInitialized = false;

        mCallbackListener = { .done = draw_new_frame };
        mSurfaceListener = { .configure = configure_surface };
        mpXdgShellListener = { .ping = shell_ping };

        mRegistryListener = {
            .global = on_global_object_available,
            .global_remove = on_global_object_removal,
        };

        mSeatListener = {
            .capabilities = seat_capabilities,
            .name = seat_name,
        };

        mTopLevelListener = {
            .configure = configure_top_level_object,
            .close = close_top_level_object,
        };

        mKeyboardListener = {
            .keymap = keyboard_mapping,
            .enter = on_keyboard_enter,
            .leave = on_keyboard_leave,
            .key = on_key_press,
            .modifiers = keyboard_modifiers,
            .repeat_info = keyboard_repeat_info,
        };

        mPointerListener = {
            .enter = on_mouse_enter,
            .leave = on_mouse_leave,
            .motion = on_mouse_motion,
            .button = on_mouse_input,
            .axis = on_mouse_axis,
            .frame = on_mouse_frame,
            .axis_source = on_mouse_axis_source,
            .axis_stop = on_mouse_axis_stop,
            .axis_discrete = on_mouse_axis_discrete,
            .axis_value120 = on_mouse_axis_value120,
            .axis_relative_direction = on_mouse_axis_relative_direction,
        };
    }

    StatusCode WaylandPlatform::CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height) {
        // Make sure the platform is not initialized already.
        if (mInitialized) return StatusCode::PlatformAlreadyInitialized;

        // Mark the platform as initialized and store the height,
        // width and name of the window to be created internally.
        mInitialized = true;
        mWindowWidth = width;
        mWindowHeight = height;
        mpWindowName = windowName;

        // Let's try and connect to the display.
        mpDisplay = wl_display_connect(nullptr);

        // If we couldn't connect to the display then return with an error status.
        if (nullptr == mpDisplay) return StatusCode::WaylandCannotConnectToDisplay;

        LTRACE("Connected to display")

        // Now, lets get the Wayland registry.
        wl_registry *registry = wl_display_get_registry(mpDisplay);

        // Add a listeners to the registry.
        wl_registry_add_listener(registry, &mRegistryListener, this);
        wl_display_dispatch(mpDisplay);
        wl_display_roundtrip(mpDisplay);

        // If we couldn't find a Wayland compositor then return with an error status.
        if (nullptr == mpCompositor) return StatusCode::WaylandCannotFindCompositor;

        LTRACE("Wayland compositor found")

        // Now, lets create a surface.
        mpSurface = wl_compositor_create_surface(mpCompositor);

        // If surface creation failed then return with an error.
        if (nullptr == mpSurface) return StatusCode::WaylandCannotCreateSurface;

        LTRACE("Created wayland surface")

        wl_callback *callback = wl_surface_frame(mpSurface);
        wl_callback_add_listener(callback, &mCallbackListener, this);

        // Let's create XDG surface.
        mpXdgSurface = xdg_wm_base_get_xdg_surface(mpWmBase, mpSurface);

        // If XDG surface creation failed then return with an error.
        if (nullptr == mpXdgSurface) return StatusCode::WaylandCannotCreateXdgSurface;

        LTRACE("Created XDG surface")

        // Attach listeners to the XDG surface.
        xdg_surface_add_listener(mpXdgSurface, &mSurfaceListener, this);

        // Assign the top level window object.
        mpXdgTopLevelObject = xdg_surface_get_toplevel(mpXdgSurface);

        // If XDG surface creation failed then return with an error.
        if (nullptr == mpXdgTopLevelObject) return StatusCode::WaylandCannotCreateTopLevelWindow;

        LTRACE("Created top level window")

        // Add listeners to the top level window.
        xdg_toplevel_add_listener(mpXdgTopLevelObject, &mTopLevelListener, this);

        // Set title for the top level window.
        xdg_toplevel_set_title(mpXdgTopLevelObject, windowName);

        wl_surface_commit(mpSurface);

        while (wl_display_dispatch(mpDisplay)) {
            if (mCloseWindow) break;
        }

        return StatusCode::Successful;
    }

    StatusCode WaylandPlatform::CloseWindow() {
        // Dispose pointer structure.
        wl_pointer_release(mpPointer);
        LTRACE("Released pointer resources")

        // Dispose keyboard structure.
        wl_keyboard_release(mpKeyboard);
        LTRACE("Released keyboard resoueces")

        // Dispose seat.
        wl_seat_release(mpSeat);
        LTRACE("Released seat resources")

        // Destroy buffer.
        if (mpBuffer) wl_buffer_destroy(mpBuffer);

        LTRACE("Destroyed wayland buffer")

        // Destroy XDG top level object.
        xdg_toplevel_destroy(mpXdgTopLevelObject);
        LTRACE("Destroyed XDG top level object")

        // Destroy XDG surface.
        xdg_surface_destroy(mpXdgSurface);
        LTRACE("Destroyed XDG surface")

        // Destroy surface.
        wl_surface_destroy(mpSurface);
        LTRACE("Destroyed wayland surface")

        // Disconnect from display.
        wl_display_disconnect(mpDisplay);
        LTRACE("Disconnected from wayland display")

        // Set the initialized flag as false.
        mInitialized = false;

        // Finally, return a successful response.
        return StatusCode::Successful;
    }

    bool WaylandPlatform::PollForEvents() {
        return true;
    }

    void WaylandPlatform::SleepForDuration(u64 duration) {
    }

    void keyboard_mapping(void *data, wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz) {
    }

    void on_keyboard_enter(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface, wl_array *keys) {
        LDEBUG("Keyboard enter event")
    }

    void on_keyboard_leave(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface) {
        LDEBUG("Keyboard leave event")
    }

    void on_key_press(void *data, wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t state) {
        if (1 == key) {
            ((WaylandPlatform *)data)->IssueTerminateCommand();
        }

        if (1 == state) {
            LDEBUG("Key down: %i", static_cast<char>(key))
        } else {
            LDEBUG("Key up: %i", static_cast<char>(key))
        }
    }

    void keyboard_modifiers(void *data, wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t grp) {
    }

    void keyboard_repeat_info(void *data, wl_keyboard *kb, int32_t rate, int32_t del) {
    }

    void seat_capabilities(void *data, wl_seat *seat, uint32_t capabilities) {
        auto platform = (WaylandPlatform *)data;

        if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD && !platform->GetKeyboard()) {
            platform->SetKeyboard(wl_seat_get_keyboard(seat));
            wl_keyboard_add_listener(platform->GetKeyboard(), platform->GetKeyboardListener(), platform);
        }

        if (capabilities & WL_SEAT_CAPABILITY_POINTER && !platform->GetPointer()) {
            platform->SetPointer(wl_seat_get_pointer(seat));
            wl_pointer_add_listener(platform->GetPointer(), platform->GetPointerListener(), platform);
        }
    }

    i32 allocate_shared_memory(WaylandPlatform *platform, uint64_t sz) {
        i32 fd = shm_open(platform->GetWindowName(), O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);

        shm_unlink(platform->GetWindowName());
        ftruncate(fd, sz);

        return fd;
    }

    void draw(WaylandPlatform *platform) {
        memset(platform->GetPixels(), 255, platform->GetWindowWidth() * platform->GetWindowHeight() * 4);

        wl_surface_attach(platform->GetSurface(), platform->GetBuffer(), 0, 0);
        wl_surface_damage_buffer(platform->GetSurface(), 0, 0, platform->GetWindowWidth(), platform->GetWindowHeight());
        wl_surface_commit(platform->GetSurface());
    }

    void resize_window(WaylandPlatform *platform) {
        i32 size = platform->GetWindowHeight() * platform->GetWindowWidth() * 4;
        i32 fd = allocate_shared_memory(platform, size);

        platform->SetPixels(mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

        wl_shm_pool *pool = wl_shm_create_pool(platform->GetSharedMemory(), fd, size);

        platform->SetBuffer(wl_shm_pool_create_buffer(pool, 0, platform->GetWindowWidth(), platform->GetWindowHeight(),
                                                      platform->GetWindowWidth() * 4, WL_SHM_FORMAT_ARGB8888));

        wl_shm_pool_destroy(pool);

        close(fd);
    }

    void draw_new_frame(void *data, wl_callback *cb, uint32_t a) {
        wl_callback_destroy(cb);

        auto platform = (WaylandPlatform *)data;

        cb = wl_surface_frame(platform->GetSurface());
        wl_callback_add_listener(cb, platform->GetCallbackListener(), platform);

        draw(platform);
    }

    void shell_ping(void *data, xdg_wm_base *wmBase, uint32_t serial) {
        xdg_wm_base_pong(wmBase, serial);
    }

    void seat_name(void *data, wl_seat *seat, const char *name) {
        LTRACE("Seat name - %s", name)
    }

    void on_global_object_available(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t v) {
        if (0 == strcmp(interface, wl_compositor_interface.name)) {
            ((WaylandPlatform *)data)->SetCompositor((wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, 4));
        } else if (0 == strcmp(interface, wl_shm_interface.name)) {
            ((WaylandPlatform *)data)->SetSharedMemory((wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, 1));
        } else if (0 == strcmp(interface, xdg_wm_base_interface.name)) {
            ((WaylandPlatform *)data)->SetWmBase((xdg_wm_base *)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
            xdg_wm_base_add_listener(((WaylandPlatform *)data)->GetWmBase(), ((WaylandPlatform *)data)->GetWmBaseListener(), data);
        } else if (0 == strcmp(interface, wl_seat_interface.name)) {
            ((WaylandPlatform *)data)->SetSeat((wl_seat *)wl_registry_bind(registry, name, &wl_seat_interface, 1));
            wl_seat_add_listener(((WaylandPlatform *)data)->GetSeat(), ((WaylandPlatform *)data)->GetSeatListener(), data);
        }
    }

    void on_global_object_removal(void *data, wl_registry *registry, uint32_t name) {
    }

    void configure_surface(void *data, xdg_surface *xdg_surface, uint32_t serial) {
        xdg_surface_ack_configure(xdg_surface, serial);

        auto platform = (WaylandPlatform *)data;

        if (!platform->GetPixels()) resize_window(platform);

        draw(platform);
    }

    void configure_top_level_object(void *data, xdg_toplevel *top, int32_t newWidth, int32_t newHeight, wl_array *stat) {
        if (!newWidth && !newHeight) {
            return;
        }

        auto platform = (WaylandPlatform *)data;

        if (platform->GetWindowWidth() != newWidth || platform->GetWindowHeight() != newHeight) {
            munmap(platform->GetPixels(), platform->GetWindowWidth() * platform->GetWindowHeight() * 4);

            platform->SetWindowWidth(newWidth);
            platform->SetWindowHeight(newHeight);

            resize_window(platform);
        }
    }

    void close_top_level_object(void *data, xdg_toplevel *top) {
        ((WaylandPlatform *)data)->IssueTerminateCommand();
    }

    void on_mouse_enter(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
        LDEBUG("Mouse entered window at (x, y) - (%i, %i)", surface_x, surface_y)
    }

    void on_mouse_leave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface) {
        LDEBUG("Mouse left window")
    }

    void on_mouse_motion(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
        LDEBUG("Mouse moved to - (%i, %i)", surface_x, surface_y)
    }

    void on_mouse_input(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
        if (1 == state)
            LDEBUG("Mouse button %i down", button)
        else
            LDEBUG("Mouse button %i up", button)
    }

    void on_mouse_axis(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
        LDEBUG("Mouse scrolled: Axis - %i, value: %i", axis, value)
    }

    void on_mouse_frame(void *data, wl_pointer *pointer) {
    }

    void on_mouse_axis_source(void *data, wl_pointer *pointer, uint32_t axis_source) {
    }

    void on_mouse_axis_stop(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis) {
    }

    void on_mouse_axis_discrete(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete) {
    }

    void on_mouse_axis_value120(void *data, wl_pointer *pointer, uint32_t axis, int32_t value120) {
    }

    void on_mouse_axis_relative_direction(void *data, wl_pointer *pointer, uint32_t axis, uint32_t direction) {
    }
} // namespace Engine

#endif
