#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUIPAINTER_IMPLEMENTATION
#include "rayguipainter.h"

int main()
{
    const int WindowWidth = 960;
    const int WindowHeight = 540;

    InitWindow(WindowWidth, WindowHeight, "Showcase");
    SetTargetFPS(60);

    const Vector2 CursorPos = { 0.0f, 0.0f };
    const Vector2 WindowBoxSize = { (float)WindowWidth, (float)WindowHeight };

    bool Toggle = false;
    int ToggleGroup = 0;
    char TextBox[255] =  { 0 };
    bool TextBoxEdit = false;
    GuiPainterDropdownBoxOptions DropdownBoxOptions = { 0 };
    bool Checked = false;
    GuiPainterValueBoxOptions ValueOptions = { 50, 0, 100, false };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        GuiPainterSetCursorPos(CursorPos);
        GuiPainterWindowBox(WindowBoxSize, "Controls");

        GuiPainterLabel("Label");
        GuiPainterButton("Button");
        GuiPainterSameLine();
        GuiPainterButton("Button 2");
        GuiPainterLabelButton("Label Button");
        Toggle = GuiPainterToggle("Toggle", &Toggle);
        GuiPainterToggleGroup("One;Two;Three", &ToggleGroup);
        GuiPainterTextBox(TextBox, sizeof(TextBox), &TextBoxEdit);
        GuiPainterDropdownBox("Red;Green;Blue", &DropdownBoxOptions);
        GuiPainterCheckBox("Check Box", &Checked);
        GuiPainterValueBox("Value Box", &ValueOptions);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
