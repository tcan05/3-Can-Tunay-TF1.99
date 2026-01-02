#include "MainMenu.h"
#include "raylib.h"

MainMenu::MainMenu()
    : startButton({ 100, 200, 320, 60 }, "START", 30),
      controlsButton({ 100, 290, 320, 60 }, "CONTROLS", 30),
	  creditsButton({ 100, 380, 320, 60 }, "CREDITS", 30),
      exitButton({ 100, 510, 320, 60 }, "EXIT", 30)

{
    startPressed = false;
    controlsPressed = false;
	creditsPressed = false;
    exitPressed = false;

	exitButton.SetColors(MAROON, RED, DARKGRAY, YELLOW, RAYWHITE);
}


void MainMenu::Update()
{
	Vector2 mousePos = GetMousePosition();
	bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	startPressed = startButton.IsPressed(mousePos, mousePressed);
	controlsPressed = controlsButton.IsPressed(mousePos, mousePressed);
	creditsPressed = creditsButton.IsPressed(mousePos, mousePressed);
	exitPressed = exitButton.IsPressed(mousePos, mousePressed);
}

void MainMenu::Draw()
{
	DrawText("TWO", 95, 120, 40, BLUE);
	DrawText("FORTS", 200, 120, 40, RED);
	DrawText("1", 350, 120, 40, BLUE);
	DrawText(".99", 363, 120, 40, RED);

	startButton.Draw();
	controlsButton.Draw();
	creditsButton.Draw();
	exitButton.Draw();
}