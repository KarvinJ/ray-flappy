#pragma once

#include <raylib.h>

class Player
{
    public:
        Player(float positionX, float positionY);
        ~Player();
        void Update(float deltaTime);
        void Draw();

    public:
        Rectangle bounds;
        float gravity;

    private:
        Texture2D sprite;
        Sound flapSound;
        float impulse;
        float gravityIncrement;
};