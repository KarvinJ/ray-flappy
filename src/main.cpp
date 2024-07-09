#include <raylib.h>
#include "player.h"

int main()
{
    const int screenWidth = 960;
    const int screenHeight = 544;

    InitWindow(screenWidth, screenHeight, "Flappy!");
    SetTargetFPS(60);

    Player player = Player(screenWidth / 2, screenHeight / 2);

    Texture2D background = LoadTexture("assets/images/background-day.png");

    Texture2D ground = LoadTexture("assets/images/base.png");

    InitAudioDevice();

    Sound hitSound = LoadSound("assets/sounds/okay.wav");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        player.Update(deltaTime);

        BeginDrawing();

        ClearBackground(Color{0, 0, 0, 0});

        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(background, background.width, 0, WHITE);
        DrawTexture(background, background.width * 2, 0, WHITE);
        DrawTexture(background, background.width * 3, 0, WHITE);

        DrawTexture(ground, 0, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width * 2, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width * 3, screenHeight - ground.height, WHITE);

        player.Draw();

        EndDrawing();
    }

    UnloadSound(hitSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}