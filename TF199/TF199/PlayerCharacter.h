// ===== PlayerCharacter.h =====
#pragma once
#include "raylib.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "Projectile.h"

class PlayerCharacter
{
public:
    // Attributes
    int id;
    int hp;
    int maxHp;
    int radius;
    int speed;
    bool isDead = false;
    Vector2 pos;
    Vector2 beginningPos;
    Color color;
    std::vector<Projectile> projectiles;
    Sound Gunshot;
    Sound hitSound;
    Sound dashSound;

    // Input Keys
    int keyUp, keyDown, keyLeft, keyRight, keyShoot, keyDash;

    // Shooting Controls
    int shootingDelay;
    int shootingCooldown;
    Vector2 bulletDirection;

    // Dashing Controls
    int dashDistance;
    int dashCooldown;
    int dashTimer;

    // ---- NEW: power-up state (Arena only, but safe everywhere) ----
    float speedBoostTimer = 0.0f;   // +25% speed for 5s
    float doubleShotTimer = 0.0f;   // shoot 2 bullets for 5s
    float piercingTimer = 0.0f;   // bullets pierce walls for 5s
    bool usedHealthOnce = false;  // +1 HP only once per player per match

    // Methods
    PlayerCharacter(int id, Vector2 startPos);
    void Draw();
    void Update(float dt, std::vector<Projectile>& enemyProjectiles);
    void Reset(Vector2 startPos);
    void SetKeys(int up, int down, int left, int right, int shoot, int dash);
    void SetMaxHp(int max);

    Vector2 GetPos() const { return pos; }
    void SetPos(Vector2 p) { pos = p; }
    float GetRadius() const { return (float)radius; }

    // ---- sprite assets ----
    static void LoadAssets();
    static void UnloadAssets();

private:
    enum class AnimState { IDLE, RUN, SHOOT, HURT, DEAD };

    AnimState animState = AnimState::IDLE;
    int frame = 0;
    float frameTimer = 0.0f;

    // facing direction: +1 right, -1 left
    int facing = 1;

    // hit + invuln + death fade
    float hurtTimer = 0.0f;
    float invulnTimer = 0.0f;
    float deathTimer = 0.0f;

    // shoot animation timer (NO movement lock)
    float shootAnimTimer = 0.0f;

private:
    void SetState(AnimState s);
    void StepAnimation(float dt, int maxFrames, float spf);

    int GetMaxFramesFor(AnimState s) const;
    float GetSPFFor(AnimState s) const;

    // returns the texture to draw for the current state/frame
    Texture2D GetCurrentTexture() const;

    // shared asset storage
    static bool assetsLoaded;

    // Blue (player 1) frames: 05-08 => 4 frames
    static Texture2D blueFrames[4];

    // Red (player 2) frames: 00-04 => 5 frames
    static Texture2D redFrames[5];

    // timings
    static constexpr float IDLE_SPF = 0.16f;
    static constexpr float RUN_SPF = 0.10f;
    static constexpr float SHOOT_SPF = 0.07f;
    static constexpr float HURT_SPF = 0.10f;
    static constexpr float DEAD_SPF = 0.12f;

    // effect durations
    static constexpr float HURT_TIME = 0.18f;
    static constexpr float INVULN_TIME = 0.45f;
    static constexpr float SHOOT_ANIM_TIME = 0.15f;
    static constexpr float DEATH_FADE_TIME = 0.75f;

    // sprite scaling
    // NOTE: 0.05f is VERY small; if your character is tiny, try 0.12f-0.20f.
    static constexpr float SPRITE_SCALE = 0.05f;
};
