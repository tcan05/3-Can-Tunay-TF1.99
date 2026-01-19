// ===== SelectGameMode.cpp =====
#include "SelectGameMode.h"
#include "raylib.h"

static Texture2D modeBackground = { 0 };
static bool bgLoaded = false;

static const char* ResolveBackgroundPath()
{
    const char* path1 = "Assets/main_menu_bg.png";
    const char* path2 = "TF199/Assets/main_menu_bg.png";

    if (FileExists(path1)) return path1;
    if (FileExists(path2)) return path2;

    return nullptr;
}

SelectGameMode::SelectGameMode()
    : arenaButton({ 100, 200, 420, 60 }, "ARENA", 30),
    highNoonButton({ 100, 300, 420, 60 }, "HIGH NOON", 30),
    backButton({ 100, 510, 180, 60 }, "BACK", 30)
{
    arenaPressed = false;
    highNoonPressed = false;
    backPressed = false;

    arenaButton.SetColors(GRAY, DARKGRAY, DARKGRAY, YELLOW, SKYBLUE);
    highNoonButton.SetColors(BROWN, DARKBROWN, DARKGRAY, YELLOW, RED);

    if (!bgLoaded)
    {
        const char* bgPath = ResolveBackgroundPath();
        if (bgPath)
        {
            modeBackground = LoadTexture(bgPath);
        }
        bgLoaded = true;
    }
}

void SelectGameMode::Update()
{
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    arenaPressed = arenaButton.IsPressed(mousePos, mousePressed);
    highNoonPressed = highNoonButton.IsPressed(mousePos, mousePressed);
    backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectGameMode::Draw()
{
    ClearBackground(BLACK);

    if (modeBackground.id != 0)
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        DrawTexturePro(
            modeBackground,
            { 0, 0, (float)modeBackground.width, (float)modeBackground.height },
            { 0, 0, sw, sh },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    DrawText("SELECT GAME MODE", 110, 120, 40, WHITE);

    arenaButton.Draw();
    highNoonButton.Draw();
    backButton.Draw();
}
