#include "Button.h"

Button::Button(const char* imagePath, Vector2 imagePosition, float scale)
{
    useTexture = true;

    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int scaledWidth = static_cast<int>(originalWidth * scale);
    int scaledHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, scaledWidth, scaledHeight);

    texture = LoadTextureFromImage(image);
    UnloadImage(image);

    position = imagePosition;

    rect = { position.x, position.y, (float)texture.width, (float)texture.height };
}

Button::Button(Rectangle r, const char* text, int size)
{
    useTexture = false;
    rect = r;
    label = text;
    fontSize = size;
}

Button::~Button()
{
    if (useTexture)
    {
        UnloadTexture(texture);
    }
}

void Button::SetColors(Color normalBg, Color hoverBg, Color normalBorder, Color hoverBorder, Color textColor)
{
    this->normalBg = normalBg;
    this->hoverBg = hoverBg;
    this->normalBorder = normalBorder;
    this->hoverBorder = hoverBorder;
    this->textColor = textColor;
}


void Button::Draw()
{
    if (useTexture)
    {
        DrawTextureV(texture, position, WHITE);
        return;
    }

    Vector2 mousePos = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePos, rect);

    Color bg = hover ? hoverBg : normalBg;
    Color border = hover ? hoverBorder : normalBorder;

    DrawRectangleRec(rect, bg);
    DrawRectangleLinesEx(rect, 3, border);

    int textWidth = MeasureText(label.c_str(), fontSize);
    int textX = (int)(rect.x + (rect.width - textWidth) / 2);
    int textY = (int)(rect.y + (rect.height - fontSize) / 2);

    DrawText(label.c_str(), textX, textY, fontSize, RAYWHITE);
}

bool Button::IsPressed(Vector2 mousePos, bool mousePressed)
{
    return CheckCollisionPointRec(mousePos, rect) && mousePressed;
}
