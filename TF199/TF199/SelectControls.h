#pragma once
#include "Button.h"

class SelectControls
{
public:
	SelectControls();

	void Update();
	void Draw();

	bool backPressed;

private:
	Button backButton;
};

