// ===== Map.h =====
#pragma once
#include <vector>
#include "raylib.h"
#include "Projectile.h"

struct Wall
{
    Rectangle rect;
    bool breakable;
    int hp;
};

struct MapData
{
    const char* name;
    Color background;
    std::vector<Wall> walls;

    // NEW: fixed power-up spawn points (ARENA mode only)
    std::vector<Vector2> powerUpSpawns;
};

enum class MapId
{
    MAP_1,
    MAP_2,
    MAP_3,
    MAP_4,
    MAP_5,
    RANDOM,
    DESERT
};

MapData CreateMap(MapId id);
void DrawMap(const MapData& map);
bool CircleHitsAnyWall(Vector2 center, float radius, const MapData& map);
void ResolveProjectileWallCollisions(std::vector<Projectile>& projectiles, MapData& map);
