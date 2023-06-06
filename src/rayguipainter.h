#ifndef RAYGUIPAINTER_H
#define RAYGUIPAINTER_H

#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24

#include "raygui.h"

#if defined(__cplusplus)
extern "C" {
#endif

void GuiPainterSetCursorPos(Vector2 pos);

bool GuiPainterWindowBox(Vector2 size, const char* title);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorSize = { 0.0f, 0.0f };

void GuiPainterSetCursorPos(Vector2 pos)
{
    guiPainterCursorPos = pos;
}

bool GuiPainterWindowBox(Vector2 size, const char* title)
{
    const Rectangle bounds = { guiPainterCursorPos.x, guiPainterCursorPos.y, size.x, size.y };
    bool result = GuiWindowBox(bounds, title);
    guiPainterCursorPos.y += RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;
    guiPainterCursorSize = size;
    return result;
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
