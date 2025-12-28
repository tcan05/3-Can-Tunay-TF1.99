#include "MainMenu.h"
#include "raylib.h"

MainMenu::MainMenu()
    : startButton({ 100, 200, 320, 60 }, "START", 30),
      controlsButton({ 100, 300, 320, 60 }, "CONTROLS", 30),
      exitButton({ 100, 510, 320, 60 }, "EXIT", 30)

{
    startPressed = false;
    controlsPressed = false;
    exitPressed = false;

	exitButton.SetColors(MAROON, RED, DARKGRAY, YELLOW, RAYWHITE);
}


void MainMenu::Update()
{
	Vector2 mousePos = GetMousePosition();
	bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	startPressed = startButton.IsPressed(mousePos, mousePressed);
	controlsPressed = controlsButton.IsPressed(mousePos, mousePressed);
	exitPressed = exitButton.IsPressed(mousePos, mousePressed);
}

void MainMenu::Draw()
{
	DrawText("TWO FORTS 1.99", 95, 120, 40, WHITE);

	startButton.Draw();
	controlsButton.Draw();
	exitButton.Draw();
}