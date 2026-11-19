/* ==============================================================================================
 * Sad Test Window
 * ============================================================================================== */

#ifndef SADWINDOW_TEST_H
#define SADWINDOW_TEST_H

#include <Windows.h>

#include "sadlib.h"

static int test_monitor()
{
    #ifdef SAD_TEST_MONITOR
    int monitor_count = 0;
    int i, j, k, count;

    monitor_count = sad_monitor_count();
    printf("monitor_count: %i\n\n", monitor_count);
    

    for (i = 0; i < monitor_count; i++) {
        const char* name = sad_monitor_name(i);
        const sad_display_mode* mode = sad_monitor_mode(i);
        const sad_display_mode* modes;
        const sad_display_res* resolutions;
        int x, y, w, h;
        
        sad_monitor_bounds(i, &x, &y, &w, &h);
        
        printf("monitor_name[%i]: %s\n", i, name);
        printf("display_mode[%i]:\n", i);
        printf("  width: %i\n", mode->width);
        printf("  height: %i\n", mode->height);
        printf("  refresh_rate: %i\n", mode->refresh_rate);
        printf("  color_depth: %i\n", mode->color_depth);
        printf("bounds[%i]:\n", i);
        printf("  x: %i\n", x);
        printf("  y: %i\n", y);
        printf("  width: %i\n", w);
        printf("  height: %i\n\n", h);
        
        modes = sad_monitor_modes(i, &count);
        printf("display_modes[%i]:\n", i);
        for (j = 0; j < count; j++) {
            printf("  mode[%i]:\n", j);
            printf("    width: %i\n", modes[j].width);
            printf("    height: %i\n", modes[j].height);
            printf("    refresh_rate: %i\n", modes[j].refresh_rate);
            printf("    color_depth: %i\n", modes[j].color_depth);
        }
        
        resolutions = sad_monitor_resolutions(i, &count);
        printf("resolutions[%i]:\n", i);
        for (j = 0; j < count; j++) {
            const int* refresh_rates;
            int width = resolutions[j].width;
            int height = resolutions[j].height;
            int c;
            
            refresh_rates = sad_monitor_refreshrates(i, width, height, &c);
            
            printf("  resolution[%i]:\n", j);
            printf("    width: %i\n", resolutions[j].width);
            printf("    height: %i\n", resolutions[j].height);
            
            for (k = 0; k < c; k++) {
                printf("    refresh_rate[%i]: %i\n", j, refresh_rates[k]);
            }
        }
        
        mode = sad_monitor_pickmode(i, 1260, 700, 1000);
        printf("pick_mode(%i, 1260, 700, 1000):\n", i);
        printf("  width: %i\n", mode->width);
        printf("  height: %i\n", mode->height);
        printf("  refresh_rate: %i\n", mode->refresh_rate);
        printf("  color_depth: %i\n", mode->color_depth);
        
        mode = sad_monitor_pickmode(i, 1900, 1100, 64);
        printf("pick_mode(%i, 1900, 1100, 64):\n", i);
        printf("  width: %i\n", mode->width);
        printf("  height: %i\n", mode->height);
        printf("  refresh_rate: %i\n", mode->refresh_rate);
        printf("  color_depth: %i\n", mode->color_depth);
        
        mode = sad_monitor_pickmode(i, 2500, 1400, 180);
        printf("pick_mode(%i, 2500, 1400, 180):\n", i);
        printf("  width: %i\n", mode->width);
        printf("  height: %i\n", mode->height);
        printf("  refresh_rate: %i\n", mode->refresh_rate);
        printf("  color_depth: %i\n", mode->color_depth);
    }
    #endif
    
    return SAD_OK;
}

