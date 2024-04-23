#include "defines.h"

#ifdef PLATFORM_LINUX

#include "linux_platform.h"
#include "xdg-shell-protocol.h"
#include "logger/logger.h"

namespace Engine {
    StatusCode LinuxPlatform::CreateNewWindow(const char *windowName, i16 x, i16 y, u16 width, u16 height) {
        // Make sure the platform is not initialized already.
        if (initialized) return StatusCode::PlatformAlreadyInitialized;

        // Mark the platform as initialized.
        initialized = true;

        // Let's try and connect to the display.
        display = wl_display_connect(NULL);

        // If we couldn't connect to the display then return with an error status.
        if (display == NULL) {
            LFETAL("Can't connect to display")
            exit(1);
        }

        LTRACE("Connected to display");

        struct wl_registry *registry = wl_display_get_registry(display);

        // TODO: fix this.
        // wl_registry_add_listener(registry, &registry_listener, NULL);

        wl_display_dispatch(display);
        wl_display_roundtrip(display);

        if (compositor == NULL) {
            LFETAL("Can't find compositor");
            exit(1);
        }

        LTRACE("Found compositor");

        surface = wl_compositor_create_surface(compositor);

        if (surface == NULL) {
            LFETAL("Can't create surface");
            exit(1);
        }

        LTRACE("Created surface");

        if (shell == NULL) {
            LFETAL("Haven't got a Wayland shell");
            exit(1);
        }

        wl_surface_destroy(surface);
        wl_display_disconnect(display);

        return StatusCode::Successful;
    }

    StatusCode LinuxPlatform::CloseWindow() {
        return StatusCode::Successful;
    }

    bool LinuxPlatform::PollForEvents() {
        return true;
    }

    void LinuxPlatform::SleepForDuration(u64 duration) {
    }
} // namespace Engine

