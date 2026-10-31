/* ==============================================================================================
 * Sad Test Window
 * ============================================================================================== */

#ifndef SADWINDOW_TEST_H
#define SADWINDOW_TEST_H

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
        SAD_WND_UNDECORATED,
        // SAD_WND_OVERFRAME,
        &cfg
    );

    while (sad_await_msg())
    {
        /* code */
    }

    #endif
    
    return SAD_OK;
}

#endif