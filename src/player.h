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

    private:
        Texture2D sprite;
        float impulse;
        float gravity;
        float gravityIncrement;
};