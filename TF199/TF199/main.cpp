#include <iostream>
#include "raylib.h"
#include "PlayerCharacter.h"
#include "MainMenu.h"
#include "SelectGameMode.h"
#include "SelectControls.h"
#include "SelectCredits.h"
#include "GameState.h"

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
    std::string winnerText;

    InitWindow(screenWidth, screenHeight, "TF199");
	SetTargetFPS(60);

	GameState gameState = GameState::MAIN_MENU;

    MainMenu mainMenu;
    SelectGameMode modeMenu;
    SelectControls controlsMenu;
    SelectCredits creditsMenu;

    bool highNoonInitialized = false;
    const float HIGH_NOON_COUNTDOWN = 2.5f;
    float highNoonCountdown = HIGH_NOON_COUNTDOWN;
    bool highNoonCanMove = false;


	// Initialize Players
    PlayerCharacter player1(1, { 100, 360 });
	player1.SetKeys(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LEFT_SHIFT);

    PlayerCharacter player2(2, { 1180, 360 });
    player2.SetKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_CONTROL, KEY_RIGHT_SHIFT);


    while (WindowShouldClose() == false)
    {

        BeginDrawing();
		ClearBackground(BLACK);

        float dt = GetFrameTime();

        switch (gameState)
        {
            case GameState::MAIN_MENU:
            
                mainMenu.Update();
                mainMenu.Draw();

                if (mainMenu.startPressed)
                {
                    gameState = GameState::SELECT_MODE;
                }
                else if (mainMenu.controlsPressed)
                {
                    gameState = GameState::CONTROLS;
                }
                else if (mainMenu.creditsPressed)
                {
                    gameState = GameState::CREDITS;
                }
                else if (mainMenu.exitPressed)
                {
                    CloseWindow();
                    return 0;
                }
                break;
            
            case GameState::CONTROLS:
            
                controlsMenu.Update();
				controlsMenu.Draw();

                if (controlsMenu.backPressed)
                {
					gameState = GameState::MAIN_MENU;
                }

                break;

            case GameState::CREDITS:

                creditsMenu.Update();
                creditsMenu.Draw();

                if (creditsMenu.backPressed)
                {
                    gameState = GameState::MAIN_MENU;
                }

                break;
            
            case GameState::SELECT_MODE:
            
                modeMenu.Update();
                modeMenu.Draw();

                if (modeMenu.arenaPressed)
                {
                    player1.SetMaxHp(3);
                    player2.SetMaxHp(3);

                    player1.Reset({ 100, 360 });
                    player2.Reset({ 1180, 360 });

                    player1.projectiles.clear();
                    player2.projectiles.clear();

                    highNoonInitialized = false;
                    gameState = GameState::GAME_ARENA;
                }

                else if (modeMenu.highNoonPressed)
                {
                    highNoonInitialized = false; 
                    gameState = GameState::GAME_HIGH_NOON;
                    highNoonCountdown = HIGH_NOON_COUNTDOWN;
                    highNoonCanMove = false;

                }

                else if (modeMenu.backPressed)
                {
                    gameState = GameState::MAIN_MENU;
                }
                break;
            
            case GameState::GAME_ARENA:
            
                ClearBackground(GRAY);

                player1.Draw();
                player2.Draw();

                player1.Update(dt, player2.projectiles);
                player2.Update(dt, player1.projectiles);

                DrawText("ARENA", 20, 20, 30, DARKBLUE);
                DrawText("3 HP - 3rd time's the charm", 20, 55, 20, SKYBLUE);
                DrawText("Backspace: Main Menu", 20, 80, 20, YELLOW);

                if (player1.isDead || player2.isDead)
                {
                    gameState = GameState::GAME_OVER;
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    player1.Reset({ 100, 360 });
                    player2.Reset({ 1180, 360 });

                    player1.projectiles.clear();
                    player2.projectiles.clear();

                    winnerText.clear();
                    gameState = GameState::MAIN_MENU;
                }

                break;

            case GameState::GAME_HIGH_NOON:
            {
                ClearBackground(DARKBROWN);

                // Reset Gamemode
                if (IsKeyPressed(KEY_R))
                {
                    highNoonInitialized = false;
                    highNoonCountdown = HIGH_NOON_COUNTDOWN;
                    highNoonCanMove = false;

                    player1.projectiles.clear();
                    player2.projectiles.clear();
                }

                // Randomize Player Spawn Location
                if (!highNoonInitialized)
                {
                    player1.SetMaxHp(1);
                    player2.SetMaxHp(1);

                    Vector2 p1Spawn = {
                        (float)GetRandomValue(150, 450),
                        (float)GetRandomValue(150, 570)
                    };
                    Vector2 p2Spawn = {
                        (float)GetRandomValue(830, 1130),
                        (float)GetRandomValue(150, 570)
                    };

                    player1.Reset(p1Spawn);
                    player2.Reset(p2Spawn);

                    player1.beginningPos = p1Spawn;
                    player2.beginningPos = p2Spawn;

                    highNoonCountdown = HIGH_NOON_COUNTDOWN;
                    highNoonCanMove = false;

                    highNoonInitialized = true;
                }
                
                // Can't Move Until Countdown Ends
                if (!highNoonCanMove)
                {
                    highNoonCountdown -= dt;
                    if (highNoonCountdown <= 0.0f)
                    {
                        highNoonCountdown = 0.0f;
                        highNoonCanMove = true;
                    }
    
                }
                player1.Draw();
                player2.Draw();

                if (highNoonCanMove)
                {
                    player1.Update(dt, player2.projectiles);
                    player2.Update(dt, player1.projectiles);
                }
                
                if (player1.isDead || player2.isDead)
                {
                    gameState = GameState::GAME_OVER;
                }

                DrawText("HIGH NOON", 20, 20, 30, RED);
                DrawText("1 HP - one hit kills", 20, 55, 20, GOLD);
                DrawText("Backspace: Main Menu", 20, 80, 20, YELLOW);
                DrawText("R: Restart Round", 20, 105, 20, YELLOW);

                // Show Countdown
                if (!highNoonCanMove)
                {
                    if (highNoonCountdown > 1.5f) DrawText("3", 620, 120, 90, GOLD);
                    else if (highNoonCountdown > 1.0f) DrawText("2", 620, 120, 90, GOLD);
                    else if (highNoonCountdown > 0.5f) DrawText("1", 620, 120, 90, GOLD);
                    else DrawText("DRAW!", 520, 120, 90, RED);
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    highNoonInitialized = false;
                    highNoonCountdown = HIGH_NOON_COUNTDOWN;
                    highNoonCanMove = false;
                    gameState = GameState::MAIN_MENU;
                }

                break;
            }

            case GameState::GAME_OVER:

                // Shows Who Won
                if (player1.hp <= 0 && player2.hp <= 0)
                {
                    winnerText = "Draw";
                }
                else if (player1.hp <= 0)
                {
                    ClearBackground(RED);
                    winnerText = "RED WINS !";
                }
                else
                {
                    ClearBackground(DARKBLUE);
                    winnerText = "BLUE WINS !";
                }

                DrawText(winnerText.c_str(), 400, 360, 40, GOLD);
                DrawText("Press BACKSPACE to return to Main Menu.", 400, 560, 20, YELLOW);

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    player1.Reset({ 100, 360 });
                    player2.Reset({ 1180, 360 });

                    player1.projectiles.clear();
                    player2.projectiles.clear();

                    winnerText.clear();
                    gameState = GameState::MAIN_MENU;
                }
                break;
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}