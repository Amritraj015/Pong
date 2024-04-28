#pragma once

#include <engine.h>
#include <iostream>

class PongApplication : public Engine::Application {
    public:
        ~PongApplication() {
            std::cout << "Application destroyed!" << std::endl;
        }
};
