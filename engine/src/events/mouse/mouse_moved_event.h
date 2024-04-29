#pragma once

#include "events/event.h"
#include "events/enums/event_category.h"

namespace Engine {
    class MouseMovedEvent : public Event {
        public:
            MouseMovedEvent(const i32 x, const i32 y) : mMouseX(x), mMouseY(y) {
            }

            [[nodiscard]] inline i32 GetX() const {
                return mMouseX;
            }

            [[nodiscard]] inline i32 GetY() const {
                return mMouseY;
            }

            [[nodiscard]] inline i32 GetCategoryFlags() const override {
                return msCategoryFlags;
            }

            [[nodiscard]] inline EventType GetEventType() const override {
                return EventType::MouseMoved;
            }

        private:
            const i32 mMouseX, mMouseY;
            const static i32 msCategoryFlags = to_underlying(EventCategory::Mouse) | to_underlying(EventCategory::Input);
    };
} // namespace Engine
