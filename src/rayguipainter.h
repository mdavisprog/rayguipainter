#ifndef RAYGUIPAINTER_H
#define RAYGUIPAINTER_H

#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24

#include "raygui.h"

typedef struct GuiPainterDropdownBoxOptions {
    int active;
    bool edit;
} GuiPainterDropdownBoxOptions;

#if defined(__cplusplus)
extern "C" {
#endif

void GuiPainterSetCursorPos(Vector2 pos);

bool GuiPainterWindowBox(Vector2 size, const char* title);
void GuiPainterLine(const char* text);
void GuiPainterLabel(const char* text);
bool GuiPainterButton(const char* text);
bool GuiPainterLabelButton(const char* text);
bool GuiPainterToggle(const char* text, bool* active);
bool GuiPainterDropdownBox(const char* text, GuiPainterDropdownBoxOptions* options);
bool GuiPainterTextBox(char* text, int textSize, bool* editMode);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorSize = { 0.0f, 0.0f };
static Vector2 guiPainterControlSpacing = { 4.0f, 4.0f };
static Vector2 guiPainterButtonPadding = { 6.0f, 4.0f };
static float guiPainterTextBoxWidth = 100.0f;

static Vector2 GuiPainterTextSize(const char* text)
{
    return MeasureTextEx(GuiGetFont(), text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING));
}

static Vector2 GuiPainterLargestTextSize(const char* text)
{
    Vector2 result = { 0 };

    char buffer[1024] = { 0 };
    const char* ptr = text;
    int index = 0;
    while (*ptr != 0)
    {
        if (*ptr == ';')
        {
            buffer[index] = 0;
            Vector2 size = GuiPainterTextSize(buffer);
            result.x = size.x > result.x ? size.x : result.x;
            result.y = size.y > result.y ? size.y : result.y;
            memset(buffer, 0, sizeof(buffer));
            index = 0;
        }
        else
        {
            buffer[index++] = *ptr;
        }

        ptr++;
    }

    if (index > 0)
    {
        buffer[index] = 0;
        Vector2 size = GuiPainterTextSize(buffer);
        result.x = size.x > result.x ? size.x : result.x;
        result.y = size.y > result.y ? size.y : result.y;
    }

    return result;
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

void GuiPainterLine(const char* text)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        guiPainterCursorSize.x - guiPainterControlSpacing.x * 2.0f,
        textSize.y == 0.0f ? (float)GuiGetStyle(DEFAULT, TEXT_SIZE) : textSize.y
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    GuiLine(bounds, text);
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

bool GuiPainterLabelButton(const char* text)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        textSize.x + guiPainterButtonPadding.x * 2.0f,
        textSize.y + guiPainterButtonPadding.y * 2.0f
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    return GuiLabelButton(bounds, text);
}

bool GuiPainterToggle(const char* text, bool* active)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        textSize.x + guiPainterButtonPadding.x * 2.0f,
        textSize.y + guiPainterButtonPadding.y * 2.0f
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    *active = GuiToggle(bounds, text, *active);
    return *active;
}

bool GuiPainterDropdownBox(const char* text, GuiPainterDropdownBoxOptions* options)
{
    const Vector2 maxSize = GuiPainterLargestTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        maxSize.x + guiPainterButtonPadding.x + (float)GuiGetStyle(DROPDOWNBOX, ARROW_PADDING) + RAYGUI_ICON_SIZE,
        maxSize.y + guiPainterButtonPadding.y * 2.0f
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    if (GuiDropdownBox(bounds, text, &options->active, options->edit))
    {
        options->edit = !options->edit;
        return true;
    }
    return false;
}

bool GuiPainterTextBox(char* text, int textSize, bool* editMode)
{
    const Vector2 boxSize = GuiPainterTextSize(" ");
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        guiPainterTextBoxWidth,
        boxSize.y + guiPainterButtonPadding.y * 2.0f
    };
    guiPainterCursorPos.y += bounds.height + guiPainterControlSpacing.y;
    if (GuiTextBox(bounds, text, textSize, *editMode))
    {
        *editMode = !(*editMode);
        return true;
    }
    return false;
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
