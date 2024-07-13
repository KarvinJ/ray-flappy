#pragma once

#include <raylib.h>

class Player
{
    public:
        Player(float positionX, float positionY);
        void Update(float deltaTime);

    public:
        Rectangle bounds;
        float gravity;

    private:
        Texture2D sprite;
        Sound flapSound;
        float impulse;
        float gravityIncrement;
};