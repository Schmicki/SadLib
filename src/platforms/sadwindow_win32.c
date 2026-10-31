#include "sadcore.h"

#ifdef SAD_OS_WINDOWS

#define SetProcessDPIAware _Win32_SetProcessDPIAware
#define SetThreadDpiAwarenessContext _Win32_SetThreadDpiAwarenessContext
#define SetProcessDpiAwarenessContext _Win32_SetProcessDpiAwarenessContext
#define GetDpiForWindow _Win32_GetDpiForWindow
#define GetDpiForSystem _Win32_GetDpiForSystem

#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>
#include <initguid.h>
// #include <SetupAPI.h>

#undef SetProcessDPIAware
#undef SetThreadDpiAwarenessContext
#undef SetProcessDpiAwarenessContext
#undef GetDpiForWindow
#undef GetDpiForSystem

#include "sadcontainers.h"
#include "sadwindow.h"

/*
* DWM Compatibility
* ----------------------------------------------------------------------------------------------- */

typedef struct DWM_MARGINS {
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} DWM_MARGINS;

#define DWM_BB_ENABLE                 0x00000001
#define DWM_BB_BLURREGION             0x00000002
#define DWM_BB_TRANSITIONONMAXIMIZED  0x00000004

#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600)
#define WM_DWMCOMPOSITIONCHANGED        0x031E
#define WM_DWMNCRENDERINGCHANGED        0x031F
#define WM_DWMCOLORIZATIONCOLORCHANGED  0x0320
#define WM_DWMWINDOWMAXIMIZEDCHANGE     0x0321
#endif /* !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600) */

typedef struct DWM_BLURBEHIND
{
    DWORD dwFlags;
    BOOL fEnable;
    HRGN hRgnBlur;
    BOOL fTransitionOnMaximized;
} DWM_BLURBEHIND;

enum DWMWINDOWATTRIBUTE
{
    DWMWA_NCRENDERING_ENABLED = 1,
    DWMWA_NCRENDERING_POLICY,
    DWMWA_TRANSITIONS_FORCEDISABLED,
    DWMWA_ALLOW_NCPAINT,
    DWMWA_CAPTION_BUTTON_BOUNDS,
    DWMWA_NONCLIENT_RTL_LAYOUT,
    DWMWA_FORCE_ICONIC_REPRESENTATION,
    DWMWA_FLIP3D_POLICY,
    DWMWA_EXTENDED_FRAME_BOUNDS,
    DWMWA_HAS_ICONIC_BITMAP,
    DWMWA_DISALLOW_PEEK,
    DWMWA_EXCLUDED_FROM_PEEK,
    DWMWA_CLOAK,
    DWMWA_CLOAKED,
    DWMWA_FREEZE_REPRESENTATION,
    DWMWA_PASSIVE_UPDATE_MODE,
    DWMWA_USE_HOSTBACKDROPBRUSH,
    DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
    DWMWA_WINDOW_CORNER_PREFERENCE = 33,
    DWMWA_BORDER_COLOR,
    DWMWA_CAPTION_COLOR,
    DWMWA_TEXT_COLOR,
    DWMWA_VISIBLE_FRAME_BORDER_THICKNESS,
    DWMWA_SYSTEMBACKDROP_TYPE,
    DWMWA_REDIRECTIONBITMAP_ALPHA,
    DWMWA_BORDER_MARGINS,
    DWMWA_LAST
};

enum DWM_SYSTEMBACKDROP_TYPE
{
    DWMSBT_AUTO,
    DWMSBT_NONE,
    DWMSBT_MAINWINDOW,
    DWMSBT_TRANSIENTWINDOW,
    DWMSBT_TABBEDWINDOW,
};

typedef HRESULT (WINAPI *DwmIsCompositionEnabledType)(BOOL* pfEnabled);
typedef HRESULT (WINAPI *DwmExtendFrameIntoClientAreaType)(HWND hWnd, const DWM_MARGINS* pMarInset);
typedef HRESULT (WINAPI *DwmEnableBlurBehindWindowType)(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
typedef BOOL (WINAPI *DwmDefWindowProcType)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);

static HMODULE hDwm = NULL;
static DwmIsCompositionEnabledType DwmIsCompositionEnabled = NULL;
static DwmExtendFrameIntoClientAreaType DwmExtendFrameIntoClientArea = NULL;
static DwmEnableBlurBehindWindowType DwmEnableBlurBehindWindow = NULL;
static DwmDefWindowProcType DwmDefWindowProc = NULL;

/*
* Shellscaling Compatibility
* ----------------------------------------------------------------------------------------------- */

typedef enum PROCESS_DPI_AWARENESS {
  PROCESS_DPI_UNAWARE = 0,
  PROCESS_SYSTEM_DPI_AWARE = 1,
  PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
  MDT_EFFECTIVE_DPI = 0,
  MDT_ANGULAR_DPI = 1,
  MDT_RAW_DPI = 2,
  MDT_DEFAULT
} MONITOR_DPI_TYPE;

typedef HRESULT (WINAPI *SetProcessDpiAwarenessType)(PROCESS_DPI_AWARENESS);
typedef HRESULT (WINAPI *GetProcessDpiAwarenessType)(HANDLE hprocess, PROCESS_DPI_AWARENESS *value);
typedef HRESULT (WINAPI *GetDpiForMonitorType)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY);

static HMODULE hShcore = NULL;
static SetProcessDpiAwarenessType SetProcessDpiAwareness = NULL;
static GetProcessDpiAwarenessType GetProcessDpiAwareness = NULL;
static GetDpiForMonitorType GetDpiForMonitor = NULL;

/*
* SetupApi Compatibility
* ----------------------------------------------------------------------------------------------- */

/*
*  Flags controlling what is included in the device information set built
*  by SetupDiGetClassDevs
*/
#define DIGCF_DEFAULT           0x00000001  /* only valid with DIGCF_DEVICEINTERFACE */
#define DIGCF_PRESENT           0x00000002
#define DIGCF_ALLCLASSES        0x00000004
#define DIGCF_PROFILE           0x00000008
#define DIGCF_DEVICEINTERFACE   0x00000010

/*
*  Values specifying the scope of a device property change
*/
#define DICS_FLAG_GLOBAL         0x00000001  /* make change in all hardware profiles */
#define DICS_FLAG_CONFIGSPECIFIC 0x00000002  /* make change in specified profile only */
#define DICS_FLAG_CONFIGGENERAL  0x00000004  /* 1 or more hardware profile-specific changes to follow. */

/*
*  KeyType values for SetupDiCreateDevRegKey, SetupDiOpenDevRegKey, and
*  SetupDiDeleteDevRegKey.
*/
#define DIREG_DEV       0x00000001          /* Open/Create/Delete device key */
#define DIREG_DRV       0x00000002          /* Open/Create/Delete driver key */
#define DIREG_BOTH      0x00000004          /* Delete both driver and Device key */

DEFINE_GUID(GUID_DEVINTERFACE_MONITOR, 0xe6f07b5f, 0xee97, 0x4a90, 0xb0, 0x76, 0x33, 0xf5, 0x7b, 0xf4, 0xea, 0xa7);

/* Define type for reference to device information set */
typedef PVOID HDEVINFO;

/*
*  Device information structure (references a device instance
*  that is a member of a device information set)
*/
typedef struct _SP_DEVINFO_DATA {
    DWORD cbSize;
    GUID  ClassGuid;
    DWORD DevInst;    // DEVINST handle
    ULONG_PTR Reserved;
} SP_DEVINFO_DATA, *PSP_DEVINFO_DATA;

