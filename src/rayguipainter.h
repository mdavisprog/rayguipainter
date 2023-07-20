#ifndef RAYGUIPAINTER_H
#define RAYGUIPAINTER_H

#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24

#if defined(RAYGUI_IMPLEMENTATION)
    #define CHECK_BOUNDS_ID(src, dst) ((src.x == dst.x) && (src.y == dst.y))
#endif

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

typedef struct GuiPainterSliderOptions {
    float value;
    float minValue;
    float maxValue;
} GuiPainterSliderOptions;

typedef struct GuiPainterListViewOptions {
    int scrollIndex;
    int active;
    int visibleListItems;
} GuiPainterListViewOptions;

#if defined(__cplusplus)
extern "C" {
#endif

void GuiPainterSetCursorPos(Vector2 pos);
Vector2 GuiPainterGetCursorPos();
void GuiPainterSetControlSpacing(Vector2 spacing);
Vector2 GuiPainterGetControlSpacing();
void GuiPainterSetCursorSize(Vector2 size);
Vector2 GuiPainterGetCursorSize();
void GuiPainterSameLine();
void GuiPainterNextLine();
void GuiPainterFillWidth();

bool GuiPainterWindowBox(Vector2 size, const char* title);
void GuiPainterPanel(Vector2 size, const char* title);
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
float GuiPainterSlider(const char* textLeft, const char* textRight, GuiPainterSliderOptions* options);
int GuiPainterListView(const char* text, GuiPainterListViewOptions* options);
bool GuiPainterImage(Texture2D texture, Color tint);
bool GuiPainterImageRec(Texture2D texture, Rectangle source, Color tint);

#if defined(__cplusplus)
}
#endif

#endif // RAYGUIPAINTER_H

#if defined(RAYGUIPAINTER_IMPLEMENTATION)

static Vector2 guiPainterCursorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorAnchorPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorPrevPos = { 0.0f, 0.0f };
static Vector2 guiPainterCursorSize = { 0.0f, 0.0f };
static Vector2 guiPainterControlSpacing = { 4.0f, 4.0f };
static Vector2 guiPainterButtonPadding = { 6.0f, 4.0f };
static float guiPainterTextBoxWidth = 100.0f;
static float guiPainterSliderWidth = 100.0f;
static bool guiPainterFillWidth = false;

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

static int GuiPainterItemCount(const char* text)
{
    int result = 1;

    const char* ptr = text;
    while (*ptr != 0)
    {
        if (*ptr == ';')
        {
            result++;
        }

        ptr++;
    }

    return result;
}

static void GuiPainterAdvanceCursorLine(Rectangle controlBounds)
{
    guiPainterCursorPrevPos.x = controlBounds.x + controlBounds.width;
    guiPainterCursorPrevPos.y = guiPainterCursorPos.y;
    guiPainterCursorPos.x = guiPainterCursorAnchorPos.x;
    guiPainterCursorPos.y += controlBounds.height + guiPainterControlSpacing.y;
    guiPainterFillWidth = false;
}

static float GuiPainterSuggestedWidth(float width, float padding)
{
    float result = width;
    if (guiPainterFillWidth)
    {
        result = guiPainterCursorSize.x - (guiPainterCursorPos.x - guiPainterCursorAnchorPos.x) - (guiPainterControlSpacing.x * 2.0f) - padding;
    }
    return result;
}

void GuiPainterSetCursorPos(Vector2 pos)
{
    guiPainterCursorPos = pos;
    guiPainterCursorAnchorPos = pos;
}

Vector2 GuiPainterGetCursorPos()
{
    return guiPainterCursorPos;
}

void GuiPainterSetControlSpacing(Vector2 spacing)
{
    guiPainterControlSpacing = spacing;
}

Vector2 GuiPainterGetControlSpacing()
{
    return guiPainterControlSpacing;
}

void GuiPainterSetCursorSize(Vector2 size)
{
    guiPainterCursorSize = size;
}

Vector2 GuiPainterGetCursorSize()
{
    return guiPainterCursorSize;
}

void GuiPainterSameLine()
{
    guiPainterCursorPos = guiPainterCursorPrevPos;
}

void GuiPainterNextLine()
{
    const float height = GuiPainterTextSize(" ").y + guiPainterButtonPadding.y * 2.0f + guiPainterControlSpacing.y;
    guiPainterCursorPos.y += height;
}

void GuiPainterFillWidth()
{
    guiPainterFillWidth = true;
}

bool GuiPainterWindowBox(Vector2 size, const char* title)
{
    const Rectangle bounds = { guiPainterCursorPos.x, guiPainterCursorPos.y, size.x, size.y };
    guiPainterCursorPos.y += RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + guiPainterControlSpacing.y;
    guiPainterCursorSize = size;
    return GuiWindowBox(bounds, title);
}

