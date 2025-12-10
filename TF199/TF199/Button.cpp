#include "Button.h"

Button::Button(const char* imagePath, Vector2 imagePosition, float scale)
{
	Image image = LoadImage(imagePath);

	int originalWidth = image.width;
	int originalHeight = image.height;

	int scaledWidth = static_cast<int>(originalWidth * scale);
	int scaledHeight = static_cast<int>(originalHeight * scale);

	ImageResize(&image, scaledWidth, scaledHeight);

	texture = LoadTextureFromImage(image);
	UnloadImage(image);

	position = imagePosition;

}

Button::~Button()
{
	UnloadTexture(texture);
}

void Button::Draw()
{
	DrawTextureV(texture, position, WHITE);
}

bool Button::IsPressed(Vector2 mousePos, bool mousePressed)
{
	Rectangle rect = { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };

	if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
	{
		return true;
	}
	else
	{
		return false;
	}
};