/*
*  Device interface information structure (references a device
*  interface that is associated with the device information
*  element that owns it).
*/
typedef struct _SP_DEVICE_INTERFACE_DATA {
    DWORD cbSize;
    GUID  InterfaceClassGuid;
    DWORD Flags;
    ULONG_PTR Reserved;
} SP_DEVICE_INTERFACE_DATA, *PSP_DEVICE_INTERFACE_DATA;

typedef struct _SP_DEVICE_INTERFACE_DETAIL_DATA_W {
    DWORD  cbSize;
    WCHAR  DevicePath[ANYSIZE_ARRAY];
} SP_DEVICE_INTERFACE_DETAIL_DATA_W, *PSP_DEVICE_INTERFACE_DETAIL_DATA_W;

typedef HDEVINFO (WINAPI *SetupDiGetClassDevsWType)(
    CONST GUID *ClassGuid,
    PCWSTR Enumerator,
    HWND hwndParent,
    DWORD Flags
);

typedef BOOL (WINAPI *SetupDiDestroyDeviceInfoListType)(
    HDEVINFO DeviceInfoSet
);

typedef BOOL (WINAPI *SetupDiOpenDeviceInterfaceWType)(
    HDEVINFO DeviceInfoSet,
    PCWSTR DevicePath,
    DWORD OpenFlags,
    PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData
);

typedef BOOL (WINAPI *SetupDiGetDeviceInterfaceDetailWType)(
    HDEVINFO DeviceInfoSet,
    PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,
    PSP_DEVICE_INTERFACE_DETAIL_DATA_W DeviceInterfaceDetailData,
    DWORD DeviceInterfaceDetailDataSize,
    PDWORD RequiredSize,
    PSP_DEVINFO_DATA DeviceInfoData
);

typedef HKEY (WINAPI *SetupDiOpenDevRegKeyType)(
    HDEVINFO DeviceInfoSet,
    PSP_DEVINFO_DATA DeviceInfoData,
    DWORD Scope,
    DWORD HwProfile,
    DWORD KeyType,
    REGSAM samDesired
);

static HMODULE hSetupApi = NULL;
static SetupDiGetClassDevsWType SetupDiGetClassDevsW = NULL;
static SetupDiDestroyDeviceInfoListType SetupDiDestroyDeviceInfoList = NULL;
static SetupDiOpenDeviceInterfaceWType SetupDiOpenDeviceInterfaceW = NULL;
static SetupDiGetDeviceInterfaceDetailWType SetupDiGetDeviceInterfaceDetailW = NULL;
static SetupDiOpenDevRegKeyType SetupDiOpenDevRegKey = NULL;

/*
* User32 Compatibility
* ----------------------------------------------------------------------------------------------- */

#ifndef WM_DPICHANGED
#define WM_DPICHANGED                   0x02E0
#endif /* WM_DPICHANGED */

#ifndef DPI_AWARENESS_CONTEXT_UNAWARE
#define DPI_AWARENESS_CONTEXT_UNAWARE              ((DPI_AWARENESS_CONTEXT)-1)
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED    ((DPI_AWARENESS_CONTEXT)-5)
#endif /* DPI_AWARENESS_CONTEXT_UNAWARE */

typedef BOOL (WINAPI *SetProcessDPIAwareType)(VOID);
typedef VOID* (WINAPI *SetThreadDpiAwarenessContextType)(VOID*);
typedef BOOL (WINAPI *SetProcessDpiAwarenessContextType)(VOID*);
typedef UINT (WINAPI *GetDpiForWindowType)(HWND hwnd);
typedef UINT (WINAPI *GetDpiForSystemType)();

static HMODULE hUser = NULL;
static SetProcessDPIAwareType SetProcessDPIAware = NULL;
static SetThreadDpiAwarenessContextType SetThreadDpiAwarenessContext = NULL;
static SetProcessDpiAwarenessContextType SetProcessDpiAwarenessContext = NULL;
static GetDpiForWindowType GetDpiForWindow = NULL;
static GetDpiForSystemType GetDpiForSystem = NULL;

/*
* Type Definitions
* ----------------------------------------------------------------------------------------------- */

typedef struct sad_wnd {
    HWND handle;
    char* name;
    short flags;
    short backdrop;

    /* fullscreen settings */
    int monitor;
    sad_display_mode display_mode;   /* display_mode used in current fullscreen window */

    /* cached window placement for mode switches */
    WINDOWPLACEMENT old_wp;

    /* event callbacks */
    sad_wnd_size_callback on_size;
    sad_wnd_size_callback on_resolution;
    sad_wnd_draw_callback on_draw;
    sad_wnd_hittest_callback on_hittest;
    /*sad_wnd_input_callback on_input;*/
} sad_wnd;

/* internal data structure used for caching display modes */
typedef struct display_mode_ctx {
    int initialized;
    WCHAR device[32];
    sad_fmap modes;
    sad_fmap resolutions;
    sad_fmap refresh_rates;
    int width; /* current refresh_rates resolution */
    int height;
} display_mode_ctx;

/*
* Global State
* ----------------------------------------------------------------------------------------------- */

static sad_wnd* g_sad_main_window = NULL;

/*
* Process Helper Declarations
* ----------------------------------------------------------------------------------------------- */

static void _wnd_load_ext();
static void _wnd_free_ext();

static void _wnd_setdpiaware();
static void _wnd_getdpi(sad_wnd* wnd, int* x, int* y);

/*
* Window Helper Declarations
* ----------------------------------------------------------------------------------------------- */

/* initialize window bounds from parameters */
static void _wnd_bounds(RECT* bounds, int width, int height, int flags, int style,
    const sad_wnd_config* cfg);

/* create os window */
static sad_wnd* _wnd_create(const char* name, const RECT* bounds, int style, int flags);

/* make sure no impossible flag combinations exist */
static int _wnd_flags_sanitize(int flags);

/* get win32 window style for flag combination */
static int _wnd_flags_to_style(int flags);

/* Apply new window style when mode changed. */
static void _wnd_apply_style(sad_wnd* wnd);

/* Apply dwm frame settings, called inside WM_ACTIVATE. */
static void _wnd_apply_frame(sad_wnd* wnd);

static void _wnd_enter_fullscreen(sad_wnd* wnd);
static void _wnd_leave_fullscreen(sad_wnd* wnd);

/* win32 window proc */
static LRESULT CALLBACK _wnd_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

/*
* Monitor Helper Declarations
* ----------------------------------------------------------------------------------------------- */

static HMONITOR sad_monitor_from_index(int monitor);
// static int sad_monitor_set_mode(int monitor, const sad_display_mode* display_mode);
static void sad_monitor_reset_mode(int monitor);

static sad_display_mode_ctx_init(display_mode_ctx* ctx);
static sad_display_mode_ctx_destroy(display_mode_ctx* ctx);
static display_mode_ctx* sad_display_mode_getctx(int monitor);
static sad_int sad_refresh_rate_cmp(const int* a, const int* b);

/*
* sad_wnd
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_wnd* sad_wnd_create(const char *name, int width, int height, int flags, const sad_wnd_config *cfg)
{
    RECT bounds = {0};
    int style;

    sad_wnd* wnd;

    _wnd_load_ext();
    _wnd_setdpiaware();

    flags = _wnd_flags_sanitize(flags);
    style = _wnd_flags_to_style(flags);

    _wnd_bounds(&bounds, width, height, flags, style, cfg);

    /* create "unmapped" window */
    wnd = _wnd_create(name, &bounds, style, flags);

    if (!wnd) return NULL;

    /* store fullscreen settings */
    if (flags & SAD_WND_FULLSCREEN) {
        wnd->monitor = cfg? cfg->monitor : 0;
        memset(&wnd->display_mode, 0, sizeof(sad_display_mode));

        if (cfg && cfg->display_mode)
            wnd->display_mode = *cfg->display_mode;
    }

    /* "map" window */
    if (!(flags & SAD_WND_HIDDEN)) {
        wnd->flags |= SAD_WND_HIDDEN; /* could be required in the future */
        sad_wnd_show(wnd);
    }

    return wnd;
}

