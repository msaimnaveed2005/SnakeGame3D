#include "raylib.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "Snake.h"
#include "Food.h"
using namespace std;

enum GameScreen
{
    MENU,
    PLAYING,
    SCOREBOARD
};

enum CameraOrientation
{
    DIAGONAL_VIEW,
    TOP_VIEW
};

void SaveScoreToFile(int score)
{
    ofstream file("scores.txt", ios::app);
    if (file.is_open())
    {
        file << score << endl;
        file.close();
    }
}

void LoadTopScores(int scores[], int& count)
{
    count = 0;

    ifstream file("scores.txt");
    if (!file.is_open())
    {
        return;
    }

    while (count < 10 && file >> scores[count])
    {
        count++;
    }

    file.close();

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j] > scores[i])
            {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

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

void ApplyCameraOrientation(Camera3D& camera, CameraOrientation orientation)
{
    if (orientation == DIAGONAL_VIEW)
    {
        camera.position = { 15.0f, 25.0f, 15.0f };
        camera.target = { 0.0f, 0.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }
    else if (orientation == TOP_VIEW)
    {
        camera.position = { 0.0f, 32.0f, 8.0f };
        camera.target = { 0.0f, 0.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
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
    CameraOrientation orientation = DIAGONAL_VIEW;
    ApplyCameraOrientation(camera, orientation);

    Snake snake;
    Food food;

    float moveTimer = 0.0f;
    float baseSpeed = 0.2f;
    float moveDelay = baseSpeed;

    bool gameOver = false;
    bool deathSoundPlayed = false;
    bool musicStopped = false;
    bool scoreSaved = false;
    int score = 0;

    GameScreen currentScreen = MENU;
    int selectedOption = 0; // 0 = Play Game, 1 = Visual Style, 2 = Orientation, 3 = Scoreboard
    bool modernLook = false;

    int topScores[10];
    int scoreCount = 0;

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
                selectedOption--;
                if (selectedOption < 0) selectedOption = 3;
            }

            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                selectedOption++;
                if (selectedOption > 3) selectedOption = 0;
            }

            if (selectedOption == 1 && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)))
            {
                modernLook = !modernLook;
            }

            if (selectedOption == 2 && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D)))
            {
                if (orientation == DIAGONAL_VIEW)
                    orientation = TOP_VIEW;
                else
                    orientation = DIAGONAL_VIEW;

                ApplyCameraOrientation(camera, orientation);
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (selectedOption == 0)
                {
                    snake.Reset();
                    food.Reset(boardWidth, boardHeight, snake.GetBody());
                    score = 0;
                    gameOver = false;
                    deathSoundPlayed = false;
                    musicStopped = false;
                    scoreSaved = false;
                    moveDelay = baseSpeed;

                    ApplyCameraOrientation(camera, orientation);

                    currentScreen = PLAYING;
                }
                else if (selectedOption == 1)
                {
                    modernLook = !modernLook;
                }
                else if (selectedOption == 2)
                {
                    if (orientation == DIAGONAL_VIEW)
                        orientation = TOP_VIEW;
                    else
                        orientation = DIAGONAL_VIEW;

                    ApplyCameraOrientation(camera, orientation);
                }
                else if (selectedOption == 3)
                {
                    LoadTopScores(topScores, scoreCount);
                    currentScreen = SCOREBOARD;
                }
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("3D SNAKE", 390, 110, 40, WHITE);
            DrawText("Menu", 460, 170, 25, LIGHTGRAY);

            Color playColor = (selectedOption == 0) ? YELLOW : WHITE;
            Color styleColor = (selectedOption == 1) ? YELLOW : WHITE;
            Color orientColor = (selectedOption == 2) ? YELLOW : WHITE;
            Color scoreColor = (selectedOption == 3) ? YELLOW : WHITE;

            DrawText("Play Game", 415, 240, 30, playColor);
            DrawText(TextFormat("Visual Style: %s", modernLook ? "Modern" : "Plain"), 320, 300, 30, styleColor);
            DrawText(TextFormat("Orientation: %s", orientation == DIAGONAL_VIEW ? "Diagonal" : "Top"), 340, 360, 30, orientColor);
            DrawText("Scoreboard", 400, 420, 30, scoreColor);

            DrawText("Use UP / DOWN to move", 350, 510, 20, GRAY);
            DrawText("Use LEFT / RIGHT or ENTER to change setting", 240, 540, 20, GRAY);
            DrawText("Press ENTER on Play Game or Scoreboard", 260, 570, 20, GRAY);

            EndDrawing();
            continue;
        }

        if (currentScreen == SCOREBOARD)
        {
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE))
            {
                currentScreen = MENU;
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("SCOREBOARD", 360, 80, 40, WHITE);

            if (scoreCount == 0)
            {
                DrawText("No scores yet", 410, 180, 25, GRAY);
            }
            else
            {
                int displayCount = (scoreCount > 5) ? 5 : scoreCount;

                for (int i = 0; i < displayCount; i++)
                {
                    DrawText(TextFormat("%d. %d", i + 1, topScores[i]), 430, 170 + i * 45, 28, LIGHTGRAY);
                }
            }

            DrawText("Press ESC or BACKSPACE for Menu", 290, 600, 20, GRAY);

            EndDrawing();
            continue;
        }

        if (musicLoaded && !musicStopped)
        {
            UpdateMusicStream(bgMusic);
        }

        if (gameOver && !scoreSaved)
        {
            SaveScoreToFile(score);
            scoreSaved = true;
        }

        if (gameOver && IsKeyPressed(KEY_R))
        {
            snake.Reset();
            food.Reset(boardWidth, boardHeight, snake.GetBody());
            score = 0;
            gameOver = false;
            deathSoundPlayed = false;
            musicStopped = false;
            scoreSaved = false;
            moveDelay = baseSpeed;

            ApplyCameraOrientation(camera, orientation);

            if (musicLoaded)
            {
                PlayMusicStream(bgMusic);
            }
        }

        if (IsKeyPressed(KEY_M))
        {
            currentScreen = MENU;
            gameOver = false;
            deathSoundPlayed = false;
            musicStopped = false;
            scoreSaved = false;

            if (musicLoaded && !IsMusicStreamPlaying(bgMusic))
            {
                PlayMusicStream(bgMusic);
            }
        }

        if (!gameOver)
        {
            if (modernLook)
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
        ClearBackground(modernLook ? Color{ 15, 15, 20, 255 } : BLACK);

        BeginMode3D(camera);

        DrawBoard(boardWidth, boardHeight);
        snake.Draw(boardWidth, boardHeight, modernLook);
        food.Draw(boardWidth, boardHeight, modernLook);

        EndMode3D();

        DrawText(TextFormat("Score: %d", score), 20, 20, 20, WHITE);
        DrawText(modernLook ? "Style: Modern" : "Style: Plain", 20, 50, 20, LIGHTGRAY);
        DrawText(orientation == DIAGONAL_VIEW ? "Orientation: Diagonal" : "Orientation: Top", 20, 80, 20, LIGHTGRAY);
        DrawText("Press M for Menu", 20, 110, 20, GRAY);

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