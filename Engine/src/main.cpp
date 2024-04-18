#include "defines.h"
#include "engine.h"
#include <wayland-client.h>
#include <iostream>

int main() {
    struct wl_display *display = wl_display_connect(NULL);

    if (!display) {
        std::cout << "Failed to connect to Wayland display.\n" << std::endl;
        return 1;
    }

    std::cout << "Connection established successfully." << std::endl;
    wl_display_disconnect(display);
    auto application = create_application();

    return 0;
}