SAD_API void sad_wnd_destroy(sad_wnd *wnd)
{
    DestroyWindow(wnd->handle);
    free(wnd->name);
    free(wnd);

    if (g_sad_main_window == wnd)
        g_sad_main_window = NULL;
}

SAD_API void *sad_wnd_handle(sad_wnd *wnd)
{
    return wnd->handle;
}

/*  Setters ----------------------------------------------------------------- */

// void sad_wnd_set_title(sad_wnd* wnd, const char* title)
// {
//     wchar_t* wtitle = sad__mbstowcs(title);
//     SetWindowTextW(wnd->handle, wtitle);
//     free(wtitle);
// }

// void sad_wnd_move(sad_wnd* wnd, int x, int y)
// {
//     int w, h;
//     /* TODO: handle behaviour when in fullscreen and borderless modes */
//     sad_wnd_size(wnd, &w, &h);
//     MoveWindow(wnd->handle, x, y, w, h, TRUE);
// }

// void sad_wnd_resize(sad_wnd* wnd, int w, int h)
// {
//     int x, y;
//     /* TODO: change fullscreen resolution when in fullscreen mode */
//     sad_wnd_pos(wnd, &x, &y);
//     MoveWindow(wnd->handle, x, y, w, h, TRUE);
// }

// void sad_wnd_place(sad_wnd* wnd, int x, int y, int w, int h)
// {
//     MoveWindow(wnd->handle, x, y, w, h, TRUE);
// }

// void sad_wnd_minimize(sad_wnd* wnd)
// {
//     if (wnd->flags & SAD_WND_MINIMIZE)
//         return;
    
//     wnd->flags |= SAD_WND_MINIMIZE;
//     wnd->flags &= ~SAD_WND_MAXIMIZE;

//     if (IsWindowVisible(wnd->handle)) {
//         ShowWindow(wnd->handle, SW_MINIMIZE);
//     }
// }

// void sad_wnd_maximize(sad_wnd* wnd)
// {
//     if (wnd->flags & SAD_WND_MAXIMIZE)
//         return;
    
//     if (wnd->flags & (SAD_WND_BORDERLESS | SAD_WND_FULLSCREEN)) {
//         wnd->cached_wp.showCmd = SW_MAXIMIZE;
//         sad_window_restore(wnd);
//         return;
//     }
    
//     wnd->flags |= SAD_WND_MAXIMIZE;
//     wnd->flags &= ~SAD_WND_MINIMIZE;

//     if (IsWindowVisible(wnd->handle)) {
//         ShowWindow(wnd->handle, SW_MAXIMIZE);
//     }
// }

// void sad_wnd_restore(sad_wnd* wnd)
// {
//     if ((wnd->flags & SAD_WND_MINIMIZE) ||
//         (wnd->flags & SAD_WND_MAXIMIZE))
//         return;
    
//     wnd->flags &= ~(SAD_WND_MINIMIZE | SAD_WND_MAXIMIZE);

//     if (IsWindowVisible(wnd->handle)) {
//         ShowWindow(wnd->handle, SW_SHOWNORMAL);
//     }
// }

void sad_wnd_show(sad_wnd* wnd)
{
    int show_cmd;

    if (IsWindowVisible(wnd->handle))
        return;
    
    wnd->flags &= ~SAD_WND_HIDDEN;

    if (wnd->flags & (SAD_WND_BORDERLESS | SAD_WND_FULLSCREEN)) {
        show_cmd = (wnd->flags & SAD_WND_MINIMIZE) ? SW_SHOWMINIMIZED : SW_SHOWNORMAL;
    }
    else {
        show_cmd = (wnd->flags & SAD_WND_MINIMIZE) ? SW_SHOWMINIMIZED :
                   (wnd->flags & SAD_WND_MAXIMIZE) ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
    }
    ShowWindow(wnd->handle, show_cmd);
}

// void sad_wnd_hide(sad_wnd* wnd)
// {
//     if (!IsWindowVisible(wnd->handle))
//         return;
    
//     wnd->flags |= SAD_WND_HIDDEN;
//     ShowWindow(wnd->handle, SW_HIDE);
// }

// void sad_wnd_windowed(sad_wnd* wnd)
// {
//     int flags = wnd->flags;

//     if (!(wnd->flags & (SAD_WND_BORDERLESS | SAD_WND_FULLSCREEN)))
//         return;
        
//     wnd->flags &= ~(SAD_WND_BORDERLESS | SAD_WND_FULLSCREEN);
//     wnd->flags &= ~(SAD_WND_MINIMIZE | SAD_WND_MAXIMIZE);

//     if (wnd->cached_wp.showCmd == SW_MAXIMIZE) {
//         wnd->flags |= SAD_WND_MAXIMIZE;
//     }

//     sad__apply_style(wnd, flags);
//     sad__update_dwm_frame(wnd);
//     SetWindowPlacement(wnd->handle, &wnd->cached_wp);
// }

// void sad_wnd_borderless(sad_wnd* wnd)
// {
//     int x, y, w, h;
//     if (wnd->flags & SAD_WND_BORDERLESS)
//         return;
    
//     if (!(wnd->flags & SAD_WND_FULLSCREEN)) {
//         int flags = wnd->flags;
//         GetWindowPlacement(wnd->handle, &wnd->cached_wp);
        
//         wnd->flags = (wnd->flags & ~0x3) | SAD_WND_BORDERLESS;
//         wnd->flags &= ~(SAD_WND_MINIMIZE | SAD_WND_MAXIMIZE);

//         sad__apply_style(wnd, flags);
//         sad__update_dwm_frame(wnd);
//     }

//     sad__get_screen_rect(wnd->handle, &x, &y, &w, &h);
//     MoveWindow(wnd->handle, x, y, w, h, 1);
// }

// void sad_wnd_fullscreen(sad_wnd* wnd)
// {

// }

// void sad_wnd_set_flag(sad_wnd* wnd, int flag, int enabled)
// {

// }

// void sad_wnd_set_backdrop(sad_wnd* wnd, int backdrop)
// {

// }

/*  Getters ----------------------------------------------------------------- */

// char *sad_wnd_title(sad_wnd *wnd)
// {
//     int len;
//     wchar_t* wbuf;
//     char* buf;
    
//     len = GetWindowTextLengthW(wnd->handle) + 1;
//     wbuf = malloc(sizeof(wchar_t) * len);
//     GetWindowTextW(wnd->handle, wbuf, len);

//     buf = sad__wcstombs(wbuf);
//     free(wbuf);
//     return buf;
// }

// void sad_wnd_pos(sad_wnd *wnd, int *x, int *y)
// {
//     RECT r;
//     GetClientRect(wnd->handle, &r);
//     ClientToScreen(wnd->handle, (POINT*)&r);
//     *x = r.left;
//     *y = r.top;
// }

// void sad_wnd_size(sad_wnd *wnd, int *w, int *h)
// {
//     RECT r;
//     GetClientRect(wnd->handle, &r);
//     *w = r.right;
//     *h = r.bottom;
// }

