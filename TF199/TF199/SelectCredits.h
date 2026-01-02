#pragma once
#include "Button.h"

class SelectCredits
{
public:
	SelectCredits();

	void Update();
	void Draw();

	bool backPressed;

private:
	Button backButton;
};

