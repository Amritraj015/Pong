#pragma once

#include "events/event.h"
#include "events/enums/event_category.h"

namespace Engine {
    class WindowCloseEvent : public Event {
        public:
            [[nodiscard]] inline EventType GetEventType() const override {
                return EventType::WindowClose;
            }

            [[nodiscard]] inline i32 GetCategoryFlags() const override {
                return msCategoryFlag;
            }

        private:
            const static i32 msCategoryFlag = to_underlying(EventCategory::ApplicationEvent);
    };
} // namespace Engine
