#include <raylib.h>
#include "player.h"

int main()
{
    const int screenWidth = 960;
    const int screenHeight = 544;

    InitWindow(screenWidth, screenHeight, "Flappy!");
    SetTargetFPS(60);

    Player player = Player(screenWidth / 2, screenHeight / 2);

    InitAudioDevice();

    Sound hitSound = LoadSound("assets/sounds/okay.wav");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        BeginDrawing();

        ClearBackground(Color{0, 0, 0, 0});

        player.Draw();

        EndDrawing();
    }

    UnloadSound(hitSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}