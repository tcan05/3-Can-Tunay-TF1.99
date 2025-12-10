#include <iostream>
#include "raylib.h"
#include "PlayerCharacter.h"
#include "MainMenu.h"
#include "SelectGameMode.h"
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
                else if (mainMenu.exitPressed)
                {
                    CloseWindow();
                    return 0;
                }
                break;
            
            case GameState::CONTROLS:
            
                DrawText("Controls:", 100, 100, 50, WHITE);

                DrawText("Player 1:", 100, 200, 30, WHITE);
                DrawText("Move: W A S D", 100, 240, 25, WHITE);
                DrawText("Shoot: SPACE", 100, 270, 25, WHITE);
                DrawText("Dash:  L-SHIFT", 100, 300, 25, WHITE);

                DrawText("Player 2:", 100, 360, 30, WHITE);
                DrawText("Move: Arrow Keys", 100, 400, 25, WHITE);
                DrawText("Shoot: R-CTRL", 100, 430, 25, WHITE);
                DrawText("Dash:  R-SHIFT", 100, 460, 25, WHITE);

                DrawText("Press BACKSPACE to go back.", 100, 550, 30, YELLOW);

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    gameState = GameState::MAIN_MENU;
                }
                break;
            
            
            case GameState::SELECT_MODE:
            
                modeMenu.Update();
                modeMenu.Draw();

                if (modeMenu.arenaPressed)
                {
                    gameState = GameState::GAME_ARENA;
                }
                else if (modeMenu.highNoonPressed)
                {
                    gameState = GameState::GAME_HIGH_NOON;
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

                if (player1.isDead || player2.isDead)
                {

                    if (player1.hp <= 0 && player2.hp <= 0)
                    {
                        winnerText = "Draw";
                    }
                    else if (player1.hp <= 0)
                    {
                        winnerText = "RED WINS !";
                    }
                    else
                    {
                        winnerText = "BLUE WINS !";
                    }
                    gameState = GameState::GAME_OVER;
                }
                break;
            
            case GameState::GAME_HIGH_NOON:

                // Random Spawn Point, 1HP
                // CHECK FOR COLLISION
            
				DrawText("High Noon Comin' Soon!", 400, 360, 40, WHITE);
                DrawText("Press BACKSPACE to return to Main Menu.", 400, 560, 20, YELLOW);

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    gameState = GameState::MAIN_MENU;
                }
                break;

            case GameState::GAME_OVER:

                ClearBackground(DARKGRAY);

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