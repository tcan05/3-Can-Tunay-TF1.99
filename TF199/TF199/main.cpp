// ===== main.cpp =====
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>

#include "raylib.h"

#include "PlayerCharacter.h"
#include "MainMenu.h"
#include "SelectGameMode.h"
#include "SelectControls.h"
#include "SelectCredits.h"
#include "SelectMap.h"
#include "GameState.h"
#include "Scoreboard.h"
#include "Map.h"
#include "PowerUp.h"

enum class ActiveMode
{
    NONE,
    ARENA,
    HIGH_NOON
};

static const char* ResolveGameBgPath()
{
    const char* p1 = "Assets/game.png";
    const char* p2 = "TF199/Assets/game.png";

    if (FileExists(p1)) return p1;
    if (FileExists(p2)) return p2;

    return nullptr;
}

static const char* ResolveWoodTexPath()
{
    const char* p1 = "Assets/box.png";
    const char* p2 = "TF199/Assets/box.png";

    if (FileExists(p1)) return p1;
    if (FileExists(p2)) return p2;

    return nullptr;
}

static const char* ResolveMetalTexPath()
{
    const char* p1 = "Assets/metalbox.png";
    const char* p2 = "TF199/Assets/metalbox.png";

    if (FileExists(p1)) return p1;
    if (FileExists(p2)) return p2;

    return nullptr;
}

