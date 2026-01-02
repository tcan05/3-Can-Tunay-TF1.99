#pragma once
#include "Button.h"

class MainMenu
{
public:
	MainMenu();

	void Update();
	void Draw();

	bool startPressed;
	bool controlsPressed;
	bool creditsPressed;
	bool exitPressed;

private:
	Button startButton;
	Button controlsButton;
	Button creditsButton;
	Button exitButton;
};

