#pragma once

#include "events/event.h"
#include "events/enums/event_category.h"
#include "events/enums/key.h"

namespace Engine {
    // A class to represent a key press or release event.
    class KeyEvent : public Event {
        public:
            KeyEvent(Key keycode, bool pressed) : mKeycode(keycode), mPressed(pressed) {
            }

            [[nodiscard]] inline i32 GetCategoryFlags() const override {
                return msCategoryFlags;
            }

            [[nodiscard]] inline EventType GetEventType() const override {
                return mPressed ? EventType::KeyPressed : EventType::KeyReleased;
            }

            [[nodiscard]] inline Key GetKeyCode() const {
                return mKeycode;
            }

            [[nodiscard]] inline bool IsKeyPressed() const {
                return mPressed;
            }

        private:
            const static i32 msCategoryFlags = to_underlying(EventCategory::Keyboard) | to_underlying(EventCategory::Input);
            const bool mPressed;
            const Key mKeycode;
    };
} // namespace Engine
