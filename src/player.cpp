#include "player.h"

Player::Player(float positionX, float positionY)
{
    bounds = Rectangle{positionX, positionY, 32, 32};
    impulse = 6000;
    gravity = 100;
}

void Player::Update(float deltaTime)
{
    bounds.y += gravity * deltaTime;

    if (IsKeyPressed(KEY_SPACE)) 
    {
        bounds.y -= impulse * deltaTime;
    }
}

void Player::Draw()
{
    DrawRectangleRec(bounds, WHITE);
}