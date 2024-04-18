#include "pong_application.h"
#include <iostream>

Engine::Application *create_application() {
    static PongApplication application;
    std::cout << "Pong Application started successfully!" << std::endl;

    return &application;
}
