#pragma once

#include "defines.h"

#if defined(PLATFORM_LINUX)

#include "platform/platform.h"
#include "xdg-shell-protocol.h"
#include "logger/logger.h"

namespace Engine {
    class WaylandPlatform : public Platform {
        public:
            WaylandPlatform(const WaylandPlatform &) = delete;
            void operator=(WaylandPlatform const &) = delete;

            WaylandPlatform();

            ~WaylandPlatform() override {
                mpWindowName = nullptr;
                mpDisplay = nullptr;
                mpSurface = nullptr;
                mpCompositor = nullptr;
                mpSharedMemory = nullptr;
                mpSeat = nullptr;
                mpWmBase = nullptr;
                mpXdgTopLevelObject = nullptr;
                mpXdgSurface = nullptr;
                mpKeyboard = nullptr;
                mpPointer = nullptr;
                mpBuffer = nullptr;
                mpPixel = nullptr;

                LTRACE("Destroyed wayland platform")
            }

            StatusCode CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height) override;
            StatusCode CloseWindow() override;
            bool PollForEvents() override;
            void SleepForDuration(u64 duration) override;

            inline void IssueTerminateCommand() {
                mCloseWindow = true;
            }

            inline void SetCompositor(wl_compositor *compositor) {
                mpCompositor = compositor;
            }

            inline void SetSharedMemory(wl_shm *shm) {
                mpSharedMemory = shm;
            }

            inline void SetSeat(wl_seat *seat) {
                mpSeat = seat;
            }

            inline void SetWmBase(xdg_wm_base *wmBase) {
                mpWmBase = wmBase;
            }

            [[nodiscard]] inline wl_seat *GetSeat() const {
                return mpSeat;
            }

            inline const wl_seat_listener *GetSeatListener() {
                return &mSeatListener;
            }

            [[nodiscard]] inline xdg_wm_base *GetWmBase() const {
                return mpWmBase;
            }

            inline const xdg_wm_base_listener *GetWmBaseListener() {
                return &mpXdgShellListener;
            }

            [[nodiscard]] inline void *GetPixels() const {
                return mpPixel;
            }

            inline void SetPixels(void *pixels) {
                mpPixel = pixels;
            }

            [[nodiscard]] inline wl_keyboard *GetKeyboard() const {
                return mpKeyboard;
            }

            inline void SetKeyboard(wl_keyboard *kb) {
                mpKeyboard = kb;
            }

            [[nodiscard]] inline wl_pointer *GetPointer() const {
                return mpPointer;
            }

            inline void SetPointer(wl_pointer *pointer) {
                mpPointer = pointer;
            }

            [[nodiscard]] inline const wl_keyboard_listener *GetKeyboardListener() const {
                return &mKeyboardListener;
            }

            [[nodiscard]] inline wl_shm *GetSharedMemory() const {
                return mpSharedMemory;
            }

            [[nodiscard]] inline wl_surface *GetSurface() const {
                return mpSurface;
            }

            [[nodiscard]] inline wl_buffer *GetBuffer() const {
                return mpBuffer;
            }

            inline void SetBuffer(wl_buffer *buffer) {
                mpBuffer = buffer;
            }

            [[nodiscard]] inline const wl_callback_listener *GetCallbackListener() const {
                return &mCallbackListener;
            }

            [[nodiscard]] inline i32 GetWindowWidth() const {
                return mWindowWidth;
            }

            inline void SetWindowWidth(i32 width) {
                mWindowWidth = width;
            }

            [[nodiscard]] inline i32 GetWindowHeight() const {
                return mWindowHeight;
            }

            inline void SetWindowHeight(i32 height) {
                mWindowHeight = height;
            }

            [[nodiscard]] inline const char *GetWindowName() const {
                return mpWindowName;
            }

            [[nodiscard]] inline const wl_pointer_listener *GetPointerListener() const {
                return &mPointerListener;
            }

        private:
            /** A boolean flag that represents if the platform has been initialized. */
            bool mInitialized;

            /** A boolean flag that is used to close the application window. */
            bool mCloseWindow;

