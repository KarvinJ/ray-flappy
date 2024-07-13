#include "Player.h"

Player::Player(float positionX, float positionY)
{
    sprite = LoadTexture("assets/images/yellowbird-midflap.png");
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};
    flapSound = LoadSound("assets/sounds/wing.wav");
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

    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        gravity = impulse * deltaTime;
        PlaySound(flapSound);
    }
}