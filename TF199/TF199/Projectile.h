// ===== Projectile.h =====
#pragma once
#include "raylib.h"

class Projectile
{
public:
    int ownerId;          // 1 = blue, 2 = red
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius;
    bool isActive;

    // NEW: bullets can pass through walls
    bool piercing;

public:
    Projectile(int ownerId, Vector2 startPos, Vector2 dir, bool piercing = false);

    void Update(float dt);
    void Draw() const;

    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
};
