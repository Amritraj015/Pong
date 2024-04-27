#pragma once
#include "platform/platform.h"
#include "application/application.h"

namespace Engine {
    class ApplicationManager {

        private:
            std::shared_ptr<Platform> mpPlatform;
            Application *mpApp;

            StatusCode InitializeSubSystems();
            StatusCode TerminateSubSystems();

        public:
            explicit ApplicationManager(const std::shared_ptr<Platform> &platform, Application *application);
            StatusCode RunApplication();
    };
}; // namespace Engine