            /** Represents the current width of the window. */
            i32 mWindowWidth{};

            /** Represents the current height of the window. */
            i32 mWindowHeight{};

            /** The name to be displayed on the application window. */
            const char *mpWindowName{};

            /** Wayland display. */
            wl_display *mpDisplay{};

            /** Wayland surface. */
            wl_surface *mpSurface{};

            /** Wayland compositor. */
            wl_compositor *mpCompositor{};

            /** Wayland shared memory. */
            wl_shm *mpSharedMemory{};

            /** Wayland seat. */
            wl_seat *mpSeat{};

            /** XDG Shell. */
            xdg_wm_base *mpWmBase{};

            /** XDG top level window. */
            xdg_toplevel *mpXdgTopLevelObject{};

            /** XDG shell surface. */
            xdg_surface *mpXdgSurface{};

            /** Wayland keyboard. */
            wl_keyboard *mpKeyboard{};

            /** Wayland pointer. */
            wl_pointer *mpPointer{};

            /** Wayland buffer. */
            wl_buffer *mpBuffer{};
            void *mpPixel{};

            /** Wayland registry listener. */
            wl_registry_listener mRegistryListener{};

            /** Wayland callback listener. */
            wl_callback_listener mCallbackListener{};

            /** Wayland seat listener. */
            wl_seat_listener mSeatListener{};

            /** XDG surface listener. */
            xdg_surface_listener mSurfaceListener{};

            /** XDG base listener. */
            xdg_wm_base_listener mpXdgShellListener{};

            /** XDG top level window listener. */
            xdg_toplevel_listener mTopLevelListener{};

            /** Wayland keyboard listener. */
            wl_keyboard_listener mKeyboardListener{};

            /** Wayland pointer listener. */
            wl_pointer_listener mPointerListener{};
    };

    // void on_mouse_enter(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y);
    // void on_mouse_leave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface);
    // void on_mouse_motion(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y);
    // void on_mouse_input(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
    // void on_mouse_axis(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
    // void on_mouse_frame(void *data, wl_pointer *pointer);
    // void on_mouse_axis_source(void *data, wl_pointer *pointer, uint32_t axis_source);
    // void on_mouse_axis_stop(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis);
    // void on_mouse_axis_discrete(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete);
    // void on_mouse_axis_value120(void *data, wl_pointer *pointer, uint32_t axis, int32_t value120);
    // void on_mouse_axis_relative_direction(void *data, wl_pointer *pointer, uint32_t axis, uint32_t direction);
    //
    // void keyboard_repeat_info(void *data, wl_keyboard *kb, int32_t rate, int32_t del);
    // void keyboard_modifiers(void *data, wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t grp);
    // void on_key_press(void *data, wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t state);
    // void on_keyboard_leave(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface);
    // void on_keyboard_enter(void *data, wl_keyboard *kb, uint32_t ser, wl_surface *surface, wl_array *keys);
    // void keyboard_mapping(void *data, wl_keyboard *kb, uint32_t format, int32_t fd, uint32_t sz);
    //
    // void configure_top_level_object(void *data, xdg_toplevel *top, int32_t newWidth, int32_t newHeight, wl_array *stat);
    // void close_top_level_object(void *data, xdg_toplevel *top);
    // void configure_surface(void *data, xdg_surface *xdg_surface, uint32_t serial);
    // void on_global_object_removal(void *data, wl_registry *registry, uint32_t name);
    // void on_global_object_available(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t v);
    // void seat_name(void *data, wl_seat *seat, const char *name);
    // void shell_ping(void *data, xdg_wm_base *wmBase, uint32_t serial);
    // void draw_new_frame(void *data, wl_callback *cb, uint32_t a);
    // void resize_window(WaylandPlatform *platform);
    // void draw(WaylandPlatform *platform);
    // i32 allocate_shared_memory(WaylandPlatform *platform, uint64_t sz);
    // void seat_capabilities(void *data, wl_seat *seat, uint32_t capabilities);
    // void translate_keycode();
} // namespace Engine

#endif
