#include "raylib.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include "Snake.h"
#include "Food.h"
using namespace std;

void DrawBoard(int boardWidth, int boardHeight)
{
    DrawPlane({ 0.0f, 0.0f, 0.0f }, { (float)boardWidth, (float)boardHeight }, DARKGRAY);

    float startX = -boardWidth / 2.0f;
    float startZ = -boardHeight / 2.0f;

    for (int i = 0; i <= boardWidth; i++)
    {
        float x = startX + i;
        DrawLine3D({ x, 0.01f, startZ }, { x, 0.01f, startZ + boardHeight }, GRAY);
    }

    for (int i = 0; i <= boardHeight; i++)
    {
        float z = startZ + i;
        DrawLine3D({ startX, 0.01f, z }, { startX + boardWidth, 0.01f, z }, GRAY);
    }
}

int main()
{
    srand(time(0));

    InitWindow(1000, 700, "3D Snake OOP");
    InitAudioDevice();
    SetMasterVolume(1.0f);
    SetTargetFPS(60);

    Sound eatSound = LoadSound("eat.wav");
    Sound gameOverSound = LoadSound("gameover.wav");
    Music bgMusic = LoadMusicStream("bgmusic2.mp3");

    SetSoundVolume(eatSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
    SetMusicVolume(bgMusic, 0.4f);

    bool eatSoundLoaded = IsSoundValid(eatSound);
    bool gameOverSoundLoaded = IsSoundValid(gameOverSound);
    bool musicLoaded = IsMusicValid(bgMusic);

    if (musicLoaded)
    {
        PlayMusicStream(bgMusic);
    }

    const int boardWidth = 20;
    const int boardHeight = 20;

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 18.0f, 18.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Snake snake;
    Food food;

    float moveTimer = 0.0f;
    float baseSpeed = 0.2f;
    float moveDelay = baseSpeed;

    bool gameOver = false;
    bool deathSoundPlayed = false;
    int score = 0;

    while (!WindowShouldClose())
    {
        if (musicLoaded)
        {
            UpdateMusicStream(bgMusic);
        }

        if (gameOver && IsKeyPressed(KEY_R))
        {
            snake.Reset();
            food.Reset(boardWidth, boardHeight, snake.GetBody());
            score = 0;
            gameOver = false;
            deathSoundPlayed = false;
            moveDelay = baseSpeed;
        }

        if (!gameOver)
        {
            if (IsKeyPressed(KEY_RIGHT)) snake.SetDirection(4);
            if (IsKeyPressed(KEY_LEFT))  snake.SetDirection(3);
            if (IsKeyPressed(KEY_UP))    snake.SetDirection(1);
            if (IsKeyPressed(KEY_DOWN))  snake.SetDirection(2);

            moveTimer += GetFrameTime();

            if (moveTimer >= moveDelay)
            {
                snake.Move();
                moveTimer = 0.0f;

                if (snake.CheckWallCollision(boardWidth, boardHeight) || snake.CheckSelfCollision())
                {
                    gameOver = true;

                    if (!deathSoundPlayed && gameOverSoundLoaded)
                    {
                        StopSound(gameOverSound);
                        PlaySound(gameOverSound);
                        deathSoundPlayed = true;
                    }
                }

                if (!gameOver && snake.GetX() == food.GetX() && snake.GetZ() == food.GetZ())
                {
                    score += 10;
                    snake.Grow();
                    food.Respawn(boardWidth, boardHeight, snake.GetBody());

                    if (eatSoundLoaded)
                    {
                        StopSound(eatSound);
                        PlaySound(eatSound);
                    }

                    moveDelay = baseSpeed - (score * 0.002f);

                    if (moveDelay < 0.05f)
                    {
                        moveDelay = 0.05f;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawBoard(boardWidth, boardHeight);
        snake.Draw(boardWidth, boardHeight);
        food.Draw(boardWidth, boardHeight);

        EndMode3D();

        DrawText(TextFormat("Score: %d", score), 20, 20, 20, WHITE);

        if (!eatSoundLoaded)
        {
            DrawText("eat.wav not loaded", 20, 50, 20, RED);
        }

        if (!gameOverSoundLoaded)
        {
            DrawText("gameover.wav not loaded", 20, 80, 20, RED);
        }

        if (!musicLoaded)
        {
            DrawText("bgmusic.mp3 not loaded", 20, 110, 20, RED);
        }

        if (gameOver)
        {
            DrawText("GAME OVER", 380, 300, 40, RED);
            DrawText("Press R to Restart", 350, 350, 20, WHITE);
        }

        EndDrawing();
    }

    if (eatSoundLoaded) UnloadSound(eatSound);
    if (gameOverSoundLoaded) UnloadSound(gameOverSound);
    if (musicLoaded) UnloadMusicStream(bgMusic);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}