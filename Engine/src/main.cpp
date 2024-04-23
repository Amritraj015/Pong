#include "engine.h"
#include "defines.h"
#include "application/application_manager.h"
#include "platform/platform_factory.h"
#include "core/status_code.h"

int main() {
    // Get the current platform.
    std::shared_ptr<Engine::Platform> platform = Engine::get_platform();

    // Terminate if the application is running on an unsupported platform.
    if (nullptr == platform) return to_underlying(Engine::StatusCode::UnsupportedPlatform);

    // Create the application.
    Engine::Application *application = create_application();

    // Terminate if the application is invalid.
    if (nullptr == application) return to_underlying(Engine::StatusCode::InvalidApplication);

    // Create the application manager.
    auto applicationManager = std::make_unique<Engine::ApplicationManager>(platform, application);

    // Initialize the application.
    applicationManager->RunApplication();

    // Exit with a succesful status code.
    return to_underlying(Engine::StatusCode::Successful);
}
