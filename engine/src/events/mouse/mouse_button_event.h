#pragma once

#include "events/event.h"
#include "events/enums/mouse_button.h"
#include "events/enums/event_category.h"

namespace Engine {
    class MouseButtonEvent : public Event {
        public:
            MouseButtonEvent(const MouseButton button, const bool pressed, const i32 mouseX, const i32 mouseY)
                : mButton(button), mPressed(pressed), mMouseX(mouseX), mMouseY(mouseY) {
            }

            [[nodiscard]] inline i32 GetCategoryFlags() const override {
                return msCategoryFlags;
            }

            [[nodiscard]] inline EventType GetEventType() const override {
                return mPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
            }

            [[nodiscard]] inline MouseButton GetMouseButton() const {
                return mButton;
            }

            [[nodiscard]] inline i32 GetMouseX() const {
                return mMouseX;
            }

            [[nodiscard]] inline i32 GetMouseY() const {
                return mMouseY;
            }

            [[nodiscard]] inline bool IsButtonPressed() const {
                return mPressed;
            }

        private:
            const static i32 msCategoryFlags =
                to_underlying(EventCategory::Mouse) | to_underlying(EventCategory::Input) | to_underlying(EventCategory::MouseButton);

            const MouseButton mButton;
            const bool mPressed;
            const i32 mMouseX, mMouseY;
    };
} // namespace Engine
