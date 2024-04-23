#include "application_manager.h"
#include "logger/logger.h"

namespace Engine {
    ApplicationManager::ApplicationManager(const std::shared_ptr<Platform> &platform, Application *application) {
        mpPlatform = platform;
        mpApplication = application;
    }

    void ApplicationManager::RunApplication() {
        this->InitializeSubSystems();

        LFETAL("%i This is %s level log.", 1, "FETAL")
        LERROR("%i This is %s level log.", 2, "ERROR")
        LWARN("%i This is %s level log.", 3, "WARN")
        LDEBUG("%i This is %s level log.", 4, "DEBUG")
        LINFO("%i This is %s level log.", 5, "INFO")
        LTRACE("%i This is %s level log.", 6, "TRACE")

        this->TerminateSubSystems();
    }

    StatusCode ApplicationManager::InitializeSubSystems() {
        // Initialize logger sub-system.
        Engine::initialize_logger();

        return StatusCode::Successful;
    }

    StatusCode ApplicationManager::TerminateSubSystems() {
        // Terminate the logger sub-system.
        Engine::terminate_logger();

        return StatusCode::Successful;
    }
}; // namespace Engine
