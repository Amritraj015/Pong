#pragma once

#include "../core/StatusCode.h"
#include "../Defines.h"

namespace PongEngine
{
    class Platform
    {
    public:
        Platform(const Platform &) = delete;
        void operator=(Platform const &) = delete;

        virtual ~Platform() = default;

        /** Creates a new window for the application.
         * @param windowName Name for the Window.
         * @param x X-coordinate for the window.
         * @param y Y-coordinate for the window.
         * @param width Width of the window to be created.
         * @param height Height of the window to be crated.
         */
        virtual StatusCode CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height) = 0;

        /** Closes the application window. */
        virtual StatusCode CloseWindow() = 0;

        /** Polls for events on the platform specific window. */
        virtual bool PollForEvents() = 0;

        /** SleepForDuration on the thread for the provided ms. This blocks the main thread.
         * Should only be used for giving time back to the OS for unused update power.
         * Therefore it is not exported.
         * @param duration SleepForDuration duration.
         */
        virtual void SleepForDuration(u64 duration) = 0;

    protected:
        Platform() = default;
    };
}