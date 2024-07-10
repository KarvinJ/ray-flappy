#include "Pipe.h"

Pipe::Pipe(float positionX, float positionY, Texture2D sprite)
{
    this->sprite = sprite; 
    bounds = Rectangle{positionX, positionY, (float)sprite.width, (float)sprite.height};
}

Pipe::~Pipe()
{
}

void Pipe::Update(float deltaTime)
{
    bounds.x -= 150 * deltaTime;
}

void Pipe::Draw()
{
    DrawTexture(sprite, bounds.x, bounds.y, WHITE);
}