// void sad_wnd_bounds(sad_wnd* wnd, int* x, int* y, int* w, int* h)
// {
//     RECT r;
//     GetClientRect(wnd->handle, &r);
//     ClientToScreen(wnd->handle, (POINT*)&r);
//     *x = r.left;
//     *y = r.top;
//     *w = r.right;
//     *h = r.bottom;
// }

// int sad_wnd_minimized(sad_wnd* wnd)
// {
//     return IsIconic(wnd->handle);
// }

// int sad_wnd_maximized(sad_wnd* wnd)
// {
//     return IsZoomed(wnd->handle);
// }

// int sad_wnd_visible(sad_wnd* wnd)
// {
//     return IsWindowVisible(wnd->handle);
// }

// /* SAD_WND_BORDERLESS, SAD_WND_FULLSCREEN or 0 for windowed */
// int sad_wnd_mode(sad_wnd* wnd)
// {
//     return wnd->flags & (SAD_WND_FULLSCREEN | SAD_WND_BORDERLESS);
// }

// int sad_wnd_flag(sad_wnd* wnd, int flag)
// {
//     return (wnd->flags & flag) != 0;
// }

// int sad_wnd_backdrop(sad_wnd* wnd)
// {
//     return wnd->backdrop;
// }


// void sad_wnd_framesize(sad_wnd* wnd, int* left, int* top, int* right, int* bottom)
// {
//     RECT r = {0};
//     AdjustWindowRectEx(&r, (LONG)GetWindowLongPtrA(wnd->handle, GWL_STYLE), FALSE, 0);
//     *left = -r.left;
//     *top = -r.top;
//     *right = r.right;
//     *bottom = r.bottom;
// }

// void sad_wnd_resolution(sad_wnd* wnd, int* w, int* h)
// {
//     RECT r;
//     GetClientRect(wnd->handle, &r);
//     *w = r.right;
//     *h = r.bottom;
// }

// float sad_wnd_dpi(sad_wnd* wnd)
// {
//     int x, y;
//     _wnd_getdpi(wnd, &x, &y);
//     return (float)x / 96.0f;
// }

// int sad_wnd_monitor(sad_wnd *wnd)
// {
//     int i = 0;
//     int count = 0;
//     DISPLAY_DEVICEA device = { sizeof(DISPLAY_DEVICEA) };
//     HMONITOR monitor = MonitorFromWindow(wnd->handle, MONITOR_DEFAULTTONULL);
//     MONITORINFOEXA minfo = { sizeof(MONITORINFOEXA) };

//     if (!monitor || !GetMonitorInfoA(monitor, (LPMONITORINFO)&minfo))
//         return -1;

//     while (EnumDisplayDevicesA(NULL, i++, &device, 0)) {
//         if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
//             continue;
//         if (strncmp(device.DeviceName, minfo.szDevice, CCHDEVICENAME) == 0)
//             return count;
//         count++;
//     }
    
//     return -1;
// }

/*  Callbacks --------------------------------------------------------------- */

// void sad_wnd_set_on_size(sad_wnd* wnd, sad_wnd_size_callback callback)
// {

// }

// void sad_wnd_set_on_resolution(sad_wnd* wnd, sad_wnd_size_callback callback)
// {

// }

// void sad_wnd_set_on_dpi(sad_wnd* wnd, sad_wnd_size_callback callback)
// {

// }

// void sad_wnd_set_on_draw(sad_wnd* wnd, sad_wnd_draw_callback callback)
// {

// }

// void sad_wnd_set_on_hittest(sad_wnd* wnd, sad_wnd_hittest_callback callback)
// {

// }

/*
* sad_monitor
* ----------------------------------------------------------------------------------------------- */

SAD_API int sad_monitor_count(void)
{
    return GetSystemMetrics(SM_CMONITORS);
}

SAD_API void sad_monitor_bounds(int monitor, int* x, int* y, int* w, int* h)
{
    HMONITOR hm = sad_monitor_from_index(monitor);
	MONITORINFO mi = { sizeof(MONITORINFO) };

	if (!GetMonitorInfo(hm, &mi))
		return;

	*x = mi.rcMonitor.left;
	*y = mi.rcMonitor.top;
	*w = mi.rcMonitor.right - mi.rcMonitor.left;
	*h = mi.rcMonitor.bottom - mi.rcMonitor.top;
}

SAD_API const char *sad_monitor_name(int monitor)
{
    WCHAR dev_id[128];
    BYTE edid[256];
    DWORD edid_size = 256;
    
    static char name[32];

    memcpy(name, "Generic Monitor", 16);
    
    {
        /* Get device interface name */
        HMONITOR hm = sad_monitor_from_index(monitor);
        MONITORINFOEXW mi = { sizeof(MONITORINFOEXW) };
        DISPLAY_DEVICEW dev = { sizeof(DISPLAY_DEVICEW) };
        
        if (!GetMonitorInfoW(hm, (MONITORINFO*)&mi))
            return name;
        
        if (!EnumDisplayDevicesW(mi.szDevice, 0, &dev, EDD_GET_DEVICE_INTERFACE_NAME))
            return name;
        
        memcpy(dev_id, dev.DeviceID, sizeof(dev.DeviceID));
    }
    
    {
        /* Get EDID from registry */
        HDEVINFO hdi;
        SP_DEVICE_INTERFACE_DATA di_data = { sizeof(SP_DEVICE_INTERFACE_DATA) };
        SP_DEVINFO_DATA dev_info = { sizeof(SP_DEVINFO_DATA) };
        
        HKEY hkey;
        DWORD type;
        LSTATUS result;

        _wnd_load_ext();

        if (!SetupDiGetClassDevsW)
            return name;

        /* device information of all present monitors */
        hdi = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_MONITOR, NULL, NULL, DIGCF_PRESENT |
            DIGCF_DEVICEINTERFACE);
        
        if (hdi == INVALID_HANDLE_VALUE)
            return name;
        
        if (!SetupDiOpenDeviceInterfaceW(hdi, dev_id, 0, &di_data)) {
            SetupDiDestroyDeviceInfoList(hdi);
            return name;
        }

        SetupDiGetDeviceInterfaceDetailW(hdi, &di_data, NULL, 0, NULL, &dev_info);

        hkey = SetupDiOpenDevRegKey(hdi, &dev_info, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
        if (hkey == INVALID_HANDLE_VALUE) {
            SetupDiDestroyDeviceInfoList(hdi);
            return name;
        }
        
        result = RegQueryValueExW(hkey, L"EDID", NULL, &type, edid, &edid_size);

        RegCloseKey(hkey);
        SetupDiDestroyDeviceInfoList(hdi);

        if (result != ERROR_SUCCESS || type != REG_BINARY)
            return name;
    }

    {
        /* Parse EDID information */
        /* Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\(DeviceUd)\Device Parameters\EDID */
        /* https://en.wikipedia.org/wiki/Extended_Display_Identification_Data */
        /* text is encoded in CP-437: https://www.ascii-codes.com and terminated with LF (only for
        *  early termination, before 13/18 bytes). */

        unsigned int manid = ((unsigned int)edid[0x8] << 8) | ((unsigned int)edid[0x9]);
        char manufacturer[4] = {
            ((manid >> 10)  & 0x1F) + 0x40,
            ((manid >> 5)  & 0x1F) + 0x40,
            ((manid >> 0) & 0x1F) + 0x40
        };
        int i, j, off = 0x48;

        memcpy(name, manufacturer, 4);

        for (i = 0; i < 3; i++, off += 18) {
            char* mem = edid + off + 5;
            if (memcmp(edid + off, "\0\0\0\xFC\0", 5) != 0)
                continue;
            
            name[3] = '-';
            for (j = 0; j < 13 && mem[j] !='\n'; j++)
                name[j + 4] = mem[j];
            name[j + 4] = 0;
        }
    }

    return name;
}

