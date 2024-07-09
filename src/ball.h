#pragma once

#include <raylib.h>

class Ball
{
public:
    Ball(float positionX, float positionY);
    void ResetPosition();
    void Update(float deltaTime);
    void Draw();
    bool HasCollideWithPlayer(Rectangle playerBounds);

public:
    Vector2 position;
    Vector2 velocity;
    int radius;
};