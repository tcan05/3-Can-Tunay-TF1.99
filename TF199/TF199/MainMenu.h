#pragma once
#include "raylib.h"
#include "Button.h"

class MainMenu
{
public:
	MainMenu();

	void Update();
	void Draw();

	bool startPressed;
	bool controlsPressed;
	bool exitPressed;

private:
	Button startButton;
	Button controlsButton;
	Button exitButton;
};

