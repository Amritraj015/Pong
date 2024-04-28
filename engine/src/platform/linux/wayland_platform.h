#pragma once

#include "defines.h"

#if defined(PLATFORM_LINUX)

#include "platform/platform.h"
#include "xdg-shell-protocol.h"

namespace Engine {
    class WaylandPlatform : public Platform {
        public:
            WaylandPlatform(const WaylandPlatform &) = delete;
            void operator=(WaylandPlatform const &) = delete;

            WaylandPlatform();

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

            inline void *GetPixels() const {
                return mpPixel;
            }

            inline const void SetPixels(void *pixels) {
                mpPixel = pixels;
            }

            inline wl_keyboard *GetKeyboard() const {
                return mpKeyboard;
            }

            inline const void SetKeyboard(wl_keyboard *kb) {
                mpKeyboard = kb;
            }

            inline wl_pointer *GetPointer() const {
                return mpPointer;
            }

            inline const void SetPointer(wl_pointer *pointer) {
                mpPointer = pointer;
            }

            inline const wl_keyboard_listener *GetKeyboardListener() const {
                return &mKeyboardListener;
            }

            inline wl_shm *GetSharedMemory() const {
                return mpSharedMemory;
            }

            inline wl_surface *GetSurface() const {
                return mpSurface;
            }

            inline wl_buffer *GetBuffer() const {
                return mpBuffer;
            }

            inline void SetBuffer(wl_buffer *buffer) {
                mpBuffer = buffer;
            }

            inline const wl_callback_listener *GetCallbackListener() const {
                return &mCallbackListener;
            }

            inline const i32 GetWindowWidth() const {
                return mWindowWidth;
            }

            const inline void SetWindowWidth(i32 width) {
                mWindowWidth = width;
            }

            inline const i32 GetWindowHeight() const {
                return mWindowHeight;
            }

            const inline void SetWindowHeight(i32 height) {
                mWindowHeight = height;
            }

            inline const char *GetWindowName() const {
                return mpWindowName;
            }

            inline const wl_pointer_listener *GetPointerListener() const {
                return &mPointerListener;
            }

        private:
            bool mInitialized;
            bool mCloseWindow;
            i32 mWindowWidth;
            i32 mWindowHeight;
            const char *mpWindowName;

            wl_display *mpDisplay;
            wl_surface *mpSurface;
            wl_compositor *mpCompositor = NULL;
            wl_shm *mpSharedMemory;
            wl_seat *mpSeat;
            xdg_wm_base *mpWmBase;
            xdg_toplevel *mpXdgTopLevelObject;
            xdg_surface *mpXdgSurface;
            wl_keyboard *mpKeyboard;
            wl_pointer *mpPointer;
            wl_buffer *mpBuffer;
            void *mpPixel;

            wl_registry_listener mRegistryListener;
            wl_callback_listener mCallbackListener;
            wl_seat_listener mSeatListener;
            xdg_surface_listener mSurfaceListener;
            xdg_wm_base_listener mpXdgShellListener;
            xdg_toplevel_listener mTopLevelListener;
            wl_keyboard_listener mKeyboardListener;
            wl_pointer_listener mPointerListener;
    };
}; // namespace Engine

#endif
