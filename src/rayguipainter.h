#ifndef RAYGUIPAINTER_H
#define RAYGUIPAINTER_H

#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24

#include "raygui.h"

typedef struct GuiPainterDropdownBoxOptions {
    int active;
    bool edit;
} GuiPainterDropdownBoxOptions;

typedef struct GuiPainterValueBoxOptions {
    int value;
    int minValue;
    int maxValue;
    bool edit;
} GuiPainterValueBoxOptions;

#if defined(__cplusplus)
extern "C" {
#endif

void GuiPainterSetCursorPos(Vector2 pos);
void GuiPainterSameLine();

bool GuiPainterWindowBox(Vector2 size, const char* title);
void GuiPainterLine(const char* text);
void GuiPainterLabel(const char* text);
bool GuiPainterButton(const char* text);
bool GuiPainterLabelButton(const char* text);
bool GuiPainterToggle(const char* text, bool* active);
int GuiPainterToggleGroup(const char* text, int* active);
bool GuiPainterCheckBox(const char* text, bool* checked);
bool GuiPainterDropdownBox(const char* text, GuiPainterDropdownBoxOptions* options);
bool GuiPainterValueBox(const char* text, GuiPainterValueBoxOptions* options);
bool GuiPainterTextBox(char* text, int textSize, bool* editMode);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorPrevPos = { 0.0f, 0.0f };
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

static void GuiPainterAdvanceCursorLine(Rectangle controlBounds)
{
    guiPainterCursorPrevPos.x = controlBounds.x + controlBounds.width;
    guiPainterCursorPrevPos.y = guiPainterCursorPos.y;
    guiPainterCursorPos.x = 0.0f;
    guiPainterCursorPos.y += controlBounds.height + guiPainterControlSpacing.y;
}

void GuiPainterSetCursorPos(Vector2 pos)
{
    guiPainterCursorPos = pos;
}

void GuiPainterSameLine()
{
    guiPainterCursorPos = guiPainterCursorPrevPos;
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
    GuiPainterAdvanceCursorLine(bounds);
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
    GuiPainterAdvanceCursorLine(bounds);
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
    GuiPainterAdvanceCursorLine(bounds);
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
    GuiPainterAdvanceCursorLine(bounds);
#if RAYGUI_VERSION_MAJOR < 4
    *active = GuiToggle(bounds, text, *active);
#else
    GuiToggle(bounds, text, active);
#endif
    return *active;
}

int GuiPainterToggleGroup(const char* text, int* active)
{
    const Vector2 maxSize = GuiPainterLargestTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        maxSize.x + guiPainterButtonPadding.x * 2.0f,
        maxSize.y + guiPainterButtonPadding.y * 2.0f
    };
    GuiPainterAdvanceCursorLine(bounds);
#if RAYGUI_VERSION_MAJOR < 4
    *active = GuiToggleGroup(bounds, text, *active);
#else
    GuiToggleGroup(bounds, text, active);
#endif
    return *active;
}

bool GuiPainterCheckBox(const char* text, bool* checked)
{
    const float size = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    const Vector2 textSize = GuiPainterTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        size + guiPainterButtonPadding.x * 2.0f,
        size + guiPainterButtonPadding.y * 2.0f
    };
    const Rectangle advanceBounds = {
        bounds.x,
        bounds.y,
        bounds.width + textSize.x + GuiGetStyle(CHECKBOX, TEXT_PADDING),
        bounds.height
    };
    GuiPainterAdvanceCursorLine(advanceBounds);
#if RAYGUI_VERSION_MAJOR < 4
    *checked = GuiCheckBox(bounds, text, *checked);
    return *checked;
#else
    return GuiCheckBox(bounds, text, checked);
#endif
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
    GuiPainterAdvanceCursorLine(bounds);
    if (GuiDropdownBox(bounds, text, &options->active, options->edit))
    {
        options->edit = !options->edit;
        return true;
    }
    return false;
}

bool GuiPainterValueBox(const char* text, GuiPainterValueBoxOptions* options)
{
    const Vector2 textSize = GuiPainterTextSize(text);
    const Vector2 maxValueSize = GuiPainterTextSize(TextFormat("%9d", options->maxValue));
    const bool leftAligned = GuiGetStyle(VALUEBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT;
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x + (leftAligned ? textSize.x + GuiGetStyle(VALUEBOX, TEXT_PADDING) : 0.0f),
        guiPainterCursorPos.y,
        maxValueSize.x + guiPainterButtonPadding.x * 2.0f,
        maxValueSize.y + guiPainterButtonPadding.y * 2.0f
    };
    const Rectangle advanceBounds = {
        bounds.x,
        bounds.y,
        bounds.width + (leftAligned == false ? textSize.x + GuiGetStyle(VALUEBOX, TEXT_PADDING) : 0.0f),
        bounds.height
    };
    GuiPainterAdvanceCursorLine(advanceBounds);
    if (GuiValueBox(bounds, text, &options->value, options->minValue, options->maxValue, options->edit))
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
    GuiPainterAdvanceCursorLine(bounds);
    if (GuiTextBox(bounds, text, textSize, *editMode))
    {
        *editMode = !(*editMode);
        return true;
    }
    return false;
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
