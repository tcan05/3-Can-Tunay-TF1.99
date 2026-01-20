// ===== Map.cpp =====
#include "Map.h"
#include <algorithm>
#include "raylib.h"

static Wall Solid(float x, float y, float w, float h)
{
    return { {x, y, w, h}, false, 0 };
}

static Wall Breakable(float x, float y, float w, float h, int hp)
{
    return { {x, y, w, h}, true, hp };
}

static Texture2D boxTex = { 0 };
static bool boxLoaded = false;

static const char* ResolveBoxPath()
{
    const char* p1 = "Assets/box.png";
    const char* p2 = "TF199/Assets/box.png";

    if (FileExists(p1)) return p1;
    if (FileExists(p2)) return p2;

    return nullptr;
}

static void EnsureBoxLoaded()
{
    if (boxLoaded) return;

    const char* p = ResolveBoxPath();
    if (p)
    {
        boxTex = LoadTexture(p);
        if (boxTex.id != 0) SetTextureFilter(boxTex, TEXTURE_FILTER_POINT);
    }

    boxLoaded = true;
}

static void DrawTextureTiledRect(Texture2D tex, Rectangle dst)
{
    if (tex.id == 0) return;

    float tw = (float)tex.width;
    float th = (float)tex.height;

    for (float y = dst.y; y < dst.y + dst.height; y += th)
    {
        float h = th;
        if (y + h > dst.y + dst.height) h = (dst.y + dst.height) - y;

        for (float x = dst.x; x < dst.x + dst.width; x += tw)
        {
            float w = tw;
            if (x + w > dst.x + dst.width) w = (dst.x + dst.width) - x;

            Rectangle src = { 0, 0, w, h };
            Rectangle d = { x, y, w, h };

            DrawTexturePro(tex, src, d, { 0, 0 }, 0.0f, WHITE);
        }
    }
}

MapData CreateMap(MapId id)
{
    MapData map;
    map.name = "UNKNOWN";
    map.background = DARKGRAY;
    map.walls.clear();
    map.powerUpSpawns.clear();

    if (id == MapId::RANDOM)
        id = (MapId)GetRandomValue(0, 4);

    switch (id)
    {
    case MapId::MAP_1:

        map.name = "COURTYARD";
        map.background = Color{ 55, 55, 60, 255 };

        map.walls = {
            Solid(220, 140, 120, 120),
            Solid(940, 140, 120, 120),
            Solid(220, 460, 120, 120),
            Solid(940, 460, 120, 120),

            Solid(560, 120, 40, 200),
            Solid(680, 400, 40, 200),

            Breakable(610, 310, 60, 100, 3),
            Breakable(430, 310, 60, 100, 2)
        };

        map.powerUpSpawns = {
            Vector2{ 320, 360 },
            Vector2{ 930, 360 },
            Vector2{ 625, 220 },
            Vector2{ 625, 520 },
            Vector2{ 625, 360 }
        };
        break;

    case MapId::MAP_2:
        map.name = "PILLARS";
        map.background = DARKGRAY;
        map.walls = {
            Solid(350,200,40,120),
            Solid(350,400,40,120),
            Solid(890,200,40,120),
            Solid(890,400,40,120),
            Breakable(610,260,60,80,2),
            Breakable(610,420,60,80,2)
        };
        map.powerUpSpawns = {
            Vector2{ 260, 180 },
            Vector2{ 990, 180 },
            Vector2{ 625, 360 },
            Vector2{ 260, 560 },
            Vector2{ 990, 560 }
        };
        break;

    case MapId::MAP_3:
        map.name = "CROSS";
        map.background = DARKGRAY;
        map.walls = {
            Solid(610,160,60,150),
            Solid(610,410,60,150),
            Solid(430,330,180,60),
            Solid(670,330,180,60),
            Breakable(610,330,60,60,2)
        };
        map.powerUpSpawns = {
            Vector2{ 320, 200 },
            Vector2{ 930, 200 },
            Vector2{ 625, 360 },
            Vector2{ 320, 540 },
            Vector2{ 930, 540 }
        };
        break;

    case MapId::MAP_4:
        map.name = "LANES";
        map.background = DARKGRAY;
        map.walls = {
            Solid(450,120,30,480),
            Solid(800,120,30,480),
            Breakable(470,320,330,30,2),
            Breakable(470,420,330,30,2)
        };
        map.powerUpSpawns = {
            Vector2{ 260, 200 },
            Vector2{ 990, 200 },
            Vector2{ 625, 240 },
            Vector2{ 625, 520 },
            Vector2{ 260, 560 }
        };
        break;

    case MapId::MAP_5:
        map.name = "DIAMOND";
        map.background = DARKGRAY;
        map.walls = {
            Solid(560,180,160,40),
            Solid(560,500,160,40),
            Solid(420,320,40,160),
            Solid(820,320,40,160),
            Breakable(610,320,60,160,3)
        };
        map.powerUpSpawns = {
            Vector2{ 360, 220 },
            Vector2{ 890, 220 },
            Vector2{ 625, 360 },
            Vector2{ 360, 540 },
            Vector2{ 890, 540 }
        };
        break;

    case MapId::DESERT:
        map.name = "DESERT";
        map.background = BROWN;
        map.walls = {};
        map.powerUpSpawns = {};
        break;

    default:
        break;
    }

    return map;
}

void DrawMap(const MapData& map)
{
    EnsureBoxLoaded();

    for (const auto& w : map.walls)
    {
        if (w.breakable)
        {
            if (boxTex.id != 0)
            {
                DrawTextureTiledRect(boxTex, w.rect);
            }
            else
            {
                DrawRectangleRec(w.rect, ORANGE);
            }
            DrawRectangleLinesEx(w.rect, 2, BLACK);
        }
        else
        {
            DrawRectangleRec(w.rect, GRAY);
            DrawRectangleLinesEx(w.rect, 2, BLACK);

            float step = 16.0f;
            for (float x = w.rect.x + step; x < w.rect.x + w.rect.width; x += step)
                DrawLine((int)x, (int)w.rect.y, (int)x, (int)(w.rect.y + w.rect.height), DARKGRAY);

            for (float y = w.rect.y + step; y < w.rect.y + w.rect.height; y += step)
                DrawLine((int)w.rect.x, (int)y, (int)(w.rect.x + w.rect.width), (int)y, DARKGRAY);
        }
    }
}

bool CircleHitsAnyWall(Vector2 center, float radius, const MapData& map)
{
    for (const auto& w : map.walls)
        if (CheckCollisionCircleRec(center, radius, w.rect))
            return true;
    return false;
}

void ResolveProjectileWallCollisions(std::vector<Projectile>& projectiles, MapData& map)
{
    for (auto& p : projectiles)
    {
        if (!p.isActive) continue;

        for (auto& w : map.walls)
        {
            if (CheckCollisionCircleRec(p.GetPosition(), p.GetRadius(), w.rect))
            {
                if (w.breakable) w.hp--;

                if (!p.piercing)
                {
                    p.isActive = false;
                    break;
                }
            }
        }
    }

    map.walls.erase(
        std::remove_if(map.walls.begin(), map.walls.end(),
            [](const Wall& w) { return w.breakable && w.hp <= 0; }),
        map.walls.end()
    );
}