/*  Mode -------------------------------------------------------------------- */

SAD_API const sad_display_mode* sad_monitor_mode(int monitor)
{
    static sad_display_mode mode = {0};
    display_mode_ctx* ctx = sad_display_mode_getctx(monitor);
    DEVMODEW dm;

    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;

    if (!EnumDisplaySettingsExW(ctx->device, ENUM_CURRENT_SETTINGS, &dm, 0))
        return NULL;
    
    mode.width = (int)dm.dmPelsWidth;
    mode.height = (int)dm.dmPelsHeight;
    mode.color_depth = (int)dm.dmBitsPerPel;
    mode.refresh_rate = (int)dm.dmDisplayFrequency;
    return &mode;
}

SAD_API const sad_display_mode *sad_monitor_pickmode(int monitor, int width, int height, int refresh_rate)
{
    int count;
    const sad_display_mode* modes = sad_monitor_modes(monitor, &count);
    const sad_display_mode* mode = sad_monitor_mode(monitor);
    sad_int i, index = 0;

    int min_delta_res = 0xFFFF;
    int min_delta_herz = 0xFFFF;

    if (width <= 0) width = mode->width;
    if (height <= 0) height = mode->height;
    if (refresh_rate <= 0) refresh_rate = mode->refresh_rate;

    for (i = 0; i < count; i++) {
        int delta_width = abs(width - modes[i].width);
        int delta_height = abs(height - modes[i].height);
        int delta_res = delta_width + delta_height;
        int delta_herz = abs(refresh_rate - modes[i].refresh_rate);

        if (delta_res > min_delta_res)
            continue;
        
        if (delta_res < min_delta_res || delta_herz < min_delta_herz) {
            mode = modes + i;
            min_delta_res = delta_res;
            min_delta_herz = delta_herz;
            index = i;
        }
    }

    return modes + index;
}

SAD_API const sad_display_mode* sad_monitor_modes(int monitor, int* count)
{
    display_mode_ctx* ctx = sad_display_mode_getctx(monitor);

    if (count) *count = ctx->modes.len;
    return (sad_display_mode*)ctx->modes.buckets;
}

SAD_API const sad_display_res* sad_monitor_resolutions(int monitor, int *count)
{
    display_mode_ctx* ctx = sad_display_mode_getctx(monitor);
    
    if (count) *count = ctx->resolutions.len;
    return (sad_display_res*)ctx->resolutions.buckets;
}

SAD_API const int* sad_monitor_refreshrates(int monitor, int width, int height, int* count)
{
    display_mode_ctx* ctx = sad_display_mode_getctx(monitor);
    sad_display_mode mode = {width, height, 0, 0};
    sad_int i;

    sad_fmap_getidx(&ctx->modes, &mode, &i);

    /* re/create refresh_rates */
    sad_fmap_destroy(&ctx->refresh_rates);
    sad_fmap_init(&ctx->refresh_rates, sizeof(int), 8, sad_refresh_rate_cmp);

    for (; i < ctx->modes.len; i++) {
        const sad_display_mode* p = ((sad_display_mode*)ctx->modes.buckets) + i;
        
        if (p->width != width || p->height != height)
            break;
        
        sad_fmap_push(&ctx->refresh_rates, &p->refresh_rate);
    }

    if (count) *count = ctx->refresh_rates.len;
    return (int*)ctx->refresh_rates.buckets;
}

/*
* Event Loop
* ----------------------------------------------------------------------------------------------- */

SAD_API int sad_await_msg(void)
{
    MSG msg;
    if (!GetMessageA(&msg, NULL, 0, 0))
        return 0;

    TranslateMessage(&msg);
    DispatchMessageA(&msg);
    return 1;
}

SAD_API int sad_poll_msg(void)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return 0;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 1;
}

/*
* Type Helpers
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_display_res_cmp(const sad_display_res *a, const sad_display_res *b)
{
    sad_int result;
    if ((result = (sad_int)(a->width - b->width))) return result;
    if ((result = (sad_int)(a->height - b->height))) return result;
    return 0;
}

SAD_API sad_int sad_display_mode_cmp(const sad_display_mode* a, const sad_display_mode* b)
{
    sad_int result;
    if ((result = (sad_int)(a->width - b->width))) return result;
    if ((result = (sad_int)(a->height - b->height))) return result;
    if ((result = (sad_int)(a->refresh_rate - b->refresh_rate))) return result;
    if ((result = (sad_int)(a->color_depth - b->color_depth))) return result;
    return 0;
}

/*
* Process Helper Implementation
* ----------------------------------------------------------------------------------------------- */

static void _wnd_load_ext()
{
    if (hUser) return;

    if ((hUser = LoadLibraryW(L"User32.dll"))) {
        SetProcessDPIAware = (SetProcessDPIAwareType)GetProcAddress(
            hUser, "SetProcessDPIAware");
        SetThreadDpiAwarenessContext = (SetThreadDpiAwarenessContextType)GetProcAddress(
            hUser, "SetThreadDpiAwarenessContext");
        SetProcessDpiAwarenessContext = (SetProcessDpiAwarenessContextType)GetProcAddress(
            hUser, "SetProcessDpiAwarenessContext");
        GetDpiForWindow = (GetDpiForWindowType)GetProcAddress(
            hUser, "GetDpiForWindow");
        GetDpiForSystem = (GetDpiForSystemType)GetProcAddress(
            hUser, "GetDpiForSystem");
    }
    if ((hDwm = LoadLibraryW(L"Dwmapi.dll"))) {
        DwmIsCompositionEnabled = (DwmIsCompositionEnabledType)GetProcAddress(
            hDwm, "DwmIsCompositionEnabled");
        DwmExtendFrameIntoClientArea = (DwmExtendFrameIntoClientAreaType)GetProcAddress(
            hDwm, "DwmExtendFrameIntoClientArea");
        DwmEnableBlurBehindWindow = (DwmEnableBlurBehindWindowType)GetProcAddress(
            hDwm, "DwmEnableBlurBehindWindow");
        DwmDefWindowProc = (DwmDefWindowProcType)GetProcAddress(
            hDwm, "DwmDefWindowProc");
    }
    if ((hShcore = LoadLibraryW(L"Shcore.dll"))) {
        SetProcessDpiAwareness = (SetProcessDpiAwarenessType)GetProcAddress(
            hShcore, "SetProcessDpiAwareness");
        GetProcessDpiAwareness = (GetProcessDpiAwarenessType)GetProcAddress(
            hShcore, "GetProcessDpiAwareness");
        GetDpiForMonitor = (GetDpiForMonitorType)GetProcAddress(
            hShcore, "GetDpiForMonitor");
    }
    if ((hSetupApi = LoadLibraryW(L"SetupApi.dll"))) {
        SetupDiGetClassDevsW = (SetupDiGetClassDevsWType)GetProcAddress(
            hSetupApi, "SetupDiGetClassDevsW");
        SetupDiDestroyDeviceInfoList = (SetupDiDestroyDeviceInfoListType)GetProcAddress(
            hSetupApi, "SetupDiDestroyDeviceInfoList");
        SetupDiOpenDeviceInterfaceW = (SetupDiOpenDeviceInterfaceWType)GetProcAddress(
            hSetupApi, "SetupDiOpenDeviceInterfaceW");
        SetupDiGetDeviceInterfaceDetailW = (SetupDiGetDeviceInterfaceDetailWType)GetProcAddress(
            hSetupApi, "SetupDiGetDeviceInterfaceDetailW");
        SetupDiOpenDevRegKey = (SetupDiOpenDevRegKeyType)GetProcAddress(
            hSetupApi, "SetupDiOpenDevRegKey");
    }
}

