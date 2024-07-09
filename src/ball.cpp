#include "ball.h"

Ball::Ball(float positionX, float positionY)
{
    position = Vector2{positionX, positionY};
    velocity = Vector2{400, 400};
    radius = 15;
}

void Ball::ResetPosition()
{
    position.x = GetScreenWidth() / 2;
    position.y = GetScreenHeight() / 2;

    velocity.x *= -1;
    velocity.y *= -1;
}

void Ball::Update(float deltaTime)
{
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    if (position.y + radius >= GetScreenHeight() || position.y - radius <= 0)
        velocity.y *= -1;
}

void Ball::Draw()
{
    DrawCircle(position.x, position.y, radius, YELLOW);
}

bool Ball::HasCollideWithPlayer(Rectangle playerBounds)
{
    return CheckCollisionCircleRec(position, radius, playerBounds);
}