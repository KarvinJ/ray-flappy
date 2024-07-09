#include "Player.h"

Player::Player(float positionX, float positionY)
{
    sprite = LoadTexture("assets/images/yellowbird-midflap.png");
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};
    impulse = -10000;
    gravity = 0;
    gravityIncrement = 400;
}

Player::~Player()
{
    UnloadTexture(sprite);
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
    // Check DrawTextureEx for the sprite rotation
    DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}