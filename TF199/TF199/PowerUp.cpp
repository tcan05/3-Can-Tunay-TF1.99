#include "PowerUp.h"
#include <cmath>

static void DrawLightning(Vector2 c, float s, Color col)
{
    DrawLineEx({ c.x - s * 0.30f, c.y - s * 0.35f }, { c.x, c.y - s * 0.10f }, 3, col);
    DrawLineEx({ c.x, c.y - s * 0.10f }, { c.x - s * 0.10f, c.y + s * 0.10f }, 3, col);
    DrawLineEx({ c.x - s * 0.10f, c.y + s * 0.10f }, { c.x + s * 0.28f, c.y + s * 0.40f }, 3, col);
}

static void DrawPlus(Vector2 c, float s, Color col)
{
    DrawRectangle((int)(c.x - s * 0.10f), (int)(c.y - s * 0.35f), (int)(s * 0.20f), (int)(s * 0.70f), col);
    DrawRectangle((int)(c.x - s * 0.35f), (int)(c.y - s * 0.10f), (int)(s * 0.70f), (int)(s * 0.20f), col);
}

static void DrawDouble(Vector2 c, float s, Color col)
{
    Vector2 a1 = { c.x - s * 0.35f, c.y };
    Vector2 b1 = { c.x - s * 0.05f, c.y - s * 0.25f };
    Vector2 d1 = { c.x - s * 0.05f, c.y + s * 0.25f };
    DrawTriangle(a1, b1, d1, col);

    Vector2 a2 = { c.x + s * 0.05f, c.y };
    Vector2 b2 = { c.x + s * 0.35f, c.y - s * 0.25f };
    Vector2 d2 = { c.x + s * 0.35f, c.y + s * 0.25f };
    DrawTriangle(a2, b2, d2, col);
}

static void DrawBurst(Vector2 c, float s, Color col)
{
    for (int i = 0; i < 8; i++)
    {
        float ang = (PI / 4.0f) * i;
        Vector2 p1 = { c.x + cosf(ang) * (s * 0.15f), c.y + sinf(ang) * (s * 0.15f) };
        Vector2 p2 = { c.x + cosf(ang) * (s * 0.55f), c.y + sinf(ang) * (s * 0.55f) };
        DrawLineEx(p1, p2, 3, col);
    }
}

static void DrawPierce(Vector2 c, float s, Color col)
{
    DrawCircleLines((int)c.x, (int)c.y, s * 0.45f, col);
    DrawLineEx({ c.x - s * 0.45f, c.y }, { c.x + s * 0.45f, c.y }, 3, col);
}

void DrawPowerUp(const PowerUp& pu)
{
    if (!pu.active) return;

    float pulse = 1.0f + 0.08f * sinf((float)GetTime() * 6.0f);
    float r = pu.radius * pulse;

    Color base = DARKGRAY;
    Color icon = WHITE;

    switch (pu.type)
    {
        case PowerUpType::SPEED:       base = Color{ 40, 80, 200, 255 }; icon = SKYBLUE; break;
        case PowerUpType::HEALTH_ONCE: base = Color{ 30, 130, 60, 255 }; icon = LIME;    break;
        case PowerUpType::DOUBLE_SHOT: base = Color{ 170, 120, 20, 255 }; icon = GOLD;   break;
        case PowerUpType::BARRAGE:     base = Color{ 170, 40, 40, 255 }; icon = ORANGE;  break;
        case PowerUpType::PIERCING:    base = Color{ 120, 60, 170, 255 }; icon = VIOLET; break;
    }

    DrawCircleV(pu.pos, r, base);
    DrawCircleLines((int)pu.pos.x, (int)pu.pos.y, r, BLACK);

    float s = pu.radius * 2.2f;
    switch (pu.type)
    {
        case PowerUpType::SPEED:       DrawLightning(pu.pos, s, icon); break;
        case PowerUpType::HEALTH_ONCE: DrawPlus(pu.pos, s, icon); break;
        case PowerUpType::DOUBLE_SHOT: DrawDouble(pu.pos, s, icon); break;
        case PowerUpType::BARRAGE:     DrawBurst(pu.pos, s, icon); break;
        case PowerUpType::PIERCING:    DrawPierce(pu.pos, s, icon); break;
    }
}
