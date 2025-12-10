#include "MainMenu.h"
#include "raylib.h"

MainMenu::MainMenu() : startButton("Assets/Start Green.png", { 100, 200 }, 0.8f),
					   controlsButton("Assets/Options.png", { 100, 300 }, 0.8f),
					   exitButton("Assets/Exit.png", { 100, 500 }, 0.8f)
{
	startPressed = false;
	controlsPressed = false;
	exitPressed = false;
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
	DrawText("MAIN MENU", 110, 120, 40, WHITE);

	startButton.Draw();
	controlsButton.Draw();
	exitButton.Draw();
}