void GuiPainterPanel(Vector2 size, const char* title)
{
    const Rectangle bounds = { guiPainterCursorPos.x, guiPainterCursorPos.y, size.x, size.y };
    guiPainterCursorPos.y += RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT + guiPainterControlSpacing.y;
    guiPainterCursorSize = size;
    GuiPanel(bounds, title);
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
        GuiPainterSuggestedWidth(textSize.x + guiPainterButtonPadding.x * 2.0f, 0.0f),
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
        GuiPainterSuggestedWidth(textSize.x + guiPainterButtonPadding.x * 2.0f, 0.0f),
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
        GuiPainterSuggestedWidth(textSize.x + guiPainterButtonPadding.x * 2.0f, 0.0f),
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
    const int itemCount = GuiPainterItemCount(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        guiPainterFillWidth ? (GuiPainterSuggestedWidth(0.0f, guiPainterControlSpacing.x) / (float)itemCount) : maxSize.x + guiPainterButtonPadding.x * 2.0f,
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
        GuiPainterSuggestedWidth(maxSize.x + guiPainterButtonPadding.x + (float)GuiGetStyle(DROPDOWNBOX, ARROW_PADDING) + RAYGUI_ICON_SIZE, 0.0f),
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
        GuiPainterSuggestedWidth(maxValueSize.x + guiPainterButtonPadding.x * 2.0f, textSize.x),
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
        GuiPainterSuggestedWidth(guiPainterTextBoxWidth, 0.0f),
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

float GuiPainterSlider(const char* textLeft, const char* textRight, GuiPainterSliderOptions* options)
{
    const float textPadding = (float)GuiGetStyle(SLIDER, TEXT_PADDING);
    const Vector2 textLeftSize = GuiPainterTextSize(textLeft);
    const Vector2 textRightSize = GuiPainterTextSize(textRight);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x + textLeftSize.x + textPadding,
        guiPainterCursorPos.y,
        GuiPainterSuggestedWidth(guiPainterSliderWidth, textLeftSize.x + textRightSize.x + textPadding + guiPainterControlSpacing.x),
        (float)GuiGetStyle(DEFAULT, TEXT_SIZE) + guiPainterButtonPadding.y * 2.0f
    };
    const Rectangle advanceBounds = {
        bounds.x,
        bounds.y,
        bounds.width + textRightSize.x + textPadding,
        bounds.height
    };
    GuiPainterAdvanceCursorLine(advanceBounds);
#if RAYGUI_VERSION_MAJOR < 4
    options->value = GuiSlider(bounds, textLeft, textRight, options->value, options->minValue, options->maxValue);
#else
    GuiSlider(bounds, textLeft, textRight, &options->value, options->minValue, options->maxValue);
#endif
    return options->value;
}

int GuiPainterListView(const char* text, GuiPainterListViewOptions* options)
{
    const int numVisible = options->visibleListItems > 0 ? options->visibleListItems : 5;
    const float scrollBarWidth = (float)GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);
    const Vector2 maxSize = GuiPainterLargestTextSize(text);
    const Rectangle bounds = {
        guiPainterCursorPos.x + guiPainterControlSpacing.x,
        guiPainterCursorPos.y,
        GuiPainterSuggestedWidth(maxSize.x + guiPainterButtonPadding.x * 2.0f + scrollBarWidth, 0.0f),
        ((float)GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT) + (float)GuiGetStyle(LISTVIEW, LIST_ITEMS_SPACING)) * (float)numVisible
    };
    GuiPainterAdvanceCursorLine(bounds);
#if RAYGUI_VERSION_MAJOR < 4
    options->active = GuiListView(bounds, text, &options->scrollIndex, options->active);
#else
    GuiListView(bounds, text, &options->scrollIndex, &options->active);
#endif
    return options->active;
}

bool GuiPainterImage(Texture2D texture, Color tint)
{
    const Vector2 position = { guiPainterCursorPos.x + guiPainterControlSpacing.x, guiPainterCursorPos.y };
    const Rectangle bounds = {
        position.x,
        guiPainterCursorPos.y,
        (float)texture.width,
        (float)texture.height
    };
    GuiPainterAdvanceCursorLine(bounds);
    bool result = false;
    if (GuiGetState() != STATE_DISABLED && !GuiIsLocked())
    {
        const Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = true;
        }
    }
    DrawTextureV(texture, position, tint);
    return result;
}

bool GuiPainterImageRec(Texture2D texture, Rectangle source, Color tint)
{
    const Vector2 position = { guiPainterCursorPos.x + guiPainterControlSpacing.x, guiPainterCursorPos.y };
    const Rectangle bounds = {
        position.x,
        position.y,
        source.width,
        source.height
    };
    GuiPainterAdvanceCursorLine(bounds);
    bool result = false;
    if (GuiGetState() != STATE_DISABLED && !GuiIsLocked())
    {
        const Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) result = true;
        }
    }
    DrawTextureRec(texture, source, position, tint);
    return result;
}

#endif // RAYGUIPAINTER_IMPLEMENTATION
