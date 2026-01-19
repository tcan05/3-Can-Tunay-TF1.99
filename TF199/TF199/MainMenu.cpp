// ===== MainMenu.cpp =====
#include "MainMenu.h"
#include "raylib.h"

static Texture2D menuBackground = { 0 };
static bool bgLoaded = false;

static const char* ResolveBackgroundPath()
{
    const char* path1 = "Assets/main_menu_bg.png";
    const char* path2 = "TF199/Assets/main_menu_bg.png";

    TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());
    TraceLog(LOG_INFO, "Try path1: %s (exists=%d)", path1, FileExists(path1));
    TraceLog(LOG_INFO, "Try path2: %s (exists=%d)", path2, FileExists(path2));

    if (FileExists(path1)) return path1;
    if (FileExists(path2)) return path2;

    return nullptr;
}

MainMenu::MainMenu()
    : startButton({ 100, 200, 320, 60 }, "START", 30),
    controlsButton({ 100, 290, 320, 60 }, "CONTROLS", 30),
    creditsButton({ 100, 380, 320, 60 }, "CREDITS", 30),
    exitButton({ 100, 510, 320, 60 }, "EXIT", 30)
{
    startPressed = false;
    controlsPressed = false;
    creditsPressed = false;
    exitPressed = false;

    exitButton.SetColors(MAROON, RED, DARKGRAY, YELLOW, RAYWHITE);

    if (!bgLoaded)
    {
        const char* bgPath = ResolveBackgroundPath();

        if (bgPath)
        {
            menuBackground = LoadTexture(bgPath);

            if (menuBackground.id == 0)
            {
                TraceLog(LOG_ERROR, "LoadTexture failed even though file exists: %s", bgPath);
            }
            else
            {
                TraceLog(LOG_INFO, "Background loaded OK: %s | id=%d | w=%d h=%d",
                    bgPath, menuBackground.id, menuBackground.width, menuBackground.height);
            }
        }
        else
        {
            TraceLog(LOG_ERROR, "Background NOT found in either Assets/... or TF199/Assets/...");
        }

        bgLoaded = true;
    }
}

void MainMenu::Update()
{
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    startPressed = startButton.IsPressed(mousePos, mousePressed);
    controlsPressed = controlsButton.IsPressed(mousePos, mousePressed);
    creditsPressed = creditsButton.IsPressed(mousePos, mousePressed);
    exitPressed = exitButton.IsPressed(mousePos, mousePressed);
}

void MainMenu::Draw()
{
    ClearBackground(BLACK);

    if (menuBackground.id != 0)
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        DrawTexturePro(
            menuBackground,
            { 0, 0, (float)menuBackground.width, (float)menuBackground.height },
            { 0, 0, sw, sh },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    DrawText("Two Forts 1.99", 50, 110, 64, RAYWHITE);

    startButton.Draw();
    controlsButton.Draw();
    creditsButton.Draw();
    exitButton.Draw();
}