int main()
{
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "TF199");
    SetTargetFPS(60);

    SetRandomSeed((unsigned int)time(nullptr));

    PlayerCharacter::LoadAssets();

    GameState gameState = GameState::MAIN_MENU;

    MainMenu mainMenu;
    SelectGameMode modeMenu;
    SelectControls controlsMenu;
    SelectCredits creditsMenu;
    SelectMap mapMenu;

    ActiveMode pendingMode = ActiveMode::NONE;
    ActiveMode activeMode = ActiveMode::NONE;
    MapData currentMap = CreateMap(MapId::MAP_1);

    Scoreboard scoreboard;
    std::string winnerText;

    Texture2D gameBg = { 0 };
    {
        const char* bgPath = ResolveGameBgPath();
        if (bgPath)
        {
            gameBg = LoadTexture(bgPath);
        }
    }

    Texture2D arenaFloor = LoadTexture("Assets/arenafloor.png");
    SetTextureFilter(arenaFloor, TEXTURE_FILTER_POINT);

    Texture2D woodTex = { 0 };
    {
        const char* p = ResolveWoodTexPath();
        if (p) woodTex = LoadTexture(p);
        if (woodTex.id != 0) SetTextureFilter(woodTex, TEXTURE_FILTER_POINT);
    }

    Texture2D metalTex = { 0 };
    {
        const char* p = ResolveMetalTexPath();
        if (p) metalTex = LoadTexture(p);
        if (metalTex.id != 0) SetTextureFilter(metalTex, TEXTURE_FILTER_POINT);
    }

    InitAudioDevice();
    Sound sfxMenuClick = LoadSound("Assets/SFX/MenuClick.wav");

    TraceLog(LOG_INFO, "CWD: %s", GetWorkingDirectory());
    TraceLog(LOG_INFO, "Exists? %d", FileExists("Assets/SFX/Victory.wav"));

    Music menuMusic = LoadMusicStream("Assets/SFX/MainMenu.wav");
    Music arenaMusic = LoadMusicStream("Assets/SFX/Charge.ogg");
    Music highNoonMusic = LoadMusicStream("Assets/SFX/LassoLady.ogg");
    Sound victoryMusic = LoadSound("Assets/SFX/Victory.wav");

    Sound woodenHitSfx = LoadSound("Assets/SFX/WoodenHit.wav");
    Sound metalHitSfx = LoadSound("Assets/SFX/MetalHit.wav");

    menuMusic.looping = true;
    arenaMusic.looping = true;
    highNoonMusic.looping = true;

    SetMusicVolume(menuMusic, 0.8f);
    SetMusicVolume(arenaMusic, 0.5f);
    SetMusicVolume(highNoonMusic, 0.5f);
    SetSoundVolume(victoryMusic, 0.6f);
    SetSoundVolume(woodenHitSfx, 0.8f);
    SetSoundVolume(metalHitSfx, 0.7f);

    bool highNoonInitialized = false;
    const float HIGH_NOON_COUNTDOWN = 2.5f;
    float highNoonCountdown = HIGH_NOON_COUNTDOWN;
    bool highNoonCanMove = false;

    bool gameOverProcessed = false;
    int lastWinnerPlayerId = 0;

    PlayerCharacter player1(1, { 260, 360 });
    player1.SetKeys(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LEFT_SHIFT);

    PlayerCharacter player2(2, { 990, 360 });
    player2.SetKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_CONTROL, KEY_RIGHT_SHIFT);

    std::vector<PowerUp> powerUps;

    static constexpr float PU_BUFF = 5.0f;
    static constexpr int PU_RESPAWN_MIN = 5;
    static constexpr int PU_RESPAWN_MAX = 15;

    // Random power-up respawn time
    auto RandomPURespawn = [&]() -> float
        {
            return (float)GetRandomValue(PU_RESPAWN_MIN, PU_RESPAWN_MAX);
        };

    // Build power-ups for Arena
    auto BuildArenaPowerUps = [&]()
        {
            powerUps.clear();
            if (currentMap.powerUpSpawns.empty()) return;

            PowerUpType cycle[] = {
                PowerUpType::SPEED,
                PowerUpType::DOUBLE_SHOT,
                PowerUpType::PIERCING,
                PowerUpType::BARRAGE,
                PowerUpType::HEALTH_ONCE
            };

            int ti = 0;
            for (Vector2 p : currentMap.powerUpSpawns)
            {
                if (CircleHitsAnyWall(p, 18.0f, currentMap)) continue;

                powerUps.emplace_back(cycle[ti % 5], p, 18.0f);
                powerUps.back().active = false;
                powerUps.back().respawnTimer = RandomPURespawn();
                ti++;
            }
        };

    // Reset players for Arena Mode
    auto ResetArenaRound = [&]()
        {
            player1.SetMaxHp(3);
            player2.SetMaxHp(3);

            player1.projectiles.clear();
            player2.projectiles.clear();

            Vector2 p1 = { 260, 360 };
            Vector2 p2 = { 990, 360 };

            for (int i = 0; i < 50; i++)
            {
                if (!CircleHitsAnyWall(p1, player1.GetRadius(), currentMap)) break;
                p1.x += 10; p1.y += 10;
            }
            for (int i = 0; i < 50; i++)
            {
                if (!CircleHitsAnyWall(p2, player2.GetRadius(), currentMap)) break;
                p2.x -= 10; p2.y -= 10;
            }

            player1.Reset(p1);
            player2.Reset(p2);
        };

    // Spawn boxes for High Noon Mode
    auto SpawnHighNoonBoxes = [&]()
        {
            currentMap.walls.clear();

            const float bw = 48.0f;
            const float bh = 128.0f;

            const float minX = 100.0f;
            const float maxX = (float)screenWidth - 100.0f - bw;
            const float minY = 160.0f;
            const float maxY = (float)screenHeight - 80.0f - bh;

            const float pad = 12.0f;

            // Check if box is not inside player or wall 
            auto IsSafeRect = [&](const Rectangle& r) -> bool
                {
                    if (CheckCollisionCircleRec(player1.GetPos(), player1.GetRadius() + pad, r)) return false;
                    if (CheckCollisionCircleRec(player2.GetPos(), player2.GetRadius() + pad, r)) return false;

                    for (const auto& w : currentMap.walls)
                    {
                        if (CheckCollisionRecs(w.rect, r)) return false;
                    }
                    return true;
                };

            int placed = 0;
            int tries = 0;

            while (placed < 3 && tries < 5000)
            {
                tries++;

                float x = (float)GetRandomValue((int)minX, (int)maxX);
                float y = (float)GetRandomValue((int)minY, (int)maxY);

                Rectangle r = { x, y, bw, bh };

                if (!IsSafeRect(r)) continue;

                Wall w;
                w.rect = r;
                w.breakable = true;
                w.hp = 2;
                currentMap.walls.push_back(w);

                placed++;
            }
        };

    // Reset players for High Noon Mode
    auto ResetHighNoonRound = [&]()
        {
            player1.SetMaxHp(1);
            player2.SetMaxHp(1);

            player1.projectiles.clear();
            player2.projectiles.clear();

            Vector2 p1Spawn = { 300, 360 };
            Vector2 p2Spawn = { 980, 360 };

            player1.Reset(p1Spawn);
            player2.Reset(p2Spawn);

            player1.beginningPos = p1Spawn;
            player2.beginningPos = p2Spawn;

            SpawnHighNoonBoxes();

            highNoonCountdown = HIGH_NOON_COUNTDOWN;
            highNoonCanMove = false;
            highNoonInitialized = true;
        };

    auto DrawScoreHUD = [&]()
        {
            DrawText(TextFormat("SCORE  MAN: %d   MACHINE: %d", scoreboard.blueWins, scoreboard.redWins),
                20, 140, 24, RAYWHITE);
        };

    auto DrawWallTextures = [&]()
        {
            const float tilescale = 0.04f;
            for (const auto& w : currentMap.walls)
            {
                Texture2D tex = w.breakable ? woodTex : metalTex;
                if (tex.id == 0) continue;

                for (float yy = w.rect.y; yy < w.rect.y + w.rect.height; yy += tex.height)
                {
                    for (float xx = w.rect.x; xx < w.rect.x + w.rect.width; xx += tex.width)
                    {
                        float dw = std::min((float)tex.width, (w.rect.x + w.rect.width) - xx);
                        float dh = std::min((float)tex.height, (w.rect.y + w.rect.height) - yy);

                        Rectangle src = { 0, 0, dw / tilescale, dh / tilescale};
                        Rectangle dst = { xx, yy, dw, dh };

                        DrawTexturePro(tex, src, dst, { 0, 0 }, 0.0f, WHITE);
                    }
                }

                DrawRectangleLinesEx(w.rect, 2, BLACK);
            }
        };


    auto PlayHitSoundsForProjectiles = [&](std::vector<Projectile>& projs)
        {
            for (auto& p : projs)
            {
                if (!p.isActive) continue;

                for (const auto& w : currentMap.walls)
                {
                    if (CheckCollisionCircleRec(p.GetPosition(), p.GetRadius(), w.rect))
                    {
                        if (w.breakable) PlaySound(woodenHitSfx);
                        else PlaySound(metalHitSfx);
                        break;
                    }
                }
            }
        };

    PlayMusicStream(menuMusic);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        UpdateMusicStream(menuMusic);
        UpdateMusicStream(arenaMusic);
        UpdateMusicStream(highNoonMusic);

        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameState)
        {
        case GameState::MAIN_MENU:
        {
            mainMenu.Update();
            mainMenu.Draw();

            if (mainMenu.startPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::SELECT_MODE;
            }
            else if (mainMenu.controlsPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::CONTROLS;
            }
            else if (mainMenu.creditsPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::CREDITS;
            }
            else if (mainMenu.exitPressed)
            {
                PlayerCharacter::UnloadAssets();
                CloseWindow();
                return 0;
            }
            break;
        }

        case GameState::CONTROLS:
        {
            controlsMenu.Update();
            controlsMenu.Draw();

            if (controlsMenu.backPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::MAIN_MENU;
            }
            break;
        }

        case GameState::CREDITS:
        {
            creditsMenu.Update();
            creditsMenu.Draw();

            if (creditsMenu.backPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::MAIN_MENU;
            }
            break;
        }

        case GameState::SELECT_MODE:
        {
            modeMenu.Update();
            modeMenu.Draw();

            if (modeMenu.arenaPressed)
            {
                PlaySound(sfxMenuClick);
                pendingMode = ActiveMode::ARENA;
                gameState = GameState::SELECT_MAP;
            }
            else if (modeMenu.highNoonPressed)
            {
                StopMusicStream(menuMusic);
                PlayMusicStream(highNoonMusic);

                activeMode = ActiveMode::HIGH_NOON;
                pendingMode = ActiveMode::NONE;

                currentMap = CreateMap(MapId::DESERT);

                gameOverProcessed = false;
                winnerText.clear();

                highNoonInitialized = false;
                highNoonCountdown = HIGH_NOON_COUNTDOWN;
                highNoonCanMove = false;

                gameState = GameState::GAME_HIGH_NOON;
            }
            else if (modeMenu.backPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::MAIN_MENU;
            }
            break;
        }

        case GameState::SELECT_MAP:
        {
            mapMenu.Update();
            mapMenu.Draw();

            if (mapMenu.backPressed)
            {
                PlaySound(sfxMenuClick);
                gameState = GameState::SELECT_MODE;
                break;
            }

            if (mapMenu.map1Pressed || mapMenu.map2Pressed || mapMenu.map3Pressed ||
                mapMenu.map4Pressed || mapMenu.map5Pressed || mapMenu.randomPressed)
            {
                StopMusicStream(menuMusic);
                PlayMusicStream(arenaMusic);

                MapId chosen = mapMenu.GetSelectedMap();
                currentMap = CreateMap(chosen);

                activeMode = pendingMode;
                pendingMode = ActiveMode::NONE;

                gameOverProcessed = false;
                winnerText.clear();

                ResetArenaRound();
                BuildArenaPowerUps();
                gameState = GameState::GAME_ARENA;
            }
            break;
        }

        case GameState::GAME_ARENA:
        {
            ClearBackground(BLACK);

            if (arenaFloor.id != 0)
            {
                int sw = GetScreenWidth();
                int sh = GetScreenHeight();

                float scale = 0.25f;

                int stepX = (int)(arenaFloor.width * scale);
                int stepY = (int)(arenaFloor.height * scale);
                if (stepX < 1) stepX = 1;
                if (stepY < 1) stepY = 1;

                for (int y = 0; y < sh; y += stepY)
                {
                    for (int x = 0; x < sw; x += stepX)
                    {
                        float dw = (float)stepX;
                        float dh = (float)stepY;

                        if (x + dw > sw) dw = (float)sw - (float)x;
                        if (y + dh > sh) dh = (float)sh - (float)y;

                        Rectangle src = { 0, 0, dw / scale, dh / scale };
                        Rectangle dst = { (float)x, (float)y, dw, dh };

                        DrawTexturePro(arenaFloor, src, dst, { 0, 0 }, 0.0f, WHITE);
                    }
                }
            }

            DrawMap(currentMap);
            DrawWallTextures();

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                scoreboard.Reset();
                winnerText.clear();
                activeMode = ActiveMode::NONE;

                powerUps.clear();

                ResetArenaRound();
                gameState = GameState::MAIN_MENU;

                StopMusicStream(arenaMusic);
                PlayMusicStream(menuMusic);

                break;
            }

            for (auto& pu : powerUps)
            {
                if (!pu.active)
                {
                    pu.respawnTimer -= dt;
                    if (pu.respawnTimer <= 0.0f)
                    {
                        pu.active = true;
                        pu.respawnTimer = 0.0f;
                    }
                    continue;
                }

                DrawPowerUp(pu);

                // Check whether player took the power-up or not
                auto TryPickup = [&](PlayerCharacter& pl)
                    {
                        if (!pu.active) return;

                        if (CheckCollisionCircles(pl.GetPos(), pl.GetRadius(), pu.pos, pu.radius))
                        {
                            switch (pu.type)
                            {
                            case PowerUpType::SPEED:
                                pl.speedBoostTimer = PU_BUFF;
                                break;

                            case PowerUpType::DOUBLE_SHOT:
                                pl.doubleShotTimer = PU_BUFF;
                                break;

                            case PowerUpType::PIERCING:
                                pl.piercingTimer = PU_BUFF;
                                break;

                            case PowerUpType::BARRAGE:
                                for (int i = 0; i < 12; i++)
                                {
                                    float ang = (2.0f * PI / 12.0f) * i;
                                    Vector2 d = { cosf(ang), sinf(ang) };
                                    pl.projectiles.emplace_back(pl.id, pl.GetPos(), d, false);
                                }
                                break;

                            case PowerUpType::HEALTH_ONCE:
                                if (!pl.usedHealthOnce)
                                {
                                    pl.usedHealthOnce = true;
                                    pl.hp = std::min(pl.hp + 1, pl.maxHp);
                                }
                                break;
                            }

                            pu.active = false;
                            pu.respawnTimer = RandomPURespawn();
                        }
                    };

                TryPickup(player1);
                TryPickup(player2);
            }

            Vector2 old1 = player1.GetPos();
            Vector2 old2 = player2.GetPos();

            player1.Update(dt, player2.projectiles);
            player2.Update(dt, player1.projectiles);

            if (CircleHitsAnyWall(player1.GetPos(), player1.GetRadius(), currentMap)) player1.SetPos(old1);
            if (CircleHitsAnyWall(player2.GetPos(), player2.GetRadius(), currentMap)) player2.SetPos(old2);

            PlayHitSoundsForProjectiles(player1.projectiles);
            PlayHitSoundsForProjectiles(player2.projectiles);

            ResolveProjectileWallCollisions(player1.projectiles, currentMap);
            ResolveProjectileWallCollisions(player2.projectiles, currentMap);

            player1.Draw();
            player2.Draw();

            DrawText("ARENA", 20, 20, 30, DARKBLUE);
            DrawText("3 HP - 3rd time's the charm", 20, 55, 20, SKYBLUE);
            DrawText("Backspace: Main Menu (score resets)", 20, 80, 20, YELLOW);
            DrawScoreHUD();

            if (player1.isDead || player2.isDead)
            {
                gameOverProcessed = false;
                gameState = GameState::GAME_OVER;
                StopMusicStream(arenaMusic);
            }
            break;
        }

        case GameState::GAME_HIGH_NOON:
        {
            ClearBackground(BLACK);

            if (gameBg.id != 0)
            {
                float sw = (float)GetScreenWidth();
                float sh = (float)GetScreenHeight();

                DrawTexturePro(
                    gameBg,
                    { 0, 0, (float)gameBg.width, (float)gameBg.height },
                    { 0, 0, sw, sh },
                    { 0, 0 },
                    0.0f,
                    WHITE
                );
            }

            DrawMap(currentMap);
            DrawWallTextures();

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                scoreboard.Reset();
                winnerText.clear();
                activeMode = ActiveMode::NONE;

                highNoonInitialized = false;
                highNoonCountdown = HIGH_NOON_COUNTDOWN;
                highNoonCanMove = false;

                ResetArenaRound();
                gameState = GameState::MAIN_MENU;

                StopMusicStream(highNoonMusic);
                PlayMusicStream(menuMusic);
                break;
            }

            if (!highNoonInitialized)
            {
                ResetHighNoonRound();
            }

            if (!highNoonCanMove)
            {
                highNoonCountdown -= dt;
                if (highNoonCountdown <= 0.0f)
                {
                    highNoonCountdown = 0.0f;
                    highNoonCanMove = true;
                }
            }

            if (highNoonCanMove)
            {
                Vector2 old1 = player1.GetPos();
                Vector2 old2 = player2.GetPos();

                player1.Update(dt, player2.projectiles);
                player2.Update(dt, player1.projectiles);

                if (CircleHitsAnyWall(player1.GetPos(), player1.GetRadius(), currentMap)) player1.SetPos(old1);
                if (CircleHitsAnyWall(player2.GetPos(), player2.GetRadius(), currentMap)) player2.SetPos(old2);

                PlayHitSoundsForProjectiles(player1.projectiles);
                PlayHitSoundsForProjectiles(player2.projectiles);

                ResolveProjectileWallCollisions(player1.projectiles, currentMap);
                ResolveProjectileWallCollisions(player2.projectiles, currentMap);
            }

            player1.Draw();
            player2.Draw();

            if (player1.isDead || player2.isDead)
            {
                gameOverProcessed = false;
                gameState = GameState::GAME_OVER;
                StopMusicStream(highNoonMusic);
            }

            DrawText("HIGH NOON", 20, 20, 30, RED);
            DrawText("1 HP - one hit kills", 20, 55, 20, GOLD);
            DrawText("Backspace: Main Menu (score resets)", 20, 80, 20, YELLOW);
            DrawScoreHUD();

            if (!highNoonCanMove)
            {
                if (highNoonCountdown > 1.5f) DrawText("3", 620, 120, 90, GOLD);
                else if (highNoonCountdown > 1.0f) DrawText("2", 620, 120, 90, GOLD);
                else if (highNoonCountdown > 0.5f) DrawText("1", 620, 120, 90, GOLD);
                else DrawText("DRAW!", 520, 120, 90, RED);
            }

            break;
        }

        case GameState::GAME_OVER:
        {
            if (player1.hp <= 0 && player2.hp <= 0)
            {
                winnerText = "DRAW";
                lastWinnerPlayerId = 0;
                ClearBackground(BLACK);
            }
            else if (player1.hp <= 0)
            {
                winnerText = "MACHINE WINS !";
                lastWinnerPlayerId = 2;
                ClearBackground(DARKGRAY);
            }
            else
            {
                winnerText = "MAN WINS !";
                lastWinnerPlayerId = 1;
                ClearBackground(DARKBLUE);
            }

            if (!gameOverProcessed)
            {
                PlaySound(victoryMusic);

                if (lastWinnerPlayerId != 0)
                    scoreboard.AddWin(lastWinnerPlayerId);
                gameOverProcessed = true;
            }

            int sw = GetScreenWidth();
            int titleSize = 50;
            int titleW = MeasureText(winnerText.c_str(), titleSize);
            DrawText(winnerText.c_str(), (sw - titleW) / 2, 300, titleSize, GOLD);

            const char* l1 = "Press R to restart immediately";
            DrawText(l1, (sw - MeasureText(l1, 24)) / 2, 420, 24, RAYWHITE);

            const char* l2 = "Press BACKSPACE to return to Main Menu";
            DrawText(l2, (sw - MeasureText(l2, 22)) / 2, 460, 22, YELLOW);

            const char* scoreLine = TextFormat("SCORE  MAN: %d   MACHINE: %d", scoreboard.blueWins, scoreboard.redWins);
            DrawText(scoreLine, (sw - MeasureText(scoreLine, 28)) / 2, 520, 28, RAYWHITE);

            if (IsKeyPressed(KEY_R))
            {
                StopSound(victoryMusic);

                winnerText.clear();
                gameOverProcessed = false;

                if (activeMode == ActiveMode::ARENA)
                {
                    ResetArenaRound();
                    BuildArenaPowerUps();
                    gameState = GameState::GAME_ARENA;
                    PlayMusicStream(arenaMusic);
                }
                else if (activeMode == ActiveMode::HIGH_NOON)
                {
                    highNoonInitialized = false;
                    currentMap = CreateMap(MapId::DESERT);
                    ResetHighNoonRound();
                    gameState = GameState::GAME_HIGH_NOON;
                    PlayMusicStream(highNoonMusic);
                }
                else
                {
                    gameState = GameState::MAIN_MENU;
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                StopSound(victoryMusic);

                scoreboard.Reset();
                winnerText.clear();
                activeMode = ActiveMode::NONE;

                powerUps.clear();

                highNoonInitialized = false;
                highNoonCountdown = HIGH_NOON_COUNTDOWN;
                highNoonCanMove = false;

                ResetArenaRound();
                gameState = GameState::MAIN_MENU;
                PlayMusicStream(menuMusic);
            }

            break;
        }

        default:
            break;
        }

        EndDrawing();
    }

    PlayerCharacter::UnloadAssets();
    CloseWindow();
    return 0;
}
