#include "pong_application.h"

Engine::Application *create_application() {
    PongApplication *application = new PongApplication();

    application->name = "Pong Application";
    application->windowWidth = 500;
    application->windowHeight = 400;
    application->windowStartX = 100;
    application->windowStartY = 100;

    return application;
}
