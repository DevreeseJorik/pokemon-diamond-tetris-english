/**
 * @file window.h
 * @brief Window drawing related functions
 *
 */

#if !defined(_WINDOW_H)
#define _WINDOW_H

#include "acehelper.h"
#include "string.h"

// Type, Constant, and Macro Definitions
typedef struct _BGSetting BGSetting;
typedef struct
{
    BGSetting *ini;
    u8 dummy[0xc];
} Window;

#define PALLET_13 13
#define COLOR_WHITE 15
#define BG0 0
#define BG1 1
#define BG2 2
#define BG3 3
#define HEAP_FIELD 11
#define HEAP_UNUSED 2

#define fsys (*(void **)pp_fsys)
#define fsysBgl (*(BGSetting **)((u32)fsys + 8))

#define _fillWindowArea ((void (*)(Window*, u8, u16, u16, u16, u16))0x020196f5)
#define _newWindow ((Window * (*)(u32, u32))0x02018ff5) // unsure 201901c
#define _deleteWindow ((void (*)(Window *, u8))0x02019179)
#define _registerWindow ((void (*)(BGSetting *, Window *, u8, u8, u8, u8, u8, u8, u16))0x02019065)
#define _fillWindow ((void (*)(Window *, u8))0x02019621)
#define _printStr ((void (*)(Window *, u32, String *, u32, u32, u32, void *))0x0201bd85)
#define _hideWindow ((void (*)(Window *))0x02019570) // unsure
#define _clearWindowArea ((void (*)(Window *))0x0201928d) // unsure
#define _showWindow ((void (*)(Window *))0x020191d1)

// Function Prototypes
static inline Window *newWindow(u32 x, u32 y, u32 width, u32 height);
static inline void printStr(Window *win, String *str);
static inline void fillWindow(Window *win, u8 color);
static inline void hideWindow(Window *win);
static inline void clearWindowArea(Window *win);
static inline void showWindow(Window *win);
static inline void fillWindowArea(Window *win, u8 color, u16 x, u16 y, u16 width, u16 height);

/**
 * @brief Create a new window
 *
 * @param x X coordinate of the top-left corner of the window
 * @param y Y coordinate of the top-left corner of the window
 * @param width Width of the window (in characters)
 * @param height Height of the window (in characters)
 * @return Window* Pointer to the created window
 * @details Utilizes settings from fsys, so it may not be usable on PC screens.
 */
inline static Window *newWindow(u32 x, u32 y, u32 width, u32 height)
{
    Window *win = _newWindow(HEAP_UNUSED, 1);
    _registerWindow(fsysBgl, win, BG3, x, y, width, height, PALLET_13, 1);
    _fillWindow(win, COLOR_WHITE);
    return win;
}

/**
 * @brief Delete a window
 *
 * @param win Pointer to the window to be deleted
 */
inline static void deleteWindow(Window *win)
{
    _deleteWindow(win, 1);
}

/**
 * @brief Draw a string in the window
 *
 * @param win Pointer to the window
 * @param str Pointer to the string to be drawn
 */
inline static void printStr(Window *win, String *str)
{
    _printStr(win, 0, str, 0, 0, 0xff, NULL);
}

/**
 * @brief Fill the window with a single color
 *
 * @param win Pointer to the window
 * @param color Color index, specified within the palette (0~255)
 */
inline static void fillWindow(Window *win, u8 color)
{
    _fillWindow(win, color);
}

/**
 * @brief Hide the window
 *
 * @param win Pointer to the window to be hidden
 */
inline static void hideWindow(Window *win)
{
    _hideWindow(win);
}

/**
 * @brief Clear the drawable area of the window
 *
 * @param win Pointer to the window to be cleared
 */
inline static void clearWindowArea(Window *win)
{
    _clearWindowArea(win);
}

/**
 * @brief Show the window
 *
 * @param win Pointer to the window to be shown
 */
inline static void showWindow(Window *win)
{
    _showWindow(win);
}

/**
 * @brief Fill a specific area of the window with a color
 *
 * @param win Pointer to the window
 * @param color Color index to fill with
 * @param x Start X coordinate
 * @param y Start Y coordinate
 * @param width Width of the area to fill
 * @param height Height of the area to fill
 */
inline static void fillWindowArea(Window *win, u8 col, u16 px, u16 py, u16 sx, u16 sy )
{
    _fillWindowArea(win, col, px, py, sx, sy);
}

#endif // _WINDOW_H
