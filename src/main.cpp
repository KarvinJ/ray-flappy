#include <raylib.h>
#include "ball.h"
#include "player.h"

int main()
{
    Color darkGreen = Color{20, 160, 133, 255};
    Color green = Color{38, 185, 154, 255};
    Color lightGreen = Color{129, 204, 184, 255};

    const int screenWidth = 960;
    const int screenHeight = 640;

    InitWindow(screenWidth, screenHeight, "Pong!");
    SetTargetFPS(144);

    Player player = Player(10, screenHeight / 2, true);

    Ball ball = Ball(screenWidth / 2, screenHeight / 2);

    Player player2 = Player(screenWidth -26, screenHeight / 2, false);

    InitAudioDevice();      // Initialize audio device, before loading sound and music.

    Sound hitSound = LoadSound("assets/sounds/okay.wav");
    Music music = LoadMusicStream("assets/music/pixel3.mp3");

    music.looping = true;

    PlayMusicStream(music);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);

        float deltaTime = GetFrameTime();

        player.Update(deltaTime);
        ball.Update(deltaTime);
        player2.Update(deltaTime);

        //Check collision between a circle and a rectangle
        if (ball.HasCollideWithPlayer(player.bounds) || ball.HasCollideWithPlayer(player2.bounds)) 
        {
            ball.velocity.x *= -1;
            PlaySound(hitSound);
        }

        if (ball.position.x > 960)
        {
            player.score++;
            ball.ResetPosition();
        }

        else if (ball.position.x < 0)
        {
            player2.score++;
            ball.ResetPosition();
        }
        
        BeginDrawing();

            ClearBackground(darkGreen);

            //Right side color
            DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, green);

            // circle for the half of the court.
            DrawCircle(screenWidth/2, screenHeight/2, 150, lightGreen);

            DrawLine(screenWidth /2, screenHeight, screenWidth / 2, 0, WHITE);

            DrawText(TextFormat("%i", player.score), 230, 20, 80, WHITE);

            DrawText(TextFormat("%i", player2.score), screenWidth - 250, 20, 80, WHITE);

            player.Draw();
    
            ball.Draw();

            player2.Draw();

        EndDrawing();
    }

    // Unload sound data
    UnloadSound(hitSound);     
    UnloadMusicStream(music);

    CloseAudioDevice();     

    CloseWindow();
    return 0;
}