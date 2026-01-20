// ===== PlayerCharacter.cpp =====
#include "PlayerCharacter.h"
#include "raylib.h"

// ---- static storage ----
bool PlayerCharacter::assetsLoaded = false;
Texture2D PlayerCharacter::blueFrames[4] = {};
Texture2D PlayerCharacter::redFrames[5] = {};

// ---- helpers ----
static inline float ClampF(float v, float mn, float mx)
{
    if (v < mn) return mn;
    if (v > mx) return mx;
    return v;
}

static inline Vector2 NormalizeSafe(Vector2 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len <= 0.0001f) return { 1.0f, 0.0f };
    return { v.x / len, v.y / len };
}

static inline Vector2 Rot(Vector2 v, float a)
{
    float cs = cosf(a), sn = sinf(a);
    return { v.x * cs - v.y * sn, v.x * sn + v.y * cs };
}

// ---- assets ----
void PlayerCharacter::LoadAssets()
{
    if (assetsLoaded) return;

    // Blue: sprite_05..sprite_08
    blueFrames[0] = LoadTexture("Assets/blue/sprite_05.png");
    blueFrames[1] = LoadTexture("Assets/blue/sprite_06.png");
    blueFrames[2] = LoadTexture("Assets/blue/sprite_05.png");
    blueFrames[3] = LoadTexture("Assets/blue/sprite_06.png");

    // Red: sprite_00..sprite_04
    redFrames[0] = LoadTexture("Assets/red/sprite_00.png");
    redFrames[1] = LoadTexture("Assets/red/sprite_03.png");
    redFrames[2] = LoadTexture("Assets/red/sprite_00.png");
    redFrames[3] = LoadTexture("Assets/red/sprite_03.png");
    redFrames[4] = LoadTexture("Assets/red/sprite_00.png");

    assetsLoaded = true;
}

void PlayerCharacter::UnloadAssets()
{
    if (!assetsLoaded) return;

    for (int i = 0; i < 4; i++) UnloadTexture(blueFrames[i]);
    for (int i = 0; i < 5; i++) UnloadTexture(redFrames[i]);

    assetsLoaded = false;
}

// ---- ctor ----
PlayerCharacter::PlayerCharacter(int id, Vector2 startPos) : id(id), pos(startPos), beginningPos(startPos)
{
    this->color = (id == 1) ? BLUE : RED;

    this->radius = 20;
    this->speed = 7;
    this->maxHp = 3;
    this->hp = maxHp;

    this->shootingDelay = 30;
    this->shootingCooldown = 0;

    this->dashDistance = 100;
    this->dashCooldown = 5;
    this->dashTimer = 0;

    bulletDirection = { 1.0f, 0.0f };

    Gunshot = LoadSound("Assets/SFX/GunshotMetallic.wav");
    SetSoundVolume(Gunshot, 0.3f);

    dashSound = LoadSound("Assets/SFX/DashTest.wav");
	SetSoundVolume(dashSound, 0.2f);

    if (id == 1)
    {
        hitSound = LoadSound("Assets/SFX/FleshHit.wav");
    }
    else
    {
        hitSound = LoadSound("Assets/SFX/MachineHit.wav");
    }
}

// ---- animation helpers ----
void PlayerCharacter::SetState(AnimState s)
{
    if (animState == s) return;
    animState = s;
    frame = 0;
    frameTimer = 0.0f;
}

int PlayerCharacter::GetMaxFramesFor(AnimState s) const
{
    switch (s)
    {
    case AnimState::IDLE:  return (id == 1) ? 4 : 5;
    case AnimState::RUN:   return (id == 1) ? 4 : 5;
    case AnimState::SHOOT: return (id == 1) ? 4 : 5;
    case AnimState::HURT:  return (id == 1) ? 4 : 5;
    case AnimState::DEAD:  return (id == 1) ? 4 : 5;
    }
    return 1;
}

