#pragma once

#include <raylib.h>

class Player
{
    public:
        Player(float positionX, float positionY);
        void Update(float deltaTime);
        void Draw();

    public:
        Rectangle bounds;

    private:
        float impulse;
        float gravity;
        float gravityIncrement;
};