#pragma once
#include "raylib.h"
#include <string>

class Button
{
public:
    Button(const char* imagePath, Vector2 imagePosition, float scale);

    Button(Rectangle rect, const char* label, int fontSize);

    ~Button();

    void Draw();
    bool IsPressed(Vector2 mousePos, bool mousePressed);
    void SetColors(Color normalBg, Color hoverBg, Color normalBorder, Color hoverBorder, Color textColor);

private:
    bool useTexture = false;

    Texture2D texture{};
    Vector2 position{};

    Rectangle rect{};
    std::string label;
    int fontSize = 20;

    Color normalBg = GRAY;
    Color hoverBg = DARKGRAY;
    Color normalBorder = WHITE;
    Color hoverBorder = YELLOW;
    Color textColor = RAYWHITE;
};
