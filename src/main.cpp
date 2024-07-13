#include <raylib.h>
#include <vector>
#include "Player.h"

const int screenWidth = 960;
const int screenHeight = 544;

bool isGameOver;
float startGameTimer;

int score;

std::vector<Vector2> groundPositions;

Texture2D upPipeSprite;
Texture2D downPipeSprite;

typedef struct
{
    Texture2D sprite;
    Rectangle bounds;
    bool isBehind;
    bool isDestroyed;
} Pipe;

std::vector<Pipe> pipes;

float lastPipeSpawnTime;

void GeneratePipes()
{
    float upPipePosition = GetRandomValue(-220, 0);

    Rectangle upPipeBounds = {screenWidth, upPipePosition, (float)upPipeSprite.width, (float)upPipeSprite.height};

    Pipe upPipe = {upPipeSprite, upPipeBounds, false, false};

    // gap size = 80.
    float downPipePosition = upPipePosition + upPipe.bounds.height + 80;

    Rectangle downPipeBounds = {screenWidth, downPipePosition, (float)downPipeSprite.width, (float)downPipeSprite.height};

    Pipe downPipe = {downPipeSprite, downPipeBounds, false, false};

    pipes.push_back(upPipe);
    pipes.push_back(downPipe);

    lastPipeSpawnTime = GetTime();
}

void ResetGame(Player &player)
{
    isGameOver = false;
    score = 0;
    startGameTimer = 0;
    player.bounds.x = screenWidth / 2;
    player.bounds.y = screenHeight / 2;
    pipes.clear();
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Flappy!");
    SetTargetFPS(60);

    Texture2D startGameBackground = LoadTexture("assets/images/message.png");
    Texture2D background = LoadTexture("assets/images/background-day.png");
    Texture2D groundSprite = LoadTexture("assets/images/base.png");

    const float groundYPosition = screenHeight - groundSprite.height;

    Rectangle groundCollisionBounds = {0, groundYPosition, screenWidth, (float)groundSprite.height};

    groundPositions.push_back({0, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width * 2, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width * 3, groundYPosition});

    // divide the birds.png
    //  Texture2D birds = LoadTexture("assets/images/birds.png");

    // Rectangle birdsBounds = {0.0f, 0.0f, (float)birds.width / 9, (float)birds.height};

    upPipeSprite = LoadTexture("assets/images/pipe-green-180.png");
    downPipeSprite = LoadTexture("assets/images/pipe-green.png");

    InitAudioDevice();

    Sound dieSound = LoadSound("assets/sounds/die.wav");
    Sound crossPipeSound = LoadSound("assets/sounds/point.wav");

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

        float deltaTime = GetFrameTime();

        startGameTimer += deltaTime;

        if (!isGameOver && startGameTimer > 1)
        {
            player.Update(deltaTime);

            if (CheckCollisionRecs(player.bounds, groundCollisionBounds))
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

                if (!actualPipe->isBehind && player.bounds.x > actualPipe->bounds.x)
                {
                    actualPipe->isBehind = true;

                    // adding score for just one pipe, without this I got +2 instead of one and double the sound effect.
                    if (actualPipe->bounds.y < player.bounds.y)
                    {
                        score++;
                        PlaySound(crossPipeSound);
                    }
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

            for (Vector2 &groundPosition : groundPositions)
            {
                groundPosition.x -= 150 * deltaTime;

                if (groundPosition.x < -groundSprite.width)
                {
                    groundPosition.x = groundSprite.width * 3;
                }
            }
        }

        else if (isGameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ResetGame(player);
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

        DrawText(TextFormat("%i", score), screenWidth / 2, 40, 36, WHITE);

        //adding this extra rendering sprite to hide the little space between grounds in the parallax effect.
        DrawTexture(groundSprite, 0, groundYPosition, WHITE);
        DrawTexture(groundSprite, groundSprite.width, groundYPosition, WHITE);
        DrawTexture(groundSprite, groundSprite.width * 2, groundYPosition, WHITE);
        DrawTexture(groundSprite, groundSprite.width * 3, groundYPosition, WHITE);

        for (Vector2 groundPosition : groundPositions)
        {
            DrawTextureV(groundSprite, groundPosition, WHITE);
        }

        if (isGameOver)
        {
            DrawTexture(startGameBackground, screenWidth / 2 - 75, 103, WHITE);
        }

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