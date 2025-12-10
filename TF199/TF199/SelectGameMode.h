#pragma once
#include "Button.h"

class SelectGameMode
{
public:
	SelectGameMode();

	void Update();
	void Draw();

	bool arenaPressed;
	bool highNoonPressed;
	bool backPressed;

private:
	Button arenaButton;
	Button highNoonButton;
	Button backButton;
};

