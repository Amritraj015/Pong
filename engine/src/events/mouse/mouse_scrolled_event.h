#pragma once

#include "events/event.h"
#include "events/enums/event_category.h"

namespace Engine {
    class MouseScrolledEvent : public Event {
        public:
            MouseScrolledEvent(const bool direction, const i32 xOffset, const i32 yOffset)
                : mDirection(direction), mXOffset(xOffset), mYOffset(yOffset) {
            }

            [[nodiscard]] inline i32 GetXOffset() const {
                return mXOffset;
            }

            [[nodiscard]] inline i32 GetYOffset() const {
                return mYOffset;
            }

            [[nodiscard]] inline bool GetDirection() const {
                return mDirection;
            }

            [[nodiscard]] inline i32 GetCategoryFlags() const override {
                return msCategoryFlags;
            }

            [[nodiscard]] inline EventType GetEventType() const override {
                return EventType::MouseScrolled;
            }

        private:
            // `true` -> mouse scrolled up else mouse scrolled down.
            const bool mDirection;
            const i32 mXOffset, mYOffset;
            const static i32 msCategoryFlags = to_underlying(EventCategory::Mouse) | to_underlying(EventCategory::Input);

    };
} // namespace Engine