static void _wnd_free_ext()
{
    if (hUser) {
        FreeLibrary(hUser);
        hUser = NULL;
        SetProcessDPIAware = NULL;
        SetProcessDpiAwarenessContext = NULL;
    }
    if (hDwm) {
        FreeLibrary(hDwm);
        hDwm = NULL;
        DwmIsCompositionEnabled = NULL;
        DwmExtendFrameIntoClientArea = NULL;
        DwmEnableBlurBehindWindow = NULL;
        DwmDefWindowProc = NULL;
    }
    if (hShcore) {
        FreeLibrary(hShcore);
        hShcore = NULL;
        SetProcessDpiAwareness = NULL;
        GetProcessDpiAwareness = NULL;
        GetDpiForMonitor = NULL;
    }
    if (hSetupApi) {
        FreeLibrary(hSetupApi);
        hSetupApi = NULL;
        SetupDiGetClassDevsW = NULL;
        SetupDiDestroyDeviceInfoList = NULL;
        SetupDiOpenDeviceInterfaceW = NULL;
        SetupDiOpenDevRegKey = NULL;
    }
}

static void _wnd_setdpiaware() {
    if (SetThreadDpiAwarenessContext) {
        /* Windows 10 1607+ */
        if (SetProcessDpiAwarenessContext) {
            /* Windows 10 1703+ */
            SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        } else {
            SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
        }
    }
    else if (SetProcessDpiAwareness) {
        /* Windows 8.1+ */
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    }
    else if (SetProcessDPIAware != NULL) {
        /* Windows Vista+ */
        SetProcessDPIAware();
    }
}

static void _wnd_getdpi(sad_wnd* wnd, int *x, int *y)
{
    HDC hdc;
    UINT dpiX = 96;
    UINT dpiY = 96;

    /* Windows 10 1607+ */
    if (wnd && GetDpiForWindow) {
        dpiX = GetDpiForWindow(wnd->handle);
        *x = dpiX;
        *y = dpiX;
        return;
    }
    
    /* Windows 8.1+ */
    if (GetDpiForMonitor) {
        HMONITOR mon = MonitorFromWindow(wnd ? wnd->handle : NULL, MONITOR_DEFAULTTOPRIMARY);
        GetDpiForMonitor(mon, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
        *x = (int)dpiX;
        *y = (int)dpiX;
        return;
    }
    
    /* Windows 2000+ */
    hdc = GetDC(NULL); /* Retrieves the screen DC */
    if (hdc) {
        dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
        *x = dpiX;
        *y = dpiX;
        ReleaseDC(NULL, hdc);
    }
}

/*
* Window Helper Implementation
* ----------------------------------------------------------------------------------------------- */

static void _wnd_bounds(RECT* bounds, int width, int height, int flags, int style, const sad_wnd_config* cfg)
{
    int mx, my, mw, mh;
    
    sad_monitor_bounds(cfg? cfg->monitor : 0, &mx, &my, &mw, &mh);

    /* resolve size */
    if (flags & SAD_WND_BORDERLESS) {
        bounds->right = width <= 0 ? mw : width;
        bounds->bottom = height <= 0 ? mh : height;
    }
    else if (flags & SAD_WND_FULLSCREEN) {
        bounds->right = width <= 0 ? 768 : width;
        bounds->bottom = height <= 0 ? 454 : height;
    }
    else {
        bounds->right = (width < 0) ? 768 : width;
        bounds->bottom = (height < 0) ? 454 : height;
    }

    /* resolve position */
    bounds->left = mx + ((cfg && cfg->x != SAD_WND_CENTER) ? cfg->x : ((mw - bounds->right) / 2));
    bounds->top = my + ((cfg && cfg->y != SAD_WND_CENTER) ? cfg->y : ((mh - bounds->bottom) / 2));
    
    /* adjust rect to include frame */
    {
        RECT r = {0};
        AdjustWindowRectEx(&r, (DWORD)style, FALSE, 0);
        
        /* Undecorated windows do not have a top, but a style containing `WS_THICKFRAME` still
        * contains a top border that we remove using `WM_NCCALCSIZE` and manual `WM_NCHITTEST`. */
        if ((flags & SAD_WND_UNDECORATED)) {
            r.top = 0;
        }

        bounds->left += r.left;
        bounds->top += r.top;
        bounds->right += -r.left + r.right;
        bounds->bottom += -r.top + r.bottom;
    }
}

static sad_wnd* _wnd_create(const char* name, const RECT* bounds, int style, int flags)
{
    HINSTANCE inst;
    WNDCLASSW wc = {0};
    const WCHAR* wc_name;
    WCHAR* wnd_name;
    
    sad_wnd* wnd;
    
    inst = GetModuleHandleW(NULL);
    wc_name = L"sad_wnd_class_default";

    if (!GetClassInfoW(inst, wc_name, &wc)) {
        /* register window class */
        WNDCLASSW wc = {0};
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = &_wnd_wndproc;
        wc.hInstance = inst;
        wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = wc_name;
        if (!RegisterClassW(&wc)) {
            printf("ERROR: sad_wnd_create -> _wnd_create -> RegisterClassW\n");
            return NULL;
        }
    }

    /* temporary utf16 name string */
    wnd_name = name ? (WCHAR*)sad_utf8_to_utf16(name, NULL) : NULL;

    wnd = (sad_wnd*)calloc(1, sizeof(sad_wnd));
    wnd->flags = flags;

    /* `wnd->handle` is assigned in `WM_NCCREATE` */
    CreateWindowExW(
        WS_EX_APPWINDOW,
        wc_name, wnd_name, style,
        bounds->left, bounds->top,
        bounds->right, bounds->bottom,
        NULL, NULL,
        inst, wnd
    );

    /* delete temporary name */
    free(wnd_name);

    /* catch error */
    if (!wnd->handle) {
        free(wnd);
        printf("ERROR: sad_wnd_create -> _wnd_create -> CreateWindowExW\n");
        printf("    Failed with code: %lu\n", GetLastError());
        return NULL;
    }

    {
        /* copy name */
        size_t size = strlen(name) + 1;
        wnd->name = malloc(size);
        
        if (wnd->name) memcpy(wnd->name, name, size);
    }

    if (g_sad_main_window == NULL)
        g_sad_main_window = wnd;
    return wnd;
}

static int _wnd_flags_sanitize(int flags)
{
    if (flags & SAD_WND_BORDERLESS) {
        flags &= ~SAD_WND_FULLSCREEN;
    }
    if (flags & SAD_WND_MINIMIZE) {
        flags &= ~SAD_WND_MAXIMIZE;
    }
    return flags;
}

static int _wnd_flags_to_style(int flags)
{
    if (flags & (SAD_WND_BORDERLESS | SAD_WND_FULLSCREEN)) {
        return WS_POPUP;
    }
    else if (flags & SAD_WND_UNDECORATED) {
        if (flags & SAD_WND_NORESIZE) {
            return WS_OVERLAPPED | WS_CAPTION | WS_BORDER;
        }
        else {
            return WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME;
        }
    }
    else if (flags & SAD_WND_NORESIZE) {
        return WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }
    else {
        return WS_OVERLAPPEDWINDOW;
    }
}

static void _wnd_apply_style(sad_wnd *wnd, int old_flags)
{
    int style, mask = ~_wnd_flags_to_style(old_flags);
    
    /* get new style */
    style = mask & (long)GetWindowLongPtrW(wnd->handle, GWL_STYLE);
    style |= _wnd_flags_to_style(wnd->flags);

    if (!(wnd->flags & SAD_WND_MAXIMIZE)) {
        style &= ~WS_MAXIMIZE;
    }
    
    /* Set style */
    SetWindowLongPtrW(wnd->handle, GWL_STYLE, (LONG_PTR)style);
    SetWindowPos(wnd->handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
        SWP_FRAMECHANGED);
}

static void _wnd_apply_frame(sad_wnd *wnd)
{
    DWM_MARGINS margins = {0};

    if (!hDwm) return;

    if ((wnd->backdrop != 0)) {
        margins.cxLeftWidth = -1;
        margins.cxRightWidth = -1;
        margins.cyBottomHeight = -1;
        margins.cyTopHeight = -1;
    } else if (wnd->flags & SAD_WND_UNDECORATED) {
        margins.cxLeftWidth = -1;
        margins.cxRightWidth = -1;
        margins.cyBottomHeight = -1;
        margins.cyTopHeight = -1;
    }
    else if (wnd->flags & SAD_WND_OVERFRAME) {
        RECT r = {0};
        AdjustWindowRectEx(&r, (DWORD)GetWindowLongPtrW(wnd->handle, GWL_STYLE), FALSE, 0);
        margins.cxLeftWidth = -r.left;
        margins.cxRightWidth = r.right;
        margins.cyBottomHeight = r.bottom;
        margins.cyTopHeight = -r.top;
    }

    DwmExtendFrameIntoClientArea(wnd->handle, &margins);
    SetWindowPos(
        wnd->handle, NULL,
        0, 0, 0, 0,
        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
    );
}

static void _wnd_enter_fullscreen(sad_wnd *wnd)
{
    HMONITOR hm;
    MONITORINFOEXW mi = {0};
    DEVMODEW devmode = {0};
    LONG result = -1;

    const sad_display_mode* display_mode;
    sad_display_mode target_mode = wnd->display_mode;

    /* get monitor id */
    hm = sad_monitor_from_index(wnd->monitor);

    /* get device for settings change */
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(hm, (MONITORINFO*)&mi))
        return;

    display_mode = sad_monitor_pickmode(wnd->monitor, target_mode.width, target_mode.height,
        target_mode.refresh_rate);
    
    /* change display_mode */
    devmode.dmSize = sizeof(devmode);
    devmode.dmDriverExtra = 0;
    devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
    devmode.dmPelsWidth = display_mode->width;
    devmode.dmPelsHeight = display_mode->height;
    devmode.dmDisplayFrequency = display_mode->refresh_rate;

    result = ChangeDisplaySettingsExW(mi.szDevice, &devmode, NULL, CDS_TEST, NULL);

    if (result == DISP_CHANGE_SUCCESSFUL) {
        ChangeDisplaySettingsExW(mi.szDevice, &devmode, NULL, CDS_FULLSCREEN, NULL);
    }

    /* update window bounds */
    if (!GetMonitorInfoW(hm, (MONITORINFO*)&mi)) {
        ChangeDisplaySettingsExW(mi.szDevice, NULL, NULL, 0, NULL); /* reset */
        return;
    }

    SetWindowPos(
        wnd->handle,
        HWND_TOPMOST,
        mi.rcMonitor.left,
        mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        SWP_NOACTIVATE
    );
}

