/* ==============================================================================================
 * Sad Lib Window
 * ============================================================================================== */

/* TODO
*  - Maybe add error codes to window mode switch
*  - Add option to create desktop background windows
*/

#ifndef SADWINDOW_H
#define SADWINDOW_H

#include "sadcore.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Enums
* ----------------------------------------------------------------------------------------------- */

enum SAD_WND_SPECIAL {
    SAD_WND_CENTER = (INT_MIN + 0),
};

enum SAD_WND_MODES {
    SAD_WND_WINDOWED    = (int)0,
    SAD_WND_BORDERLESS  = (int)1,
    SAD_WND_FULLSCREEN  = (int)2,
};

enum SAD_WND_FLAGS {
    /* Window mode can also be specified in creation flags.
    SAD_WND_WINDOWED    = 0,
    SAD_WND_BORDERLESS  = 1,
    SAD_WND_FULLSCREEN  = 2,
    */

    /* Flags */
    SAD_WND_MINIMIZE    = (int)(1 <<  2),
    SAD_WND_MAXIMIZE    = (int)(1 <<  3),
    SAD_WND_HIDDEN      = (int)(1 <<  4),
    SAD_WND_UNDECORATED = (int)(1 <<  5),
    SAD_WND_NORESIZE    = (int)(1 <<  6),
    SAD_WND_OVERFRAME   = (int)(1 <<  7),
    SAD_WND_TRANSPARENT = (int)(1 <<  8),
};

enum SAD_WND_BACKDROPS {
    SAD_WNDBD_NONE,
    SAD_WNDBD_DEFAULT,
    SAD_WNDBD_BLUR,
    SAD_WNDBD_MICA,
    SAD_WNDBD_MICA_ALT,
    SAD_WNDBD_ACRYLIC,
};

enum SAD_WND_HIT {
    SAD_WNDHIT_NONE,
    SAD_WNDHIT_CAPTION,
    SAD_WNDHIT_TOP,
    SAD_WNDHIT_TOPLEFT,
    SAD_WNDHIT_TOPRIGHT,
    SAD_WNDHIT_BOTTOM,
    SAD_WNDHIT_BOTTOMLEFT,
    SAD_WNDHIT_BOTTOMRIGHT,
    SAD_WNDHIT_LEFT,
    SAD_WNDHIT_RIGHT,
    SAD_WNDHIT_TRANSPARENT, /* May not work on all platforms. */
};

/*
* Type definitions
* ----------------------------------------------------------------------------------------------- */

typedef struct sad_wnd sad_wnd;

typedef struct sad_display_res {
    int width;
    int height;
} sad_display_res;

/* Values <= zero are replaced by the current display_mode. */
typedef struct sad_display_mode {
    int width;
    int height;
    int refresh_rate;
    int color_depth;
} sad_display_mode;

typedef struct sad_wnd_config {
    int x;                                  /* Window position. Supports `SAD_WND_CENTER`. */
    int y;                                  /* Window position. Supports `SAD_WND_CENTER`. */
    int monitor;                            /* Target monitor. Used for positioning and fullscreen. 0 = Primary. */
    const sad_display_mode* display_mode;   /* Optional, fullscreen display_mode. The closest existing match will be chosen. */
    int backdrop;                           /* Window backdrop type, from SAD_WND_BACKDROPS. */
} sad_wnd_config;

typedef void (*sad_wnd_size_callback)(sad_wnd* wnd, int width, int height);
typedef void (*sad_wnd_draw_callback)(sad_wnd* wnd);
typedef int (*sad_wnd_hittest_callback)(sad_wnd* wnd, int x, int y);
/* typedef void (*sad_wnd_input_callback)(sad_wnd* wnd, int code, int x, int y); */

/*
* sad_wnd
* ----------------------------------------------------------------------------------------------- */

/*
* @param width Size of the window. In borderless mode a values less then or equal zero means
* borderless fullscreen. In fullscreen mode this value is ignored, use `cfg.display_mode` to change
* fullscreen behaviour.
* @param height See `width`
* @param flags Window flags from `SAD_WND_FLAGS`.
* @param cfg Optional
* @return A window handle or NULL.
*/
SAD_API sad_wnd* sad_wnd_create(const char* name, int width, int height, int flags, const sad_wnd_config* cfg);
SAD_API void sad_wnd_destroy(sad_wnd* wnd);
SAD_API void* sad_wnd_handle(sad_wnd* wnd);

/*  Setters ----------------------------------------------------------------- */

// SAD_API void sad_wnd_rename(sad_wnd* wnd, const char* name);
// SAD_API void sad_wnd_move(sad_wnd* wnd, int x, int y);
// SAD_API void sad_wnd_resize(sad_wnd* wnd, int w, int h);
// SAD_API void sad_wnd_place(sad_wnd* wnd, int x, int y, int w, int h);
// SAD_API void sad_wnd_minimize(sad_wnd* wnd);
// SAD_API void sad_wnd_maximize(sad_wnd* wnd);
// SAD_API void sad_wnd_restore(sad_wnd* wnd);
SAD_API void sad_wnd_show(sad_wnd* wnd);
// SAD_API void sad_wnd_hide(sad_wnd* wnd);

