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

Player::~Player()
{
    // UnloadTexture(sprite);
    // UnloadSound(flapSound);
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

void Player::Draw()
{
    DrawTexture(sprite, bounds.x, bounds.y, WHITE);

    // With this I can rotate sprite 45 degree and even increase the scale by 1.5.
    //Gonna comment this for the time being.
    // DrawTextureEx(sprite, {bounds.x, bounds.y}, 45, 1.5, WHITE);
}