static void _wnd_leave_fullscreen(sad_wnd *wnd)
{
    RECT r = {0};
    sad_wnd_config cfg = {0};
    int mx, my, mw, mh;

    /* reset display mode */
    sad_monitor_reset_mode(wnd->monitor);
    sad_monitor_bounds(wnd->monitor, &mx, &my, &mw, &mh);

    /* get current window size */
    GetClientRect(wnd->handle, &r);

    /* clamp window size */
    r.right = r.right < mw ? r.right : mw;
    r.bottom = r.bottom < mh ? r.bottom : mh;

    /* center window size */
    r.left = mx + (mw - r.right) / 2;
    r.top = my + (mh - r.bottom) / 2;

    SetWindowPos(
        wnd->handle,
        HWND_NOTOPMOST,
        r.left, r.top, r.right, r.bottom,
        SWP_NOACTIVATE
    );
}

/*
* Monitor Helper Implementation
* ----------------------------------------------------------------------------------------------- */

typedef struct {
    int index;
    int count;
    HMONITOR primary;
    HMONITOR handle;
} enum_monitor_data;

static BOOL enum_monitor_proc(HMONITOR mon, HDC hdc, LPRECT rect, LPARAM lp)
{
    enum_monitor_data* data = (enum_monitor_data*)lp;
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if (GetMonitorInfo(mon, &mi)) {
        if (mi.dwFlags & MONITORINFOF_PRIMARY) {
            data->primary = mon;
            return TRUE;
        }
    }

    if (data->index == data->count) {
        data->handle = mon;
        return FALSE;
    }

    data->count++;
    return TRUE;
}

static HMONITOR sad_monitor_from_index(int monitor)
{
    if (monitor <= 0) {
        POINT p = {0};
        return MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
    }

    if (monitor > 0) {
        enum_monitor_data data = { monitor, 1, NULL, NULL };
        EnumDisplayMonitors(NULL, NULL, &enum_monitor_proc, (LPARAM)&data);
        return data.handle ? data.handle : data.primary;
    }
    
    return NULL;
}

static void sad_monitor_reset_mode(int monitor)
{
    HMONITOR hm;
    MONITORINFOEXW mi = {0};

    /* get monitor id */
    hm = sad_monitor_from_index(monitor);

    /* get device for settings change */
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(hm, (MONITORINFO*)&mi))
        return;
    
    /* reset to registry settings */
    ChangeDisplaySettingsExW(mi.szDevice, NULL, NULL, 0, NULL);
}

/* Display Mode Cache ------------------------------------------------------- */

static sad_display_mode_ctx_init(display_mode_ctx* ctx)
{
    sad_fmap_init(&ctx->modes, sizeof(sad_display_mode), 32, (sad_cmpfunc)&sad_display_mode_cmp);
    sad_fmap_init(&ctx->resolutions, sizeof(sad_display_res), 16, (sad_cmpfunc)&sad_display_res_cmp);
    sad_fmap_init(&ctx->refresh_rates, sizeof(int), 8, &sad_refresh_rate_cmp);
    ctx->width = -1;
    ctx->height = -1;
    ctx->initialized = 1;
}

static sad_display_mode_ctx_destroy(display_mode_ctx* ctx)
{
    if (ctx->initialized) {
        ctx->initialized = 0;
        sad_fmap_destroy(&ctx->modes);
        sad_fmap_destroy(&ctx->resolutions);
        sad_fmap_destroy(&ctx->refresh_rates);
    }
}

static display_mode_ctx* sad_display_mode_getctx(int monitor)
{
    static display_mode_ctx ctx = {0};

    HMONITOR hm = sad_monitor_from_index(monitor);
    MONITORINFOEXW mi = { sizeof(MONITORINFOEXW) };
    DEVMODEW dm = {0};
    int color_depth;
    int i;
    
    /* get monitor device */
    if (!GetMonitorInfoW(hm, (MONITORINFO*)&mi)) {
        sad_display_mode_ctx_destroy(&ctx);
        return &ctx;
    }

    /* device cache is initialized */
    if (lstrcmpW(mi.szDevice, ctx.device) == 0)
        return &ctx;
    
    /* re/create device cache */
    sad_display_mode_ctx_destroy(&ctx);
    sad_display_mode_ctx_init(&ctx);
    
    lstrcpynW(ctx.device, mi.szDevice, 32);
    dm.dmSize = sizeof(dm);

    for (i = 0; EnumDisplaySettingsExW(ctx.device, i, &dm, 0) && i < 0x800; i++) {
        sad_display_mode mode = {
            (int)dm.dmPelsWidth,
            (int)dm.dmPelsHeight,
            (int)dm.dmDisplayFrequency,
            (int)dm.dmBitsPerPel
        };
        sad_display_res res = {
            mode.width, mode.height
        };

        /* get current color depth */
        if (i == 0) {
            EnumDisplaySettingsExW(ctx.device, ENUM_CURRENT_SETTINGS, &dm, 0);
            color_depth = (int)dm.dmBitsPerPel;
        }

        /* filter by current color depth */
        if (mode.color_depth == color_depth) {
            sad_fmap_push(&ctx.modes, &mode);
            sad_fmap_push(&ctx.resolutions, &res);
        }
    }

    return &ctx;
}