float PlayerCharacter::GetSPFFor(AnimState s) const
{
    switch (s)
    {
    case AnimState::IDLE:  return IDLE_SPF;
    case AnimState::RUN:   return RUN_SPF;
    case AnimState::SHOOT: return SHOOT_SPF;
    case AnimState::HURT:  return HURT_SPF;
    case AnimState::DEAD:  return DEAD_SPF;
    }
    return 0.12f;
}

void PlayerCharacter::StepAnimation(float dt, int maxFrames, float spf)
{
    if (maxFrames <= 1) return;

    frameTimer += dt;
    if (frameTimer >= spf)
    {
        frameTimer -= spf;
        frame++;
        if (frame >= maxFrames) frame = 0;
    }
}

Texture2D PlayerCharacter::GetCurrentTexture() const
{
    int maxFrames = GetMaxFramesFor(animState);
    int idx = frame;
    if (idx < 0) idx = 0;
    if (idx >= maxFrames) idx = maxFrames - 1;

    if (id == 1)
    {
        if (idx >= 4) idx = 3;
        return blueFrames[idx];
    }
    else
    {
        if (idx >= 5) idx = 4;
        return redFrames[idx];
    }
}

// ---- draw ----
void PlayerCharacter::Draw()
{
    // draw bullets
    for (auto& projectile : projectiles)
        projectile.Draw();

    // draw player sprite
    Texture2D tex = GetCurrentTexture();
    if (tex.id == 0)
    {
        DrawCircleV(pos, (float)radius, color);
        return;
    }

    // compute fade (death)
    float alpha = 1.0f;
    if (isDead)
    {
        alpha = ClampF(1.0f - (deathTimer / DEATH_FADE_TIME), 0.0f, 1.0f);
    }

    // hurt flash
    bool flash = (invulnTimer > 0.0f) && (fmodf((float)GetTime() * 18.0f, 1.0f) < 0.5f);

    Color tint = WHITE;
    tint.a = (unsigned char)(255.0f * alpha);
    if (flash) tint = Color{ 255, 255, 255, (unsigned char)(255.0f * alpha) };

    // draw centered, with flip
    float w = (float)tex.width * SPRITE_SCALE;
    float h = (float)tex.height * SPRITE_SCALE;

    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    if (facing < 0) src.width = -src.width; // flip

    Rectangle dst = { pos.x, pos.y, w, h };
    Vector2 origin = { w / 2.0f, h / 2.0f };

    DrawTexturePro(tex, src, dst, origin, 0.0f, tint);
}

// ---- reset / hp ----
void PlayerCharacter::Reset(Vector2 startPos)
{
    pos = startPos;
    beginningPos = startPos;

    hp = maxHp;
    isDead = false;

    shootingCooldown = 0;
    dashTimer = 0;

    // reset animation/effects
    animState = AnimState::IDLE;
    frame = 0;
    frameTimer = 0;
    facing = 1;

    hurtTimer = 0;
    invulnTimer = 0;
    deathTimer = 0;
    shootAnimTimer = 0;

    // reset powerups
    speedBoostTimer = 0;
    doubleShotTimer = 0;
    piercingTimer = 0;
    usedHealthOnce = false;
}

void PlayerCharacter::SetMaxHp(int max)
{
    maxHp = max;
    hp = maxHp;
    isDead = false;
}

void PlayerCharacter::SetKeys(int up, int down, int left, int right, int shoot, int dash)
{
    keyUp = up; keyDown = down; keyLeft = left; keyRight = right;
    keyShoot = shoot; keyDash = dash;
}

