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

        LFATAL("%i This is %s level log.", 1, "FETAL")
        LERROR("%i This is %s level log.", 2, "ERROR")
        LWARN("%i This is %s level log.", 3, "WARN")
        LDEBUG("%i This is %s level log.", 4, "DEBUG")
        LINFO("%i This is %s level log.", 5, "INFO")
        LTRACE("%i This is %s level log.", 6, "TRACE")

        LINFO("***************************************************************")
        LINFO("Application details")
        LINFO("***************************************************************")
        LINFO("Application name              | %s", mpApp->name)
        LINFO("Window Height requested       | %i", mpApp->windowHeight)
        LINFO("Window Width requested        | %i", mpApp->windowWidth)
        LINFO("Window Starting X position    | %i", mpApp->windowStartX)
        LINFO("Window Starting Y position    | %i", mpApp->windowStartY)
        LINFO("***************************************************************")

        return TerminateSubSystems();
    }

    StatusCode ApplicationManager::InitializeSubSystems() {
        // Initialize the logger sub-system.
        StatusCode statusCode = Engine::initialize_logger();

        ENSURE_SUCCESS(statusCode, "StatusCode: %i - Faild to initialize logger.", statusCode)

        statusCode = mpPlatform->CreateNewWindow(mpApp->name, mpApp->windowStartX, mpApp->windowStartY,
                                                 mpApp->windowWidth, mpApp->windowHeight);

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
