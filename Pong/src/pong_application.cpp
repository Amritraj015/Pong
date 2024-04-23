#include "pong_application.h"

Engine::Application *create_application() {
    static PongApplication application;

    return &application;
}
