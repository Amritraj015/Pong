#include "application_manager.h"
#include "logger/logger.h"

namespace Engine {
    ApplicationManager::ApplicationManager(const std::shared_ptr<Platform> &platform, Application *application) {
        mpPlatform = platform;
        mpApp = application;
    }

    StatusCode ApplicationManager::RunApplication() {
        StatusCode statusCode = InitializeSubSystems();

        // Return if any sub-system failed to initialize.
        RETURN_ON_FAIL(statusCode)

        return TerminateSubSystems();
    }

    StatusCode ApplicationManager::InitializeSubSystems() {
        // Initialize the logger sub-system.
        StatusCode statusCode = Engine::initialize_logger();

        ENSURE_SUCCESS(statusCode, "StatusCode: %i - Faild to initialize logger.", statusCode)

        LINFO("***************************************************************")
        LINFO("Application details")
        LINFO("***************************************************************")
        LINFO("Application name              | %s", mpApp->name)
        LINFO("Window Height requested       | %i", mpApp->windowHeight)
        LINFO("Window Width requested        | %i", mpApp->windowWidth)
        LINFO("Window Starting X position    | %i", mpApp->windowStartX)
        LINFO("Window Starting Y position    | %i", mpApp->windowStartY)
        LINFO("***************************************************************")

        statusCode =
            mpPlatform->CreateNewWindow(mpApp->name, mpApp->windowStartX, mpApp->windowStartY, mpApp->windowWidth, mpApp->windowHeight);

        ENSURE_SUCCESS(statusCode, "StatusCode: %i - Failed to create window.", statusCode)

        return StatusCode::Successful;
    }

    StatusCode ApplicationManager::TerminateSubSystems() {
        // Close the application window.
        mpPlatform->CloseWindow();

        // Terminate the logger sub-system.
        Engine::terminate_logger();

        return StatusCode::Successful;
    }
}; // namespace Engine
