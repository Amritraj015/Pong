#pragma once
#include "platform/platform.h"
#include "application/application.h"

namespace Engine {
    class ApplicationManager {

        private:
            std::shared_ptr<Platform> mpPlatform;
            Application *mpApplication;

            StatusCode InitializeSubSystems();
            StatusCode TerminateSubSystems();

        public:
            explicit ApplicationManager(const std::shared_ptr<Platform> &platform, Application *application);
            void RunApplication();
    };
}; // namespace Engine
