#include <raylib.h>
#include <vector>
#include <iostream>
#include <fstream>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

bool isGameOver;
bool isGamePaused;
float startGameTimer;

int score;
float initialAngle;
int highScore;

float impulse = -10000;
float gravity;
float gravityIncrement = 400;

Sound dieSound;
Sound crossPipeSound;
Sound flapSound;

std::vector<Vector2> groundPositions;

Rectangle groundCollisionBounds;

Texture2D upPipeSprite;
Texture2D downPipeSprite;
Texture2D groundSprite;

typedef struct
{
    Rectangle bounds;
    Texture2D sprite;
} Player;

Player player;

typedef struct
{
    Texture2D sprite;
    Rectangle bounds;
    bool isBehind;
    bool isDestroyed;
} Pipe;

std::vector<Pipe> pipes;

float lastPipeSpawnTime;

void generatePipes()
{
    float upPipePosition = GetRandomValue(-220, 0);

    Rectangle upPipeBounds = {SCREEN_WIDTH, upPipePosition, (float)upPipeSprite.width, (float)upPipeSprite.height};

    Pipe upPipe = {upPipeSprite, upPipeBounds, false, false};

    // gap size = 80.
    float downPipePosition = upPipePosition + upPipe.bounds.height + 80;

    Rectangle downPipeBounds = {SCREEN_WIDTH, downPipePosition, (float)downPipeSprite.width, (float)downPipeSprite.height};

    Pipe downPipe = {downPipeSprite, downPipeBounds, false, false};

    pipes.push_back(upPipe);
    pipes.push_back(downPipe);

    lastPipeSpawnTime = GetTime();
}

int loadHighScore()
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

void saveHighScore()
{
    std::ofstream highScores("high-score.txt");

    std::string scoreString = std::to_string(score);
    // Write to the file
    highScores << scoreString;

    // Close the file
    highScores.close();
}

void resetGame()
{
    if (score > highScore)
    {
        saveHighScore();
    }

    highScore = loadHighScore();

    isGameOver = false;
    score = 0;
    startGameTimer = 0;
    initialAngle = 0;
    player.bounds.x = SCREEN_WIDTH / 2;
    player.bounds.y = SCREEN_HEIGHT / 2;
    gravity = 0;
    pipes.clear();
}

void update(float deltaTime)
{
    if (player.bounds.y < SCREEN_HEIGHT - player.bounds.width)
    {
        player.bounds.y += gravity * deltaTime;
        gravity += gravityIncrement * deltaTime;
    }

    if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        gravity = impulse * deltaTime;
        PlaySound(flapSound);
    }

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
        generatePipes();
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

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy!");
    SetTargetFPS(60);

    highScore = loadHighScore();

    Texture2D startGameBackground = LoadTexture("assets/images/message.png");
    Texture2D background = LoadTexture("assets/images/background-day.png");
    groundSprite = LoadTexture("assets/images/base.png");

    const float GROUND_Y_POSITION = SCREEN_HEIGHT - groundSprite.height;

    groundCollisionBounds = {0, GROUND_Y_POSITION, SCREEN_WIDTH, (float)groundSprite.height};

    groundPositions.push_back({0, GROUND_Y_POSITION});
    groundPositions.push_back({(float)groundSprite.width, GROUND_Y_POSITION});
    groundPositions.push_back({(float)groundSprite.width * 2, GROUND_Y_POSITION});
    groundPositions.push_back({(float)groundSprite.width * 3, GROUND_Y_POSITION});

    upPipeSprite = LoadTexture("assets/images/pipe-green-180.png");
    downPipeSprite = LoadTexture("assets/images/pipe-green.png");

    InitAudioDevice();

    Sound pauseSound = LoadSound("assets/sounds/magic.wav");
    dieSound = LoadSound("assets/sounds/die.wav");
    crossPipeSound = LoadSound("assets/sounds/point.wav");
    flapSound = LoadSound("assets/sounds/wing.wav");

    Texture2D playerSprite = LoadTexture("assets/images/yellowbird-midflap.png");
    Rectangle bounds = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)playerSprite.width, (float)playerSprite.height};
    player = {bounds, playerSprite};

    std::vector<Texture2D> numbers;
    numbers.reserve(10);

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
        if (IsKeyPressed(KEY_F))
        {
            isGamePaused = !isGamePaused;
            lastPipeSpawnTime = 0;
            PlaySound(pauseSound);
        }

        if (score == 99)
        {
            isGameOver = true;
        }

        if (!isGameOver && !isGamePaused)
        {
            // Sprite animation
            framesCounter++;

            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 2)
                {
                    currentFrame = 0;
                }

                birdsBounds.x = (float)currentFrame * (float)birdSprites.width / 3;
            }
        }

        float deltaTime = GetFrameTime();

        startGameTimer += deltaTime;

        if (!isGameOver && !isGamePaused && startGameTimer > 1)
        {
            update(deltaTime);
        }

        else if (isGameOver && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE)))
        {
            resetGame();
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
            DrawTexture(numbers[score], SCREEN_WIDTH / 2, 30, WHITE);
        }
        else
        {
            int tens = (int)(score / 10);
            int units = (int)(score % 10);
            DrawTexture(numbers[tens], SCREEN_WIDTH / 2 - 20, 30, WHITE);
            DrawTexture(numbers[units], SCREEN_WIDTH / 2, 30, WHITE);
        }

        // adding this extra rendering sprite to hide the little space between grounds in the parallax effect.
        DrawTexture(groundSprite, 0, GROUND_Y_POSITION, WHITE);
        DrawTexture(groundSprite, groundSprite.width, GROUND_Y_POSITION, WHITE);
        DrawTexture(groundSprite, groundSprite.width * 2, GROUND_Y_POSITION, WHITE);
        DrawTexture(groundSprite, groundSprite.width * 3, GROUND_Y_POSITION, WHITE);

        for (Vector2 groundPosition : groundPositions)
        {
            DrawTextureV(groundSprite, groundPosition, WHITE);
        }

        if (isGameOver)
        {
            DrawTexture(startGameBackground, SCREEN_WIDTH / 2 - 75, 103, WHITE);
        }

        if (startGameTimer > 1)
        {
            downRotationTimer += deltaTime;

            if (downRotationTimer < 0.5f)
            {
                DrawTexturePro(birdSprites, birdsBounds, player.bounds, {0, 0}, initialAngle, WHITE);
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

                DrawTexturePro(birdSprites, birdsBounds, player.bounds, {0, 0}, initialAngle, WHITE);
            }

            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                shouldRotateUp = true;
                upRotationTimer = 1;
                downRotationTimer = 0;
                initialAngle = -20;
            }

            if (downRotationTimer > 0.5f)
            {
                if (initialAngle <= 90 && !isGameOver && !isGamePaused)
                {
                    initialAngle += 2;
                }

                DrawTexturePro(birdSprites, birdsBounds, player.bounds, {0, 0}, initialAngle, WHITE);
            }
        }
        else
        {
            DrawTexturePro(birdSprites, birdsBounds, player.bounds, {0, 0}, 0, WHITE);
        }

        EndDrawing();
    }

    for (Texture2D number : numbers)
    {
        UnloadTexture(number);
    }

    UnloadTexture(playerSprite);
    UnloadTexture(birdSprites);
    UnloadTexture(upPipeSprite);
    UnloadTexture(downPipeSprite);
    
    UnloadSound(dieSound);
    UnloadSound(flapSound);
    UnloadSound(pauseSound);
    UnloadSound(crossPipeSound);

    CloseAudioDevice();
    CloseWindow();
}