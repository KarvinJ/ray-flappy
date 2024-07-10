#pragma once

#include <raylib.h>

class Pipe
{
    public:
        Pipe(float positionX, float positionY, Texture2D sprite);
        ~Pipe();
        void Update(float deltaTime);
        void Draw();

    public:
        Rectangle bounds;

    private:
        Texture2D sprite;
};