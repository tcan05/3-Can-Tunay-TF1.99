#include <iostream>
#include "raylib.h"
#include "PlayerCharacter.h"

int main()
{
	const int screenWidth = 1280;
	const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "TF199");
	SetTargetFPS(60);

    PlayerCharacter player1(1, { 100, 360 });
	player1.SetKeys(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LEFT_SHIFT);

    PlayerCharacter player2(2, { 1180, 360 });
    player2.SetKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_CONTROL, KEY_RIGHT_SHIFT);

    while (WindowShouldClose() == false)
    {
        float dt = GetFrameTime();

        player1.Update(dt);
        player2.Update(dt);

        BeginDrawing();

		ClearBackground(GRAY);
        player1.Draw();
        player2.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}