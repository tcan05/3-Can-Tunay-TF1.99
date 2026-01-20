// ===== SelectControls.cpp =====
#include "SelectControls.h"
#include "raylib.h"

static Texture2D controlsBackground = { 0 };
static bool bgLoaded = false;

static const char* ResolveBackgroundPath()
{
    const char* path1 = "Assets/arena.png";
    const char* path2 = "TF199/Assets/arena.png";

    if (FileExists(path1)) return path1;
    if (FileExists(path2)) return path2;

    return nullptr;
}

SelectControls::SelectControls()
    : backButton({ 100, 510, 180, 60 }, "BACK", 30)
{
    backPressed = false;

    if (!bgLoaded)
    {
        const char* bgPath = ResolveBackgroundPath();
        if (bgPath)
        {
            controlsBackground = LoadTexture(bgPath);
        }
        bgLoaded = true;
    }
}

void SelectControls::Update()
{
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectControls::Draw()
{
    ClearBackground(BLACK);

    if (controlsBackground.id != 0)
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        DrawTexturePro(
            controlsBackground,
            { 0, 0, (float)controlsBackground.width, (float)controlsBackground.height },
            { 0, 0, sw, sh },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    DrawText("Controls", 100, 100, 50, WHITE);

    // Draw player controls
    DrawText("Player 1", 100, 200, 30, BLACK);
    DrawText("Move:  W A S D", 100, 240, 25, WHITE);
    DrawText("Shoot: SPACE", 100, 270, 25, WHITE);
    DrawText("Dash:  L-SHIFT", 100, 300, 25, WHITE);

    DrawText("Player 2", 100, 360, 30, BLACK);
    DrawText("Move:  Arrow Keys", 100, 400, 25, WHITE);
    DrawText("Shoot: R-CTRL", 100, 430, 25, WHITE);
    DrawText("Dash:  R-SHIFT", 100, 460, 25, WHITE);

    // Draw power-up descriptions
    DrawText("Power Ups", 400, 200, 30, BLACK);

    DrawText("Speed Boost: ", 400, 240, 25, DARKBLUE);
    DrawText("Faster Movement (5 sec)", 570, 240, 25, WHITE);

    DrawText("More Health: ", 400, 280, 25, DARKGREEN);
    DrawText("+1 HP (max HP is 3)", 570, 280, 25, WHITE);

    DrawText("Double Shot: ", 400, 320, 25, YELLOW);
    DrawText("Shoot 2 bullets (5 sec)", 570, 320, 25, WHITE);

    DrawText("Barrage: ", 400, 360, 25, RED);
    DrawText("Shoot bullets in every direction.", 570, 360, 25, WHITE);

    DrawText("Piercing: ", 400, 400, 25, DARKPURPLE);
    DrawText("Shoot bullets throught obstacles (5 sec)", 570, 400, 25, WHITE);

    backButton.Draw();
}
