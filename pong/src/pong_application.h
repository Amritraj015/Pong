#pragma once

#include <engine.h>

class PongApplication : public Engine::Application {
    public:
        ~PongApplication() override {
            name = nullptr;
        }
};
