#include <raylib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Player.h"

const int screenWidth = 960;
const int screenHeight = 544;

bool isGameOver;
float startGameTimer;

int score = 0;
float initialAngle = 0;
int highScore;

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

int LoadHighScore()
{
    std::string highScoreText;

    // Read from the text file
    std::ifstream highScores("high-score.txt");

    // read the firstLine of the file and store the string data in my variable highScoreText.
    getline(highScores, highScoreText);

    // Close the file
    highScores.close();

    int highScore = stoi(highScoreText);

    return highScore;
}

void SaveScore()
{
    std::ofstream highScores("high-score.txt");

    std::string scoreString = std::to_string(score);
    // Write to the file
    highScores << scoreString;

    // Close the file
    highScores.close();
}

void ResetGame(Player &player)
{
    if (score > highScore)
    {
        SaveScore();
    }

    highScore = LoadHighScore();

    isGameOver = false;
    score = 0;
    startGameTimer = 0;
    initialAngle = 0;
    player.bounds.x = screenWidth / 2;
    player.bounds.y = screenHeight / 2;
    player.gravity = 0;
    pipes.clear();
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Flappy!");
    SetTargetFPS(60);

    highScore = LoadHighScore();

    Texture2D startGameBackground = LoadTexture("assets/images/message.png");
    Texture2D background = LoadTexture("assets/images/background-day.png");
    Texture2D groundSprite = LoadTexture("assets/images/base.png");

    const float groundYPosition = screenHeight - groundSprite.height;

    Rectangle groundCollisionBounds = {0, groundYPosition, screenWidth, (float)groundSprite.height};

    groundPositions.push_back({0, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width * 2, groundYPosition});
    groundPositions.push_back({(float)groundSprite.width * 3, groundYPosition});

    upPipeSprite = LoadTexture("assets/images/pipe-green-180.png");
    downPipeSprite = LoadTexture("assets/images/pipe-green.png");

    InitAudioDevice();

    Sound dieSound = LoadSound("assets/sounds/die.wav");
    Sound crossPipeSound = LoadSound("assets/sounds/point.wav");

    Player player = Player(screenWidth / 2, screenHeight / 2);

    std::vector<Texture2D> numbers;

    std::string baseString = "assets/images/";
    std::string fileExtension = ".png";

    for (int i = 0; i < 10; i++)
    {
        std::string completeString = baseString + std::to_string(i) + fileExtension;

        numbers.push_back(LoadTexture(completeString.c_str()));
    }

    Texture2D birdSprites = LoadTexture("assets/images/yellow-bird.png");

    Rectangle birdsBounds = {0, 0, (float)birdSprites.width / 3, (float)birdSprites.height};

    int framesCounter = 0;
    int framesSpeed = 6;

    int currentFrame = 0;

    bool shouldRotateUp = false;
    float downRotationTimer = 0;
    float upRotationTimer = 0;

    while (!WindowShouldClose())
    {

        if (!isGameOver)
        {
            // Sprite animation
            framesCounter++;

            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 2)
                    currentFrame = 0;

                birdsBounds.x = (float)currentFrame * (float)birdSprites.width / 3;
            }
        }

        if (score == 99)
        {
            isGameOver = true;
        }

        float deltaTime = GetFrameTime();

        startGameTimer += deltaTime;

        if (!isGameOver && startGameTimer > 1)
        {
            player.Update(deltaTime);

            if (player.bounds.y < -player.bounds.height)
            {
                isGameOver = true;
                PlaySound(dieSound);
            }

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

        ClearBackground(BLACK);

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

        DrawText(TextFormat("High Score: "), 20, 30, 36, WHITE);

        if (highScore < 10)
        {
            DrawTexture(numbers[highScore], 245, 30, WHITE);
        }
        else
        {
            int tens = (int)(highScore / 10);
            int units = (int)(highScore % 10);
            DrawTexture(numbers[tens], 225, 30, WHITE);
            DrawTexture(numbers[units], 245, 30, WHITE);
        }

        if (score < 10)
        {
            DrawTexture(numbers[score], screenWidth / 2, 30, WHITE);
        }
        else
        {
            int tens = (int)(score / 10);
            int units = (int)(score % 10);
            DrawTexture(numbers[tens], screenWidth / 2 - 20, 30, WHITE);
            DrawTexture(numbers[units], screenWidth / 2, 30, WHITE);
        }

        // adding this extra rendering sprite to hide the little space between grounds in the parallax effect.
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

        if (startGameTimer > 1)
        {
            downRotationTimer += deltaTime;

            if (downRotationTimer < 0.5f)
            {
                DrawTexturePro(birdSprites, birdsBounds, {screenWidth / 2, player.bounds.y, player.bounds.width, player.bounds.height}, {0, 0}, initialAngle, WHITE);
            }

            if (shouldRotateUp)
            {
                if (upRotationTimer > 0)
                {
                    upRotationTimer -= deltaTime;
                }

                if (upRotationTimer <= 0)
                {
                    shouldRotateUp = false;
                }

                DrawTexturePro(birdSprites, birdsBounds, {screenWidth / 2, player.bounds.y, player.bounds.width, player.bounds.height}, {0, 0}, initialAngle, WHITE);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                shouldRotateUp = true;
                upRotationTimer = 1;
                downRotationTimer = 0;
                initialAngle = -20;
            }

            if (downRotationTimer > 0.5f)
            {
                if (initialAngle <= 90 && !isGameOver)
                {
                    initialAngle += 2;
                }

                DrawTexturePro(birdSprites, birdsBounds, {screenWidth / 2, player.bounds.y, player.bounds.width, player.bounds.height}, {0, 0}, initialAngle, WHITE);
            }
        }
        else
        {
            DrawTexturePro(birdSprites, birdsBounds, {screenWidth / 2, player.bounds.y, player.bounds.width, player.bounds.height}, {0, 0}, 0, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(upPipeSprite);
    UnloadTexture(downPipeSprite);
    UnloadSound(dieSound);
    CloseAudioDevice();

    CloseWindow();
}