#endif

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <wayland-client.h>
// #include <wayland-client-protocol.h>
// #include <wayland-egl.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <errno.h>
// #include <unistd.h>
// #include "platform/xdg-shell-protocol.h"
//
// struct wl_display *display = NULL;
// struct wl_compositor *compositor = NULL;
// struct wl_surface *surface;
// struct wl_shell *shell;
// struct wl_shell_surface *shell_surface;
// struct wl_shm *shm;
// struct wl_buffer *buffer;
// struct wl_callback *frame_callback;
//
// void *shm_data;
//
// int WIDTH = 480;
// int HEIGHT = 256;
//
// static void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
//     wl_shell_surface_pong(shell_surface, serial);
//     fprintf(stderr, "Pinged and ponged\n");
// }
//
// static void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width,
//                              int32_t height) {
// }
//
// static void handle_popup_done(void *data, struct wl_shell_surface *shell_surface) {
// }
//
// static const struct wl_shell_surface_listener shell_surface_listener = { handle_ping, handle_configure,
//                                                                          handle_popup_done };
//
// static int set_cloexec_or_close(int fd) {
//     long flags;
//
//     if (fd == -1) return -1;
//
//     flags = fcntl(fd, F_GETFD);
//     if (flags == -1) goto err;
//
//     if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1) goto err;
//
//     return fd;
//
// err:
//     close(fd);
//     return -1;
// }
//
// static int create_tmpfile_cloexec(char *tmpname) {
//     int fd;
//
// #ifdef HAVE_MKOSTEMP
//     fd = mkostemp(tmpname, O_CLOEXEC);
//     if (fd >= 0) unlink(tmpname);
// #else
//     fd = mkstemp(tmpname);
//     if (fd >= 0) {
//         fd = set_cloexec_or_close(fd);
//         unlink(tmpname);
//     }
// #endif
//
//     return fd;
// }
//
// /*
//  * Create a new, unique, anonymous file of the given size, and
//  * return the file descriptor for it. The file descriptor is set
//  * CLOEXEC. The file is immediately suitable for mmap()'ing
//  * the given size at offset zero.
//  *
//  * The file should not have a permanent backing store like a disk,
//  * but may have if XDG_RUNTIME_DIR is not properly implemented in OS.
//  *
//  * The file name is deleted from the file system.
//  *
//  * The file is suitable for buffer sharing between processes by
//  * transmitting the file descriptor over Unix sockets using the
//  * SCM_RIGHTS methods.
//  */
// int os_create_anonymous_file(off_t size) {
//     static const char templ[] = "/weston-shared-XXXXXX";
//     const char *path;
//     char *name;
//     int fd;
//
//     path = getenv("XDG_RUNTIME_DIR");
//     if (!path) {
//         errno = ENOENT;
//         return -1;
//     }
//
//     name = (char *)malloc(strlen(path) + sizeof(templ));
//     if (!name) return -1;
//     strcpy(name, path);
//     strcat(name, templ);
//
//     fd = create_tmpfile_cloexec(name);
//
//     free(name);
//
//     if (fd < 0) return -1;
//
//     if (ftruncate(fd, size) < 0) {
//         close(fd);
//         return -1;
//     }
//
//     return fd;
// }
//
// uint32_t pixel_value = 0x0; // black
//
// static void paint_pixels() {
//     int n;
//     uint32_t *pixel = (uint32_t *)shm_data;
//
//     for (n = 0; n < WIDTH * HEIGHT; n++) {
//         *pixel++ = pixel_value;
//     }
//
//     // increase each RGB component by one
//     pixel_value += 0x10101;
//
//     // if it's reached 0xffffff (white) reset to zero
//     if (pixel_value > 0xffffff) {
//         pixel_value = 0x0;
//     }
// }
//
// static struct wl_callback_listener frame_listener;
//
// int ht;
//
// static void redraw(void *data, struct wl_callback *callback, uint32_t time) {
//     // fprintf(stderr, "Redrawing\n");
//     wl_callback_destroy(frame_callback);
//     if (ht == 0) ht = HEIGHT;
//     wl_surface_damage(surface, 0, 0, WIDTH, ht--);
//     paint_pixels();
//     frame_callback = wl_surface_frame(surface);
//     wl_surface_attach(surface, buffer, 0, 0);
//     wl_callback_add_listener(frame_callback, &frame_listener, NULL);
//     wl_surface_commit(surface);
// }
//
// static struct wl_buffer *create_buffer() {
//     struct wl_shm_pool *pool;
//     int stride = WIDTH * 4; // 4 bytes per pixel
//     int size = stride * HEIGHT;
//     int fd;
//     struct wl_buffer *buff;
//
//     ht = HEIGHT;
//
//     fd = os_create_anonymous_file(size);
//     if (fd < 0) {
//         fprintf(stderr, "creating a buffer file for %d B failed: %m\n", size);
//         exit(1);
//     }
//
//     shm_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     if (shm_data == MAP_FAILED) {
//         fprintf(stderr, "mmap failed: %m\n");
//         close(fd);
//         exit(1);
//     }
//
//     pool = wl_shm_create_pool(shm, fd, size);
//     buff = wl_shm_pool_create_buffer(pool, 0, WIDTH, HEIGHT, stride, WL_SHM_FORMAT_XRGB8888);
//     // wl_buffer_add_listener(buffer, &buffer_listener, buffer);
//     wl_shm_pool_destroy(pool);
//     return buff;
// }
//
// static void create_window() {
//
//     buffer = create_buffer();
//
//     wl_surface_attach(surface, buffer, 0, 0);
//     // wl_surface_damage(surface, 0, 0, WIDTH, HEIGHT);
//     wl_surface_commit(surface);
// }
//
// static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
//     char *s;
//     switch (format) {
//     case WL_SHM_FORMAT_ARGB8888:
//         s = "ARGB8888";
//         break;
//     case WL_SHM_FORMAT_XRGB8888:
//         s = "XRGB8888";
//         break;
//     case WL_SHM_FORMAT_RGB565:
//         s = "RGB565";
//         break;
//     default:
//         s = "other format";
//         break;
//     }
//     fprintf(stderr, "Possible shmem format %s\n", s);
// }
//
// struct wl_shm_listener shm_listener = { shm_format };
//
// static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface,
//                                     uint32_t version) {
//     // if (strcmp(interface, "wl_compositor") == 0) {
//     if (strcmp(interface, wl_compositor_interface.name) == 0) {
//         compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
//     } else if (strcmp(interface, "wl_shell") == 0) {
//         shell = (wl_shell *)wl_registry_bind(registry, id, &wl_shell_interface, 1);
//     } else if (strcmp(interface, "wl_shm") == 0) {
//         shm = (wl_shm *)wl_registry_bind(registry, id, &wl_shm_interface, 1);
//         wl_shm_add_listener(shm, &shm_listener, NULL);
//     }
// }
//
// static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
//     printf("Got a registry losing event for %d\n", id);
// }
//
// static const struct wl_registry_listener registry_listener = { global_registry_handler, global_registry_remover };
//
// int main(int argc, char **argv) {
//     frame_listener = { .done = redraw };
//
//     display = wl_display_connect(NULL);
//     if (display == NULL) {
//         fprintf(stderr, "Can't connect to display\n");
//         exit(1);
//     }
//     printf("connected to display\n");
//
//     struct wl_registry *registry = wl_display_get_registry(display);
//     wl_registry_add_listener(registry, &registry_listener, NULL);
//
//     wl_display_dispatch(display);
//     wl_display_roundtrip(display);
//
//     if (compositor == NULL) {
//         fprintf(stderr, "Can't find compositor\n");
//         exit(1);
//     } else {
//         fprintf(stderr, "Found compositor\n");
//     }
//
//     surface = wl_compositor_create_surface(compositor);
//     if (surface == NULL) {
//         fprintf(stderr, "Can't create surface\n");
//         exit(1);
//     } else {
//         fprintf(stderr, "Created surface\n");
//     }
//
//     if (shell == NULL) {
//         fprintf(stderr, "Haven't got a Wayland shell\n");
//         exit(1);
//     }
//
//     shell_surface = wl_shell_get_shell_surface(shell, surface);
//     if (shell_surface == NULL) {
//         fprintf(stderr, "Can't create shell surface\n");
//         exit(1);
//     } else {
//         fprintf(stderr, "Created shell surface\n");
//     }
//     wl_shell_surface_set_toplevel(shell_surface);
//
//     wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);
//
//     frame_callback = wl_surface_frame(surface);
//     wl_callback_add_listener(frame_callback, &frame_listener, NULL);
//
//     create_window();
//     redraw(NULL, NULL, 0);
//
//     while (wl_display_dispatch(display) != -1) {
//         ;
//     }
//
//     wl_display_disconnect(display);
//     printf("disconnected from display\n");
//
//     exit(0);
// }