static int on_msg(sad_wnd* wnd, int msg, const sad_wnd_msg* data)
{
    static char ctrl = 0;
    static char alt = 0;
    static char shift = 0;

    if (msg == SAD_WND_MSG_KEY) {

        // printf("on key msg: %i, %i\n", data->key.code, data->key.state);
        
        if (data->key.code == SAD_KEY_CTRL) {
            ctrl = data->key.state == 1;
        }
        else if (data->key.code == SAD_KEY_ALT) {
            alt = data->key.state == 1;
        }
        else if (data->key.code == SAD_KEY_SHIFT) {
            shift = data->key.state == 1;
        }
        else if (data->key.code == SAD_KEY_ESC) {
            sad_wnd_destroy(wnd);
        }
        else if (data->key.code == '1' && data->key.state == 1) {
            const char* name = sad_wnd_name(wnd);
            printf("[INFO]: old name - %s\n", name);

            sad_wnd_rename(wnd, "Sad Window - Renamed");
            
            name = sad_wnd_name(wnd);
            printf("[INFO]: new name - %s\n", name);
        }
        else if (data->key.code == '2' && data->key.state == 1) {
            int x, y;
            sad_wnd_pos(wnd, &x, &y);
            printf("[INFO]: old pos - [%i, %i]\n", x, y);

            sad_wnd_move(wnd, 64, 64);
            sad_wnd_pos(wnd, &x, &y);
            printf("[INFO]: new pos - [%i, %i]\n", x, y);
        }
        else if (data->key.code == '3' && data->key.state == 1) {
            int x, y;
            sad_wnd_size(wnd, &x, &y);
            printf("[INFO]: old size - [%i, %i]\n", x, y);

            sad_wnd_resize(wnd, 1920, 1080);
            sad_wnd_size(wnd, &x, &y);
            printf("[INFO]: new size - [%i, %i]\n", x, y);
        }
        else if (data->key.code == '4' && data->key.state == 1) {
            int x, y, w, h;
            sad_wnd_bounds(wnd, &x, &y, &w, &h);
            printf("[INFO]: old bounds - [%i, %i, %i, %i]\n", x, y, w, h);

            sad_wnd_place(wnd, 320, 180, 1920, 1080);
            sad_wnd_bounds(wnd, &x, &y, &w, &h);
            printf("[INFO]: new bounds - [%i, %i, %i, %i]\n", x, y, w, h);
        }
        else if (data->key.code == '5' && data->key.state == 1) {
            if (shift) {
                printf("[INFO]: old maximized - %i\n", sad_wnd_maximized(wnd));
                sad_wnd_maximize(wnd);
                printf("[INFO]: new maximized - %i\n", sad_wnd_maximized(wnd));
            }
            else if (ctrl) {
                printf("[INFO]: old minimized - %i\n", sad_wnd_minimized(wnd));
                sad_wnd_minimize(wnd);
                printf("[INFO]: new minimized - %i\n", sad_wnd_minimized(wnd));
                Sleep(2000);
                sad_wnd_restore(wnd);
                printf("[INFO]: new minimized - %i\n", sad_wnd_minimized(wnd));
            }
            else {
                printf("[INFO]: old minimized, maximized - %i, %i\n", sad_wnd_minimized(wnd), sad_wnd_maximized(wnd));
                sad_wnd_restore(wnd);
                printf("[INFO]: new minimized, maximized - %i, %i\n", sad_wnd_minimized(wnd), sad_wnd_maximized(wnd));
            }
        }
        else if (data->key.code == '6' && data->key.state == 1) {
            printf("[INFO]: old visible - %i\n", sad_wnd_visible(wnd));
            sad_wnd_hide(wnd);
            printf("[INFO]: new visible - %i\n", sad_wnd_visible(wnd));
            Sleep(2000);
            sad_wnd_show(wnd);
            printf("[INFO]: new visible - %i\n", sad_wnd_visible(wnd));
        }
        else if (data->key.code == '7' && data->key.state == 1) {
            int x, y, w, h;
            printf("[INFO]: old windowed - %i\n", sad_wnd_mode(wnd) == 0);
            if (shift) {
                sad_wnd_windowed(wnd, 1280, 720);
            } else {
                sad_wnd_windowed(wnd, 0, 0);
            }
            printf("[INFO]: new windowed - %i\n", sad_wnd_mode(wnd) == 0);
            sad_wnd_bounds(wnd, &x, &y, &w, &h);
            printf("[INFO]: new bounds - [%i, %i, %i, %i]\n", x, y, w, h);
        }
        else if (data->key.code == '8' && data->key.state == 1) {
            int x, y, w, h;
            printf("[INFO]: old borderless - %i\n", sad_wnd_mode(wnd) == SAD_WND_BORDERLESS);
            if (shift) {
                sad_wnd_borderless(wnd, 1280, 720);
            } else {
                sad_wnd_borderless(wnd, 0, 0);
            }
            printf("[INFO]: new borderless - %i\n", sad_wnd_mode(wnd) == SAD_WND_BORDERLESS);
            sad_wnd_bounds(wnd, &x, &y, &w, &h);
            printf("[INFO]: new bounds - [%i, %i, %i, %i]\n", x, y, w, h);
        }
        else if (data->key.code == '9' && data->key.state == 1) {
            int x, y, w, h;
            printf("[INFO]: old fullscreen - %i\n", sad_wnd_mode(wnd) == SAD_WND_FULLSCREEN);
            if (shift) {
                sad_display_mode mode = { 1280, 720 };
                sad_wnd_fullscreen(wnd, 1, &mode);
            } else {
                sad_wnd_fullscreen(wnd, 0, NULL);
            }
            printf("[INFO]: new fullscreen - %i\n", sad_wnd_mode(wnd) == SAD_WND_FULLSCREEN);
            sad_wnd_bounds(wnd, &x, &y, &w, &h);
            printf("[INFO]: new bounds - [%i, %i, %i, %i]\n", x, y, w, h);
        }
        else if (data->key.code == '0' && data->key.state == 1) {
            if (ctrl) {
                printf("[INFO]: undecorated %s\n", alt ? "true" : "false");
                sad_wnd_set_flag(wnd, SAD_WND_UNDECORATED, alt);
            } else if (shift) {
                printf("[INFO]: no resize %s\n", alt ? "true" : "false");
                sad_wnd_set_flag(wnd, SAD_WND_NORESIZE, alt);
            } else {
                printf("[INFO]: overframe %s\n", alt ? "true" : "false");
                sad_wnd_set_flag(wnd, SAD_WND_OVERFRAME, alt);
            }
        }
        else if (data->key.code == 'A' && data->key.state == 1) {
            if (ctrl) {
                printf("[INFO]: mica_alt\n");
                sad_wnd_set_backdrop(wnd, SAD_WNDBD_MICA_ALT);
            } else if (shift) {
                printf("[INFO]: transient\n");
                sad_wnd_set_backdrop(wnd, SAD_WNDBD_ACRYLIC);
            } else {
                printf("[INFO]: mica\n");
                sad_wnd_set_backdrop(wnd, SAD_WNDBD_MICA);
            }
        }
        else if (data->key.code == 'M' && data->key.state == 1) {
            printf("[INFO]: monitor: %i\n", sad_wnd_monitor(wnd));
        }
    }
    else if (msg == SAD_WND_MSG_MOUSE_ENTER) {
        printf("[INFO]: Mouse Enter!\n");
    }
    else if (msg == SAD_WND_MSG_MOUSE_LEAVE) {
        printf("[INFO]: Mouse Leave!\n");
    }
    else if (msg == SAD_WND_MSG_MOUSE_MOVE) {
        /*printf("[INFO]: Mouse Move!\n");
        printf("[INFO]: Pos: (%i, %i)\n", data->mouse.x, data->mouse.y);
        printf("[INFO]: Delta: (%i, %i)\n", data->mouse.delta_x, data->mouse.delta_y);*/
    }
    else if (msg == SAD_WND_MSG_MOUSE_SCROLL) {
        printf("[INFO]: Mouse Scroll!\n");
        printf("[INFO]: Delta: (%i, %i)\n", data->scroll.delta_x, data->scroll.delta_y);
    }
    
    if (msg == SAD_WND_MSG_CHAR_CODE) {
        printf("[INFO]: Character: %u\n", data->char_code.val);
    }

    return 0;
}

static int test_window()
{
    #ifdef SAD_TEST_WINDOW

    sad_display_mode display_mode = {
        1280,
        720,
        0
    };

    sad_wnd_config cfg = {
        SAD_WND_CENTER, /* x */
        SAD_WND_CENTER, /* y */
        0,
        &display_mode
    };

    sad_wnd* wnd = sad_wnd_create(
        "My Window", -1, -1,
        // SAD_WND_BORDERLESS,
        // SAD_WND_FULLSCREEN,
        // SAD_WND_UNDECORATED,
        // SAD_WND_OVERFRAME,
        0,
        &cfg
    );

    sad_wnd_set_msg_callback(wnd, &on_msg);

    while (sad_await_msg())
    {
        /* code */
    }

    #endif
    
    return SAD_OK;
}

#endif