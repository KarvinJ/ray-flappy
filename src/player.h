#pragma once

#include <raylib.h>

class Player
{
public:
    Player(float positionX, float positionY, bool isPlayer1);
    void Update(float deltaTime);
    void Draw();

public:
    Rectangle bounds;
    int speed;
    int score;
    bool isPlayer1;
};