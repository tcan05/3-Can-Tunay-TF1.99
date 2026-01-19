#pragma once
#include "Button.h"
#include "Map.h"

class SelectMap
{
public:
    SelectMap();

    void Update();
    void Draw();
    MapId GetSelectedMap() const;

    bool map1Pressed, map2Pressed, map3Pressed, map4Pressed, map5Pressed;
    bool randomPressed, backPressed;

private:
    Button map1Btn, map2Btn, map3Btn, map4Btn, map5Btn;
    Button randomBtn, backBtn;
};
