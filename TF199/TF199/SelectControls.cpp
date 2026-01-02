#include "SelectControls.h"
#include "raylib.h"

SelectControls::SelectControls()
	: backButton({ 100, 510, 180, 60 }, "BACK", 30)
{
	backPressed = false;
}

void SelectControls::Update()
{
	Vector2 mousePos = GetMousePosition();
	bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectControls::Draw()
{
    DrawText("Controls", 100, 100, 50, WHITE);

    DrawText("Player 1", 100, 200, 30, GOLD);
    DrawText("Move:  W A S D", 100, 240, 25, WHITE);
    DrawText("Shoot: SPACE", 100, 270, 25, WHITE);
    DrawText("Dash:  L-SHIFT", 100, 300, 25, WHITE);

    DrawText("Player 2", 100, 360, 30, GOLD);
    DrawText("Move:  Arrow Keys", 100, 400, 25, WHITE);
    DrawText("Shoot: R-CTRL", 100, 430, 25, WHITE);
    DrawText("Dash:  R-SHIFT", 100, 460, 25, WHITE);

	backButton.Draw();
}