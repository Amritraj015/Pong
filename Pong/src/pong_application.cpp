#include <iostream>
#include "pong_application.h"

Engine::Application *create_application()
{
    static PongApplication application;
    std::cout << "Pong Application started successfully!" << std::endl;

    return &application;
}