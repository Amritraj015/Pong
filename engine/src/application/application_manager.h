#pragma once

#include "platform/platform.h"
#include "application/application.h"

namespace Engine {
    class ApplicationManager {

        private:
            Platform *mpPlatform;
            Application *mpApplication;

            StatusCode InitializeSubSystems();
            StatusCode TerminateSubSystems();

        public:
            explicit ApplicationManager(Platform *platform, Application *application);
            StatusCode RunApplication();

            ~ApplicationManager() {
                delete mpApplication;
            }
    };
}; // namespace Engine
