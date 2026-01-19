// ===== Projectile.cpp =====
#include "Projectile.h"
#include <cmath>

static inline Vector2 NormalizeSafe(Vector2 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len <= 0.0001f) return { 1.0f, 0.0f };
    return { v.x / len, v.y / len };
}

Projectile::Projectile(int ownerId, Vector2 startPos, Vector2 dir, bool piercing)
{
    this->ownerId = ownerId;
    this->position = startPos;
    this->direction = NormalizeSafe(dir);

    this->speed = 650.0f;
    this->radius = 6.0f;
    this->isActive = true;

    this->piercing = piercing;
}

void Projectile::Update(float dt)
{
    if (!isActive) return;

    position.x += direction.x * speed * dt;
    position.y += direction.y * speed * dt;

    // deactivate if off-screen
    if (position.x < -50 || position.x > 1280 + 50 ||
        position.y < -50 || position.y > 720 + 50)
    {
        isActive = false;
    }
}

void Projectile::Draw() const
{
    if (!isActive) return;

    // simple "animation": pulse + tiny trail
    float t = (float)GetTime();
    float pulse = 1.0f + 0.20f * sinf(t * 20.0f);
    float r = radius * pulse;

    Color c;
    if (piercing)
    {
        c = VIOLET; // piercing bullets look different
    }
    else
    {
        c = (ownerId == 1) ? SKYBLUE : RED; // red-ish
    }

    // trail
    Vector2 back = { position.x - direction.x * 10.0f, position.y - direction.y * 10.0f };
    DrawLineEx(back, position, 3.0f, Fade(c, 0.6f));

    // bullet core
    DrawCircleV(position, r, c);
    DrawCircleLines((int)position.x, (int)position.y, r, BLACK);
}
