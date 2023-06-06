#ifndef RAYGUIPAINTER_H
#define RAYGUIPAINTER_H

#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24

#include "raygui.h"

#if defined(__cplusplus)
extern "C" {
#endif

void GuiPainterSetCursorPos(Vector2 pos);

bool GuiPainterWindowBox(Vector2 size, const char* title);
void GuiPainterLabel(const char* text);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorSize = { 0.0f, 0.0f };
static Vector2 guiPainterControlSpacing = { 4.0f, 4.0f };

static Vector2 GuiPainterTextSize(const char* text)
{
    return MeasureTextEx(GuiGetFont(), text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
}

void GuiPainterSetCursorPos(Vector2 pos)
{
    guiPainterCursorPos = pos;
}

bool GuiPainterWindowBox(Vector2 size, const char* title)
{
    const Rectangle bounds = { guiPainterCursorPos.x, guiPainterCursorPos.y, size.x, size.y };
    bool result = GuiWindowBox(bounds, title);
    guiPainterCursorPos.y += RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + guiPainterControlSpacing.y;
    guiPainterCursorSize = size;
    return result;
}

void GuiPainterLabel(const char* text)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = { guiPainterCursorPos.x + guiPainterControlSpacing.x, guiPainterCursorPos.y, textSize.x + 6.0f, textSize.y };
    GuiLabel(bounds, text);
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
