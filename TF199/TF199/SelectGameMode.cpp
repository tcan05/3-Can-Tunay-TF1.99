#include "SelectGameMode.h"
#include "raylib.h"

SelectGameMode::SelectGameMode()
    : arenaButton({ 100, 200, 420, 60 }, "ARENA", 30),
      highNoonButton({ 100, 300, 420, 60 }, "HIGH NOON", 30),
      backButton({ 100, 510, 180, 60 }, "BACK", 30)

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