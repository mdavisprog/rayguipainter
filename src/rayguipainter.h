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
bool GuiPainterButton(const char* text);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorSize = { 0.0f, 0.0f };
static Vector2 guiPainterControlSpacing = { 4.0f, 4.0f };
static Vector2 guiPainterButtonPadding = { 6.0f, 4.0f };

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
    guiPainterCursorPos.y += RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + guiPainterControlSpacing.y;
    guiPainterCursorSize = size;
    return GuiWindowBox(bounds, title);
}

void GuiPainterLabel(const char* text)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = { guiPainterCursorPos.x + guiPainterControlSpacing.x, guiPainterCursorPos.y, textSize.x + 6.0f, textSize.y };
    GuiLabel(bounds, text);
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
}

bool GuiPainterButton(const char* text)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        textSize.x + guiPainterButtonPadding.x * 2.0f,
        textSize.y + guiPainterButtonPadding.y * 2.0f
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    return GuiButton(bounds, text);
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
