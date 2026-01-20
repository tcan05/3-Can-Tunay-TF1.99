// ===== SelectMap.cpp =====
#include "SelectMap.h"
#include "raylib.h"

static Texture2D mapBackground = { 0 };
static bool bgLoaded = false;

static const char* ResolveBackgroundPath()
{
    const char* path1 = "Assets/arena.png";
    const char* path2 = "TF199/Assets/arena.png";

    if (FileExists(path1)) return path1;
    if (FileExists(path2)) return path2;

    return nullptr;
}

SelectMap::SelectMap()
    : map1Btn({ 100, 200, 420, 60 }, "MAP 1 - BOX RING", 24),
    map2Btn({ 100, 270, 420, 60 }, "MAP 2 - PILLARS", 24),
    map3Btn({ 100, 340, 420, 60 }, "MAP 3 - CROSS", 24),
    map4Btn({ 100, 410, 420, 60 }, "MAP 4 - LANES", 24),
    map5Btn({ 100, 480, 420, 60 }, "MAP 5 - DIAMOND", 24),
    randomBtn({ 340, 590, 180, 60 }, "RANDOM", 30),
    backBtn({ 100, 590, 180, 60 }, "BACK", 30)
{
    map1Pressed = false;
    map2Pressed = false;
    map3Pressed = false;
    map4Pressed = false;
    map5Pressed = false;
    randomPressed = false;
    backPressed = false;

    if (!bgLoaded)
    {
        const char* bgPath = ResolveBackgroundPath();
        if (bgPath)
        {
            mapBackground = LoadTexture(bgPath);
        }
        bgLoaded = true;
    }
}

void SelectMap::Update()
{
    Vector2 mp = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    map1Pressed = map1Btn.IsPressed(mp, click);
    map2Pressed = map2Btn.IsPressed(mp, click);
    map3Pressed = map3Btn.IsPressed(mp, click);
    map4Pressed = map4Btn.IsPressed(mp, click);
    map5Pressed = map5Btn.IsPressed(mp, click);

    randomPressed = randomBtn.IsPressed(mp, click);
    backPressed = backBtn.IsPressed(mp, click);
}

void SelectMap::Draw()
{
    ClearBackground(BLACK);

    if (mapBackground.id != 0)
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();

        DrawTexturePro(
            mapBackground,
            { 0, 0, (float)mapBackground.width, (float)mapBackground.height },
            { 0, 0, sw, sh },
            { 0, 0 },
            0.0f,
            WHITE
        );
    }

    DrawText("SELECT MAP", 110, 120, 40, WHITE);

    map1Btn.Draw();
    map2Btn.Draw();
    map3Btn.Draw();
    map4Btn.Draw();
    map5Btn.Draw();

    randomBtn.Draw();
    backBtn.Draw();
}

MapId SelectMap::GetSelectedMap() const
{
    if (map1Pressed) return MapId::MAP_1;
    if (map2Pressed) return MapId::MAP_2;
    if (map3Pressed) return MapId::MAP_3;
    if (map4Pressed) return MapId::MAP_4;
    if (map5Pressed) return MapId::MAP_5;
    if (randomPressed) return MapId::RANDOM;

    return MapId::MAP_1;
}