static sad_int sad_refresh_rate_cmp(const int* a, const int* b)
{
    return (sad_int)(*a - *b);
}

/*
* Event Loop Helper Implementation
* ----------------------------------------------------------------------------------------------- */

/* To achieve window frame manipulation, three functions are used:
*  - _wnd_apply_frame()
*  - _wnd_wmnccalcsize()
*  - _wnd_wmnchittest()
*/

static int _wnd_wmnccalcsize(sad_wnd* wnd, WPARAM wp, LPARAM lp, LRESULT* result)
{
    RECT frame = {0};
    RECT* rect = 0;

    AdjustWindowRectEx(&frame, (LONG)GetWindowLongPtrA(wnd->handle, GWL_STYLE), FALSE, 0);
    rect = wp ? ((NCCALCSIZE_PARAMS*)lp)->rgrc : (RECT*)lp;

    if (wnd->flags & SAD_WND_UNDECORATED) {
        *result = 0;
        return 1;
    }
    else if (wnd->flags & SAD_WND_OVERFRAME) {
        // rect->left   -= frame.left;
        // rect->right  -= frame.right;
        // rect->bottom -= frame.bottom;
        // rect->top -= frame.top;
        
        if (IsZoomed(wnd->handle)) {
            // rect->top += frame.bottom; // Windows 10 or lower only?
        }
        
        *result = 0;
        return 1;
    }

    return 0;
}

static int _wnd_wmnchittest(sad_wnd* wnd, WPARAM wp, LPARAM lp, LRESULT* result)
{
    RECT frame = {0};
    RECT client = {0};
    POINT point = {LOWORD(lp), HIWORD(lp)};
    int hit = 0;

    ScreenToClient(wnd->handle, &point);
    GetClientRect(wnd->handle, &client);
    AdjustWindowRectEx(&frame, (LONG)GetWindowLongPtrA(wnd->handle, GWL_STYLE), FALSE, 0);

    /* User test */
    if (wnd->on_hittest) {
        *result = wnd->on_hittest(wnd, point.x, point.y);
        if (*result != SAD_WNDHIT_NONE)
            return 1;
    }
    
    if (!(wnd->flags & SAD_WND_NORESIZE) && (wnd->flags & (SAD_WND_UNDECORATED | SAD_WND_OVERFRAME)))
    {
        hit |= (int)(point.y < frame.bottom)                    << 0; /* TOP */
        hit |= (int)(point.y >= (client.bottom - frame.bottom)) << 1; /* BOTTOM */
        hit |= (int)(point.x < frame.right)                     << 2; /* LEFT */
        hit |= (int)(point.x >= (client.right - frame.right))   << 3; /* RIGHT */
    }

    if (hit) {
        static const char result_map[16] = {0, HTTOP, HTBOTTOM, 0, HTLEFT, HTTOPLEFT, HTBOTTOMLEFT,
            0, HTRIGHT, HTTOPRIGHT, HTBOTTOMRIGHT};
        *result = result_map[hit];
        return 1;
    }

    if ((wnd->flags & SAD_WND_OVERFRAME) && (point.y < -frame.top)) {
        *result = HTCAPTION;
        return 1;
    }
    
    return 0;
}

static void _wnd_wmactivate(sad_wnd* wnd, int active, int minimized)
{
    if (active && !minimized && (wnd->flags & SAD_WND_FULLSCREEN)) {
        _wnd_enter_fullscreen(wnd);
    }
    if (!active && (wnd->flags & SAD_WND_FULLSCREEN)) {
        _wnd_leave_fullscreen(wnd);
    }

    _wnd_apply_frame(wnd);
}

static void _wnd_wmsize(sad_wnd* wnd, WPARAM wp, LPARAM lp)
{
    int width = (int)LOWORD(lp);
    int height = (int)HIWORD(lp);
    
    switch (wp)
    {
    case SIZE_RESTORED:
        wnd->flags &= ~(SAD_WND_MINIMIZE | SAD_WND_MAXIMIZE);
        break;
    
    case SIZE_MINIMIZED:
        wnd->flags = (wnd->flags & ~SAD_WND_MAXIMIZE) | SAD_WND_MINIMIZE;
        break;
    
    case SIZE_MAXIMIZED:
        wnd->flags = (wnd->flags & ~SAD_WND_MINIMIZE) | SAD_WND_MAXIMIZE;
        break;
    
    default:
        break;
    }
    
    if (wnd->on_size)
        wnd->on_size(wnd, width, height);
}

static void _wnd_wminput(sad_wnd* wnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT* result)
{

}

/* Event Loop --------------------------------------------------------------- */

static LRESULT _wnd_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    sad_wnd* wnd = (sad_wnd*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    LRESULT result;

    if (DwmDefWindowProc && DwmDefWindowProc(hwnd, msg, wp, lp, &result))
        return result;

    switch (msg)
    {
    default:
        break;
    
    /* Non-Client Messages ------------------------------ */
    case WM_NCCREATE:
        wnd = (sad_wnd*)((LPCREATESTRUCTW)lp)->lpCreateParams;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
        wnd->handle = hwnd;
        break;
    case WM_NCCALCSIZE:
        if (_wnd_wmnccalcsize(wnd, wp, lp, &result))
            return result;
        break;
    case WM_NCHITTEST:
        if (_wnd_wmnchittest(wnd, wp, lp, &result))
            return result;
        break;
    case WM_NCPAINT:
        break;
    case WM_ERASEBKGND:
        break;
    
    /* Client Messages ---------------------------------- */
    case WM_ACTIVATE: {
        int activated = (int)LOWORD(wp);
        int minimized = (int)HIWORD(wp);

        printf("[INFO]: WM_ACTIVATE (active=%i, minimized=%i)\n", activated, minimized);
        _wnd_wmactivate(wnd, activated, minimized);
        return 0;
    }

    case WM_SIZE: {
        printf("[INFO]: WM_SIZE\n");
        _wnd_wmsize(wnd, wp, lp);
        return 0;
    }
    
    case WM_PAINT: {
        printf("[INFO]: WM_PAINT\n");
        ValidateRect(wnd->handle, NULL);
        return 0;
    }
    
    case WM_DESTROY: {
        if (wnd == g_sad_main_window) {
            _wnd_free_ext();
            PostQuitMessage(0);
        }
        return 0;
    }
    
    /* Theme Messages ----------------------------------- */
    case WM_DWMCOMPOSITIONCHANGED:
        break;
    case WM_DWMCOLORIZATIONCOLORCHANGED:
        break;
    case WM_DISPLAYCHANGE:
        printf("[INFO]: WM_DISPLAYCHANGE\n");
        break;
    case WM_DPICHANGED:
        break;
    }

    return DefWindowProcW(hwnd, msg, wp, lp);
}

#endif /* SAD_OS_WINDOWS */
