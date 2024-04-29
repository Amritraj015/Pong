#pragma once

#include "platform/platform.h"
#include "application/application.h"
#include "logger/logger.h"

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
                LTRACE("Destroying application: %s", mpApplication->name)

                delete mpApplication;

                LTRACE("Destroyed application manager")
            }
    };
}; // namespace Engine