// =====================================================================================
// #include <cstring>
// #include <fcntl.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <wayland-client.h>
//
// #include "platform/xdg-shell-protocol.h"
//
// struct wl_compositor *comp;
// struct wl_surface *srfc;
// struct wl_buffer *bfr;
// struct wl_shm *shm;
// struct xdg_wm_base *sh;
// struct xdg_toplevel *top;
// struct wl_seat *seat;
// struct wl_keyboard *kb;
// void *pixl;
// int w = 500;
// int h = 500;
// uint8_t cls;
//
// int alc_shm(uint64_t sz) {
//     const char *name = "/testwin";
//
//     int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
//
//     shm_unlink(name);
//     ftruncate(fd, sz);
//
//     return fd;
// }
//
// void resz() {
//     int fd = alc_shm(w * h * 4);
//
//     pixl = mmap(0, w * h * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//
//     struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, w * h * 4);
//     bfr = wl_shm_pool_create_buffer(pool, 0, w, h, w * 4, WL_SHM_FORMAT_ARGB8888);
//     wl_shm_pool_destroy(pool);
//     close(fd);
// }
//
// void draw() {
//     memset(pixl, 255, w * h * 4);
//
//     wl_surface_attach(srfc, bfr, 0, 0);
//     wl_surface_damage_buffer(srfc, 0, 0, w, h);
//     wl_surface_commit(srfc);
// }
//
// struct wl_callback_listener cb_list;
//
// void frame_new(void *data, struct wl_callback *cb, uint32_t a) {
//     wl_callback_destroy(cb);
//     cb = wl_surface_frame(srfc);
//     wl_callback_add_listener(cb, &cb_list, 0);
//
//     draw();
// }
//
// // wl_callback_listener cb_list = {.done = frame_new};
//
// void xrfc_conf(void *data, struct xdg_surface *xrfc, uint32_t ser) {
//     xdg_surface_ack_configure(xrfc, ser);
//     if (!pixl) {
//         resz();
//     }
//
//     draw();
// }
//
// struct xdg_surface_listener xrfc_list = { .configure = xrfc_conf };
//
// void top_conf(void *data, struct xdg_toplevel *top, int32_t nw, int32_t nh, struct wl_array *stat) {
//     if (!nw && !nh) {
//         return;
//     }
//
//     if (w != nw || h != nh) {
//         munmap(pixl, w * h * 4);
//         w = nw;
//         h = nh;
//         resz();
//     }
// }
//
// void top_cls(void *data, struct xdg_toplevel *top) {
//     cls = 1;
// }
//
// struct xdg_toplevel_listener top_list = { .configure = top_conf, .close = top_cls };
//
// void sh_ping(void *data, struct xdg_wm_base *sh, uint32_t ser) {
//     xdg_wm_base_pong(sh, ser);
// }
//
// struct xdg_wm_base_listener sh_list = { .ping = sh_ping };
//
// void kb_map(void *data, struct wl_keyboard *kb, uint32_t frmt, int32_t fd, uint32_t sz) {
// }
//
// void kb_enter(void *data, struct wl_keyboard *kb, uint32_t ser, struct wl_surface *srfc, struct wl_array *keys) {
// }
//
// void kb_leave(void *data, struct wl_keyboard *kb, uint32_t ser, struct wl_surface *srfc) {
// }
//
// void kb_key(void *data, struct wl_keyboard *kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t stat) {
//     if (key == 1) {
//         cls = 1;
//     } else if (key == 30) {
//         printf("a\n");
//     } else if (key == 32) {
//         printf("d\n");
//     }
// }
//
// void kb_mod(void *data, struct wl_keyboard *kb, uint32_t ser, uint32_t dep, uint32_t lat, uint32_t lock, uint32_t
// grp) {
// }
//
// void kb_rep(void *data, struct wl_keyboard *kb, int32_t rate, int32_t del) {
// }
//
// struct wl_keyboard_listener kb_list = {
//     .keymap = kb_map, .enter = kb_enter, .leave = kb_leave, .key = kb_key, .modifiers = kb_mod, .repeat_info = kb_rep
// };
//
// void seat_cap(void *data, struct wl_seat *seat, uint32_t cap) {
//     if (cap & WL_SEAT_CAPABILITY_KEYBOARD && !kb) {
//         kb = wl_seat_get_keyboard(seat);
//         wl_keyboard_add_listener(kb, &kb_list, 0);
//     }
// }
//
// void seat_name(void *data, struct wl_seat *seat, const char *name) {
// }
//
// struct wl_seat_listener seat_list = { .capabilities = seat_cap, .name = seat_name };
//
// void reg_glob(void *data, struct wl_registry *reg, uint32_t name, const char *intf, uint32_t v) {
//     if (!strcmp(intf, wl_compositor_interface.name)) {
//         comp = (wl_compositor *)wl_registry_bind(reg, name, &wl_compositor_interface, 4);
//     } else if (!strcmp(intf, wl_shm_interface.name)) {
//         shm = (wl_shm *)wl_registry_bind(reg, name, &wl_shm_interface, 1);
//     } else if (!strcmp(intf, xdg_wm_base_interface.name)) {
//         sh = (xdg_wm_base *)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1);
//         xdg_wm_base_add_listener(sh, &sh_list, 0);
//     } else if (!strcmp(intf, wl_seat_interface.name)) {
//         seat = (wl_seat *)wl_registry_bind(reg, name, &wl_seat_interface, 1);
//         wl_seat_add_listener(seat, &seat_list, 0);
//     }
// }
//
// void reg_glob_rem(void *data, struct wl_registry *reg, uint32_t name) {
// }
//
// struct wl_registry_listener reg_list = { .global = reg_glob, .global_remove = reg_glob_rem };
//
// int main() {
//     cb_list = { .done = frame_new };
//     struct wl_display *disp = wl_display_connect(0);
//     struct wl_registry *reg = wl_display_get_registry(disp);
//     wl_registry_add_listener(reg, &reg_list, 0);
//     wl_display_roundtrip(disp);
//
//     srfc = wl_compositor_create_surface(comp);
//     struct wl_callback *cb = wl_surface_frame(srfc);
//     wl_callback_add_listener(cb, &cb_list, 0);
//
//     struct xdg_surface *xrfc = xdg_wm_base_get_xdg_surface(sh, srfc);
//     xdg_surface_add_listener(xrfc, &xrfc_list, 0);
//     top = xdg_surface_get_toplevel(xrfc);
//     xdg_toplevel_add_listener(top, &top_list, 0);
//     xdg_toplevel_set_title(top, "wayland client");
//     wl_surface_commit(srfc);
//
//     while (wl_display_dispatch(disp)) {
//         if (cls) break;
//     }
//
//     if (kb) {
//         wl_keyboard_destroy(kb);
//     }
//
//     wl_seat_release(seat);
//
//     if (bfr) {
//         wl_buffer_destroy(bfr);
//     }
//
//     xdg_toplevel_destroy(top);
//     xdg_surface_destroy(xrfc);
//     wl_surface_destroy(srfc);
//     wl_display_disconnect(disp);
//
//     return 0;
// }
