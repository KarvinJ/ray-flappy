#include "player.h"

Player::Player(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 32, 32};
    impulse = -10000;
    gravity = 0;
    gravityIncrement = 400;
}

void Player::Update(float deltaTime)
{
    if (bounds.y < GetScreenHeight() - bounds.width)
    {
        bounds.y += gravity * deltaTime;
        gravity += gravityIncrement * deltaTime;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        gravity = impulse * deltaTime;
    }
}

void Player::Draw()
{
    DrawRectangleRec(bounds, WHITE);
}