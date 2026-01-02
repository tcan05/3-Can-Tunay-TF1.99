#include "SelectCredits.h"
#include "raylib.h"

SelectCredits::SelectCredits()
	: backButton({ 100, 510, 180, 60 }, "BACK", 30)
{
    backPressed = false;
}

void SelectCredits::Update()
{
	Vector2 mousePos = GetMousePosition();
	bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

	backPressed = backButton.IsPressed(mousePos, mousePressed);
}

void SelectCredits::Draw()
{
    DrawText("Credits", 100, 100, 50, WHITE);

	DrawText("Programming", 100, 200, 40, GOLD);
	DrawText("- Tunay Can", 100, 250, 30, WHITE);
	DrawText("- Abdullah G. H. Alamarah", 100, 290, 30, WHITE);
	DrawText("- M. Emir Yurt", 100, 330, 30, WHITE);
	DrawText("- Zeynep Ersal", 100, 370, 30, WHITE);

	/*DrawText("Programming - Tunay Can", 100, 200, 30, WHITE);
	DrawText(" - Abdullah G. H. Alamarah", 290, 240, 30, WHITE);
	DrawText(" - M. Emir Yurt", 290, 280, 30, WHITE);
	DrawText(" - Zeynep Ersal", 290, 320, 30, WHITE);*/

	DrawText("Textures ", 700, 200, 40, GOLD);
	DrawText("To Be Filled", 700, 250, 30, WHITE);

	DrawText("SFX", 700, 320, 40, GOLD);
	DrawText("To Be Filled", 700, 370, 30, WHITE);


    backButton.Draw();
}