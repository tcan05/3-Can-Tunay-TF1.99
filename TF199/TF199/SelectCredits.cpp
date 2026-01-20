// ===== SelectCredits.cpp =====
#include "SelectCredits.h"
#include "raylib.h"

static Texture2D creditsBackground = { 0 };
static bool bgLoaded = false;

static const char* ResolveBackgroundPath()
{
    const char* path1 = "Assets/arena.png";
    const char* path2 = "TF199/Assets/arena.png";

    if (FileExists(path1)) return path1;
    if (FileExists(path2)) return path2;

    return nullptr;
}

SelectCredits::SelectCredits()
    : backButton({ 100, 510, 180, 60 }, "BACK", 30)
{
    backPressed = false;

    if (!bgLoaded)
    {
        const char* bgPath = ResolveBackgroundPath();
        if (bgPath)
        {
            creditsBackground = LoadTexture(bgPath);
        }
        bgLoaded = true;
    }
}

void SelectCredits::Update()
{
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectCredits::Draw()
{
    ClearBackground(BLACK);

    if (creditsBackground.id != 0)
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        DrawTexturePro(
            creditsBackground,
            { 0, 0, (float)creditsBackground.width, (float)creditsBackground.height },
            { 0, 0, sw, sh },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    DrawText("Credits", 100, 100, 50, WHITE);

    DrawText("Programming", 100, 200, 40, BLACK);
    DrawText("- Tunay Can", 100, 250, 30, WHITE);
    DrawText("- Abdullah G. H. Alamarah", 100, 290, 30, WHITE);

    DrawText("Textures Found", 700, 200, 40, BLACK);
    DrawText("- Abdullah G. H. Alamarah", 700, 250, 30, WHITE);

    DrawText("SFX Found", 700, 320, 40, BLACK);
    DrawText("- Tunay Can", 700, 370, 30, WHITE);

    backButton.Draw();
}
