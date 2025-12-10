#include "SelectGameMode.h"
#include "raylib.h"

SelectGameMode::SelectGameMode() : arenaButton("Assets/Start Purple.png", { 100, 200 }, 0.8f),
								   highNoonButton("Assets/Start Purple.png", { 100, 300 }, 0.8f),
								   backButton("Assets/Exit.png", { 100, 500 }, 0.8f)
{
	arenaPressed = false;
	highNoonPressed = false;
	backPressed = false;
}

void SelectGameMode::Update()
{
	Vector2 mousePos = GetMousePosition();
	bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	arenaPressed = arenaButton.IsPressed(mousePos, mousePressed);
	highNoonPressed = highNoonButton.IsPressed(mousePos, mousePressed);
	backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectGameMode::Draw()
{
	DrawText("SELECT GAME MODE", 110, 120, 40, WHITE);

	arenaButton.Draw();
	highNoonButton.Draw();
	backButton.Draw();
}