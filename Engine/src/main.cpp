#include "engine.h"
#include <wayland-client.h>
#include "logger/logger.h"

int main() {
    // struct wl_display *display = wl_display_connect(NULL);
    //
    // if (!display) {
    //     std::cout << "Failed to connect to Wayland display.\n" <<
    // std::endl;
    //     return 1;
    // }

    // std::cout << "Connection established successfully." << std::endl;
    // wl_display_disconnect(display);

    Engine::initialize_logger();

    LFATAL("%i This is %s level log.", 1, "FETAL")
    LERROR("%i This is %s level log.", 2, "ERROR")
    LWARN("%i This is %s level log.", 3, "WARN")
    LDEBUG("%i This is %s level log.", 4, "DEBUG")
    LINFO("%i This is %s level log.", 5, "INFO")
    LTRACE("%i This is %s level log.", 6, "TRACE")

    Engine::terminate_logger();

    LINFO("Creating Application...")
    auto application = create_application();
    LINFO("Application created successfully!")

    return 0;
}
