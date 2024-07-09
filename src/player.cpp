#include "player.h"

Player::Player(float positionX, float positionY, bool isPlayer1)
{
    bounds = Rectangle{positionX, positionY, 16 , 84};
    speed = 800;
    score = 0;
    this->isPlayer1 = isPlayer1;
}

void Player::Update(float deltaTime)
{
    if (isPlayer1)
    {
       //El origen en raylib sucede en la esquina superior izquierda, debido a esto debo restar para subir y sumar para bajar.
        if (IsKeyDown(KEY_W) && bounds.y >= 0)
            bounds.y -= speed * deltaTime;

        if (IsKeyDown(KEY_S) && bounds.y <= GetScreenHeight() - bounds.height)
            bounds.y += speed * deltaTime;
    }
    
    else
    {
        if (IsKeyDown(KEY_UP) && bounds.y >= 0)
            bounds.y -= speed * deltaTime;

        if (IsKeyDown(KEY_DOWN) && bounds.y <= GetScreenHeight() - bounds.height)
            bounds.y += speed * deltaTime;
    }
}

void Player::Draw()
{
    DrawRectangleRounded(bounds, 0.8, 0, WHITE);
}