// ---- update ----
void PlayerCharacter::Update(float dt, std::vector<Projectile>& enemyProjectiles)
{
    if (isDead)
    {
        deathTimer += dt;
        return;
    }

    // tick power-up timers
    if (speedBoostTimer > 0.0f)  speedBoostTimer -= dt;
    if (doubleShotTimer > 0.0f)  doubleShotTimer -= dt;
    if (piercingTimer > 0.0f)    piercingTimer -= dt;

    // tick hurt/invuln timers
    if (hurtTimer > 0.0f)   hurtTimer -= dt;
    if (invulnTimer > 0.0f) invulnTimer -= dt;
    if (shootAnimTimer > 0.0f) shootAnimTimer -= dt;

    // effective speed (+25%)
    int effectiveSpeed = speed;
    if (speedBoostTimer > 0.0f)
        effectiveSpeed = (int)roundf((float)speed * 1.25f);

    // movement input
    Vector2 movement = { 0,0 };
    bool moved = false;

    if (IsKeyDown(keyUp)) { movement.y -= 1; pos.y -= effectiveSpeed; moved = true; }
    if (IsKeyDown(keyDown)) { movement.y += 1; pos.y += effectiveSpeed; moved = true; }
    if (IsKeyDown(keyLeft)) { movement.x -= 1; pos.x -= effectiveSpeed; moved = true; facing = -1; }
    if (IsKeyDown(keyRight)) { movement.x += 1; pos.x += effectiveSpeed; moved = true; facing = 1; }

    // update bullet direction from movement
    if (movement.x != 0 || movement.y != 0)
        bulletDirection = NormalizeSafe(movement);

    // pick animation state
    if (hurtTimer > 0.0f) SetState(AnimState::HURT);
    else if (shootAnimTimer > 0.0f) SetState(AnimState::SHOOT);
    else SetState(moved ? AnimState::RUN : AnimState::IDLE);

    StepAnimation(dt, GetMaxFramesFor(animState), GetSPFFor(animState));

    // shooting cooldown
    if (shootingCooldown > 0) shootingCooldown--;

    bool pierce = (piercingTimer > 0.0f);

    // shooting
    if (shootingCooldown <= 0 && IsKeyDown(keyShoot))
    {
        PlaySound(Gunshot);
        Vector2 d = NormalizeSafe(bulletDirection);

        if (doubleShotTimer > 0.0f)
        {
            Vector2 d1 = Rot(d, -0.12f);
            Vector2 d2 = Rot(d, +0.12f);
            projectiles.emplace_back(id, pos, d1, pierce);
            projectiles.emplace_back(id, pos, d2, pierce);
        }
        else
        {
            projectiles.emplace_back(id, pos, d, pierce);
        }

        shootingCooldown = shootingDelay;
        shootAnimTimer = SHOOT_ANIM_TIME;
        SetState(AnimState::SHOOT);
    }

    // update own projectiles
    for (auto& projectile : projectiles)
        projectile.Update(dt);

    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.isActive; }),
        projectiles.end()
    );

    // collision with enemy bullets (invuln prevents repeat hits)
    for (auto& projectile : enemyProjectiles)
    {
        if (!projectile.isActive) continue;

        if (CheckCollisionCircles(pos, (float)radius, projectile.GetPosition(), projectile.GetRadius()))
        {
            if (invulnTimer > 0.0f) continue;

            hp--;
            PlaySound(hitSound);
            projectile.isActive = false;

            if (hp <= 0)
            {
                isDead = true;
                deathTimer = 0.0f;
                SetState(AnimState::DEAD);
                break;
            }
            else
            {
                // hurt feedback + short invulnerability
                hurtTimer = HURT_TIME;
                invulnTimer = INVULN_TIME;
                SetState(AnimState::HURT);

                // *** FIX: respawn to round start (fixed spawn) ***
                pos = beginningPos;
            }
        }
    }

    enemyProjectiles.erase(
        std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
            [](const Projectile& p) { return !p.isActive; }),
        enemyProjectiles.end()
    );

    // dashing
    if (dashTimer > 0) dashTimer--;
    else if (IsKeyDown(keyDash))
    {
        PlaySound(dashSound);

        if (IsKeyDown(keyUp)) pos.y -= dashDistance;
        else if (IsKeyDown(keyDown)) pos.y += dashDistance;
        else if (IsKeyDown(keyLeft)) pos.x -= dashDistance;
        else if (IsKeyDown(keyRight)) pos.x += dashDistance;

        dashTimer = dashCooldown * 60;
    }

    // keep inside screen
    pos.x = ClampF(pos.x, (float)radius, 1280.0f - (float)radius);
    pos.y = ClampF(pos.y, (float)radius, 720.0f - (float)radius);
}
