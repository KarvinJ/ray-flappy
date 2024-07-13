#include <raylib.h>
#include <vector>
#include "Player.h"

const int screenWidth = 960;
const int screenHeight = 544;

bool isGameOver;

Texture2D upPipeSprite;
Texture2D downPipeSprite;

typedef struct
{
    Texture2D sprite;
    Rectangle bounds;
    bool isDestroyed;
} Pipe;

std::vector<Pipe> pipes;

float lastPipeSpawnTime;

void GeneratePipes()
{
    float upPipePosition = GetRandomValue(-220, 0);

    Rectangle upPipeBounds = {screenWidth, upPipePosition, (float)upPipeSprite.width, (float)upPipeSprite.height};

    Pipe upPipe = {upPipeSprite, upPipeBounds, false};

    // gap size = 80.
    float downPipePosition = upPipePosition + upPipe.bounds.height + 80;

    Rectangle downPipeBounds = {screenWidth, downPipePosition, (float)downPipeSprite.width, (float)downPipeSprite.height};

    Pipe downPipe = {downPipeSprite, downPipeBounds, false};

    pipes.push_back(upPipe);
    pipes.push_back(downPipe);

    lastPipeSpawnTime = GetTime();
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Flappy!");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/images/background-day.png");
    Texture2D ground = LoadTexture("assets/images/base.png");

    // divide the birds.png
    //  Texture2D birds = LoadTexture("assets/images/birds.png");

    // Rectangle birdsBounds = {0.0f, 0.0f, (float)birds.width / 9, (float)birds.height};

    Rectangle groundBounds = {0, (float)(screenHeight - ground.height), screenWidth, (float)ground.height};

    upPipeSprite = LoadTexture("assets/images/pipe-green-180.png");
    downPipeSprite = LoadTexture("assets/images/pipe-green.png");

    InitAudioDevice();

    Sound dieSound = LoadSound("assets/sounds/die.wav");

    Player player = Player(screenWidth / 2, screenHeight / 2);

    // int framesCounter = 0;
    // int framesSpeed = 8;

    // int currentFrame = 0;

    while (!WindowShouldClose())
    {

        // framesCounter++;

        // if (framesCounter >= (60 / framesSpeed))
        // {
        //     framesCounter = 0;
        //     currentFrame++;

        //     if (currentFrame > 8)
        //         currentFrame = 0;

        //     birdsBounds.x = (float)currentFrame * (float)birdsBounds.width / 9;
        // }

        if (!isGameOver)
        {
            float deltaTime = GetFrameTime();

            player.Update(deltaTime);

            if (CheckCollisionRecs(player.bounds, groundBounds))
            {
                isGameOver = true;
                PlaySound(dieSound);
            }

            if (GetTime() - lastPipeSpawnTime >= 2)
            {
                GeneratePipes();
            }

            for (auto actualPipe = pipes.begin(); actualPipe != pipes.end();)
            {
                if (!actualPipe->isDestroyed)
                {
                    actualPipe->bounds.x -= 150 * deltaTime;
                }

                if (CheckCollisionRecs(player.bounds, actualPipe->bounds))
                {
                    isGameOver = true;
                    PlaySound(dieSound);
                }

                if (actualPipe->bounds.x < -actualPipe->bounds.width)
                {
                    actualPipe->isDestroyed = true;
                    pipes.erase(actualPipe);
                }
                else
                {
                    actualPipe++;
                }
            }
        }

        BeginDrawing();

        ClearBackground(Color{0, 0, 0, 0});

        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(background, background.width, 0, WHITE);
        DrawTexture(background, background.width * 2, 0, WHITE);
        DrawTexture(background, background.width * 3, 0, WHITE);

        for (Pipe pipe : pipes)
        {
            if (!pipe.isDestroyed)
            {
                DrawTexture(pipe.sprite, pipe.bounds.x, pipe.bounds.y, WHITE);
            }
        }

        DrawTexture(ground, 0, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width * 2, screenHeight - ground.height, WHITE);
        DrawTexture(ground, ground.width * 3, screenHeight - ground.height, WHITE);

        player.Draw();

        // DrawTextureRec(birds, birdsBounds, {screenWidth/2, screenHeight/2}, WHITE);

        EndDrawing();
    }

    UnloadTexture(upPipeSprite);
    UnloadTexture(downPipeSprite);
    UnloadSound(dieSound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}