SAD_API void sad_wnd_windowed(sad_wnd* wnd, int width, int height);
SAD_API void sad_wnd_borderless(sad_wnd* wnd, int width, int height);
SAD_API void sad_wnd_fullscreen(sad_wnd* wnd, int monitor, const sad_display_mode* display_mode);

/* Most flags are changed in dedicated functions, can only modify the following flags:
* 
*  - SAD_WND_UNDECORATED
*  - SAD_WND_NORESIZE
*  - SAD_WND_OVERFRAME
*  - SAD_WND_TRANSPARENT
*  
*  Some flags may not be modified on some platforms. */
// SAD_API void sad_wnd_set_flag(sad_wnd* wnd, int flag, int enabled);
// SAD_API void sad_wnd_set_backdrop(sad_wnd* wnd, int backdrop);

/*  Getters ----------------------------------------------------------------- */

/* The returned pointer is only valid until the name is changed. */
// SAD_API const char* sad_wnd_name(sad_wnd* wnd);
// SAD_API void sad_wnd_pos(sad_wnd* wnd, int* x, int* y);
// SAD_API void sad_wnd_size(sad_wnd* wnd, int* w, int* h);
// SAD_API void sad_wnd_bounds(sad_wnd* wnd, int* x, int* y, int* w, int* h);
// SAD_API int sad_wnd_minimized(sad_wnd* wnd);
// SAD_API int sad_wnd_maximized(sad_wnd* wnd);
// SAD_API int sad_wnd_visible(sad_wnd* wnd);
// SAD_API int sad_wnd_mode(sad_wnd* wnd);
// SAD_API int sad_wnd_flag(sad_wnd* wnd, int flag);
// SAD_API int sad_wnd_backdrop(sad_wnd* wnd);

/* Get size of window decorations/frame. */
// SAD_API void sad_wnd_framesize(sad_wnd* wnd, int* left, int* top, int* right, int* bottom);
// SAD_API void sad_wnd_resolution(sad_wnd* wnd, int* w, int* h);
// SAD_API float sad_wnd_dpi(sad_wnd* wnd);
// SAD_API int sad_wnd_monitor(sad_wnd* wnd);

/*  Control ----------------------------------------------------------------- */

/* Initialize a drag from a hit location. See SAD_WNDHIT_* */
// SAD_API int sad_wnd_drag(sad_wnd* wnd, int hit);

/*  Callbacks --------------------------------------------------------------- */

// SAD_API void sad_wnd_set_on_size(sad_wnd* wnd, sad_wnd_size_callback callback);
// SAD_API void sad_wnd_set_on_resolution(sad_wnd* wnd, sad_wnd_size_callback callback);
// SAD_API void sad_wnd_set_on_dpi(sad_wnd* wnd, sad_wnd_size_callback callback);
// SAD_API void sad_wnd_set_on_draw(sad_wnd* wnd, sad_wnd_draw_callback callback);
// SAD_API void sad_wnd_set_on_hittest(sad_wnd* wnd, sad_wnd_hittest_callback callback);
/* SAD_API void sad_wnd_set_on_input(sad_wnd* wnd, sad_wnd_input_callback callback); */

/*
* sad_monitor
* ----------------------------------------------------------------------------------------------- *//*
*   - 0 is the primary monitor.
*   - Calling any functions with an invalid monitor index, is treated as calling with 0.
*/

SAD_API int sad_monitor_count(void);
SAD_API void sad_monitor_bounds(int monitor, int* x, int* y, int* w, int* h);
SAD_API const char* sad_monitor_name(int monitor);

/*  Mode -------------------------------------------------------------------- */

SAD_API const sad_display_mode* sad_monitor_mode(int monitor);
SAD_API const sad_display_mode* sad_monitor_pickmode(int monitor, int width, int height, int refresh_rate);

SAD_API const sad_display_res* sad_monitor_resolutions(int monitor, int* count);
SAD_API const int* sad_monitor_refreshrates(int monitor, int width, int height, int* count);
SAD_API const sad_display_mode* sad_monitor_modes(int monitor, int* count);

/*
* Event Loop
* ----------------------------------------------------------------------------------------------- */

SAD_API int sad_await_msg(void); /* return: 1 = continue, 0 = exit */
SAD_API int sad_poll_msg(void); /* return: 1 = continue, 0 = exit */

/*
* Type Helpers
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_display_res_cmp(const sad_display_res* a, const sad_display_res* b);
SAD_API sad_int sad_display_mode_cmp(const sad_display_mode* a, const sad_display_mode* b);

#ifdef __cplusplus
}
#endif

#endif /* SADWINDOW_H */