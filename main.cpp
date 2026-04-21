#include "raylib.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include "Snake.h"
#include "Food.h"
using namespace std;

enum GameScreen
{
    MENU,
    PLAYING
};

void DrawBoard(int boardWidth, int boardHeight, bool premiumMode)
{
    if (premiumMode)
    {
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { (float)boardWidth, (float)boardHeight }, Color{ 30, 30, 35, 255 });

        float startX = -boardWidth / 2.0f;
        float startZ = -boardHeight / 2.0f;

        for (int i = 0; i <= boardWidth; i++)
        {
            float x = startX + i;
            DrawLine3D({ x, 0.01f, startZ }, { x, 0.01f, startZ + boardHeight }, Color{ 60, 60, 70, 255 });
        }

        for (int i = 0; i <= boardHeight; i++)
        {
            float z = startZ + i;
            DrawLine3D({ startX, 0.01f, z }, { startX + boardWidth, 0.01f, z }, Color{ 60, 60, 70, 255 });
        }

        DrawCubeWires({ 0, 0.5f, 0 }, (float)boardWidth, 1.0f, (float)boardHeight, Color{ 120, 120, 140, 255 });
    }
    else
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
    Music bgMusic = LoadMusicStream("bgmusic.mp3");

    SetSoundVolume(eatSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
    SetMusicVolume(bgMusic, 0.4f);

    bool eatSoundLoaded = IsSoundValid(eatSound);
    bool gameOverSoundLoaded = IsSoundValid(gameOverSound);
    bool musicLoaded = IsMusicValid(bgMusic);

    const int boardWidth = 20;
    const int boardHeight = 20;

    Camera3D camera = { 0 };
    camera.position = { 15.0f, 25.0f, 15.0f };
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
    bool musicStopped = false;
    int score = 0;

    GameScreen currentScreen = MENU;
    int selectedOption = 0; // 0 = Simple, 1 = Premium
    bool premiumMode = false;

    while (!WindowShouldClose())
    {
        if (currentScreen == MENU)
        {
            if (musicLoaded && !IsMusicStreamPlaying(bgMusic))
            {
                PlayMusicStream(bgMusic);
                musicStopped = false;
            }

            if (musicLoaded && !musicStopped)
            {
                UpdateMusicStream(bgMusic);
            }

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                selectedOption = 0;
            }

            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                selectedOption = 1;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                premiumMode = (selectedOption == 1);

                snake.Reset();
                food.Reset(boardWidth, boardHeight, snake.GetBody());
                score = 0;
                gameOver = false;
                deathSoundPlayed = false;
                moveDelay = baseSpeed;

                currentScreen = PLAYING;
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("3D SNAKE", 390, 130, 40, WHITE);
            DrawText("Choose Game Mode", 350, 200, 25, LIGHTGRAY);

            Color simpleColor = (selectedOption == 0) ? YELLOW : WHITE;
            Color premiumColor = (selectedOption == 1) ? YELLOW : WHITE;

            DrawText("Simple Mode", 400, 280, 30, simpleColor);
            DrawText("Premium Mode", 390, 340, 30, premiumColor);

            DrawText("Use UP / DOWN and press ENTER", 300, 430, 20, GRAY);

            if (!eatSoundLoaded)
            {
                DrawText("eat.wav not loaded", 20, 20, 20, RED);
            }

            if (!gameOverSoundLoaded)
            {
                DrawText("gameover.wav not loaded", 20, 50, 20, RED);
            }

            if (!musicLoaded)
            {
                DrawText("bgmusic.mp3 not loaded", 20, 80, 20, RED);
            }

            EndDrawing();
            continue;
        }

        if (musicLoaded && !musicStopped)
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

            if (musicLoaded)
            {
                PlayMusicStream(bgMusic);
                musicStopped = false;
            }
        }

        if (IsKeyPressed(KEY_M))
        {
            currentScreen = MENU;
            gameOver = false;
            deathSoundPlayed = false;

            if (musicLoaded && !IsMusicStreamPlaying(bgMusic))
            {
                PlayMusicStream(bgMusic);
                musicStopped = false;
            }
        }

        if (!gameOver)
        {
            if (premiumMode)
            {
                food.Update();
            }

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

                    if (!musicStopped && musicLoaded)
                    {
                        StopMusicStream(bgMusic);
                        musicStopped = true;
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
        ClearBackground(premiumMode ? Color{ 15, 15, 20, 255 } : BLACK);

        BeginMode3D(camera);

        DrawBoard(boardWidth, boardHeight, premiumMode);
        snake.Draw(boardWidth, boardHeight, premiumMode);
        food.Draw(boardWidth, boardHeight, premiumMode);

        EndMode3D();

        DrawText(TextFormat("Score: %d", score), 20, 20, 20, WHITE);
        DrawText(premiumMode ? "Mode: Premium" : "Mode: Simple", 20, 50, 20, LIGHTGRAY);
        DrawText("Press M for Menu", 20, 80, 20, GRAY);

        if (!eatSoundLoaded)
        {
            DrawText("eat.wav not loaded", 20, 110, 20, RED);
        }

        if (!gameOverSoundLoaded)
        {
            DrawText("gameover.wav not loaded", 20, 140, 20, RED);
        }

        if (!musicLoaded)
        {
            DrawText("bgmusic.mp3 not loaded", 20, 170, 20, RED);
        }

        if (gameOver)
        {
            DrawText("GAME OVER", 380, 300, 40, RED);
            DrawText("Press R to Restart", 350, 350, 20, WHITE);
            DrawText("Press M for Menu", 360, 380, 20, LIGHTGRAY);
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