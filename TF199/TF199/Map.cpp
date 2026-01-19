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
            // corner blocks (solid)
            Solid(220, 140, 120, 120),
            Solid(940, 140, 120, 120),
            Solid(220, 460, 120, 120),
            Solid(940, 460, 120, 120),

            // mid separators (solid)
            Solid(560, 120, 40, 200),
            Solid(680, 400, 40, 200),

            // breakables (dynamic cover)
            Breakable(610, 310, 60, 100, 3),
            Breakable(430, 310, 60, 100, 2)
        };

        // fixed power-up spawns
        map.powerUpSpawns = {
            Vector2{ 320, 360 },  // left mid
            Vector2{ 930, 360 },  // right mid
            Vector2{ 625, 220 },  // top mid
            Vector2{ 625, 520 },  // bottom mid
            Vector2{ 625, 360 }   // center
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
    for (const auto& w : map.walls)
    {
        Color c = w.breakable ? ORANGE : GRAY;
        DrawRectangleRec(w.rect, c);
        DrawRectangleLinesEx(w.rect, 2, BLACK);
    }
}

bool CircleHitsAnyWall(Vector2 center, float radius, const MapData& map)
{
    for (const auto& w : map.walls)
        if (CheckCollisionCircleRec(center, radius, w.rect))
            return true;
    return false;
}

// UPDATED: supports piercing bullets (pierce walls only)
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
                // piercing continues
            }
        }
    }

    map.walls.erase(
        std::remove_if(map.walls.begin(), map.walls.end(),
            [](const Wall& w) { return w.breakable && w.hp <= 0; }),
        map.walls.end()
    );
}
