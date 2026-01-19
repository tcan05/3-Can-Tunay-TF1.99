#pragma once
#include "raylib.h"

enum class PowerUpType
{
    SPEED,
    HEALTH_ONCE,
    DOUBLE_SHOT,
    BARRAGE,
    PIERCING
};

struct PowerUp
{
    PowerUpType type;
    Vector2 pos;
    float radius;
    bool active;
    float respawnTimer; // counts down when inactive

    PowerUp(PowerUpType t, Vector2 p, float r)
        : type(t), pos(p), radius(r), active(true), respawnTimer(0.0f) {
    }
};

void DrawPowerUp(const PowerUp& pu);
