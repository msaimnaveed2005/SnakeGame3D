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
    SCOREBOARD,
    NAME_INPUT
};

enum CameraOrientation
{
    DIAGONAL_VIEW,
    TOP_VIEW
};

void SaveScoreToFile(const string& playerName, int score)
{
    ofstream file("scores.txt", ios::app);
    if (file.is_open())
    {
        file << playerName << " " << score << endl;
        file.close();
    }
}

void LoadTopScores(string names[], int scores[], int& count)
{
    count = 0;

    ifstream file("scores.txt");
    if (!file.is_open())
    {
        return;
    }

    while (count < 10 && file >> names[count] >> scores[count])
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
                int tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;

                string tempName = names[i];
                names[i] = names[j];
                names[j] = tempName;
            }
        }
    }
}

void DrawBoard(int boardWidth, int boardHeight, bool modernLook)
{
    DrawPlane({ 0.0f, 0.0f, 0.0f }, { (float)boardWidth, (float)boardHeight }, modernLook ? Color{ 38, 38, 45, 255 } : DARKGRAY);

    float startX = -boardWidth / 2.0f;
    float startZ = -boardHeight / 2.0f;

    for (int i = 0; i <= boardWidth; i++)
    {
        float x = startX + i;
        DrawLine3D({ x, 0.01f, startZ }, { x, 0.01f, startZ + boardHeight }, modernLook ? Color{ 85, 85, 95, 255 } : GRAY);
    }

    for (int i = 0; i <= boardHeight; i++)
    {
        float z = startZ + i;
        DrawLine3D({ startX, 0.01f, z }, { startX + boardWidth, 0.01f, z }, modernLook ? Color{ 85, 85, 95, 255 } : GRAY);
    }

    if (modernLook)
    {
        DrawCubeWires({ 0.0f, 0.6f, 0.0f }, (float)boardWidth, 1.2f, (float)boardHeight, Color{ 150, 150, 175, 255 });

        float halfW = boardWidth / 2.0f;
        float halfH = boardHeight / 2.0f;

        DrawCube({ -halfW - 0.7f, 1.5f, -halfH - 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ halfW + 0.7f, 1.5f, -halfH - 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ -halfW - 0.7f, 1.5f,  halfH + 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ halfW + 0.7f, 1.5f,  halfH + 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });

        DrawCube({ 0.0f, -0.35f, 0.0f }, (float)boardWidth + 1.0f, 0.2f, (float)boardHeight + 1.0f, Color{ 20, 20, 25, 255 });
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
    else
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
    srand((unsigned int)time(0));

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
    int selectedOption = 0; // 0 Play, 1 Style, 2 Orientation, 3 Scoreboard
    bool modernLook = false;

    string playerName = "";
    string tempName = "";

    string topNames[10];
    int topScores[10];
    int scoreCount = 0;

    while (!WindowShouldClose())
    {
        if (currentScreen == MENU)
        {
            if (musicLoaded && !musicStopped)
            {
                if (!IsMusicStreamPlaying(bgMusic))
                {
                    PlayMusicStream(bgMusic);
                }
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
                orientation = (orientation == DIAGONAL_VIEW) ? TOP_VIEW : DIAGONAL_VIEW;
                ApplyCameraOrientation(camera, orientation);
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (selectedOption == 0)
                {
                    if (playerName.empty())
                    {
                        tempName = "";
                        currentScreen = NAME_INPUT;
                    }
                    else
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
                }
                else if (selectedOption == 1)
                {
                    modernLook = !modernLook;
                }
                else if (selectedOption == 2)
                {
                    orientation = (orientation == DIAGONAL_VIEW) ? TOP_VIEW : DIAGONAL_VIEW;
                    ApplyCameraOrientation(camera, orientation);
                }
                else if (selectedOption == 3)
                {
                    LoadTopScores(topNames, topScores, scoreCount);
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

            if (!playerName.empty())
            {
                DrawText(TextFormat("Player: %s", playerName.c_str()), 380, 470, 24, LIGHTGRAY);
            }

            DrawText("Use UP / DOWN to move", 350, 520, 20, GRAY);
            DrawText("Use LEFT / RIGHT or ENTER to change setting", 240, 550, 20, GRAY);
            DrawText("Press ENTER on Play Game or Scoreboard", 260, 580, 20, GRAY);

            EndDrawing();
            continue;
        }

        if (currentScreen == NAME_INPUT)
        {
            if (musicLoaded && !musicStopped)
            {
                if (!IsMusicStreamPlaying(bgMusic))
                {
                    PlayMusicStream(bgMusic);
                }
                UpdateMusicStream(bgMusic);
            }

            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && tempName.length() < 18)
                {
                    tempName.push_back((char)key);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !tempName.empty())
            {
                tempName.pop_back();
            }

            if (IsKeyPressed(KEY_ENTER) && !tempName.empty())
            {
                playerName = tempName;

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

            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentScreen = MENU;
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("ENTER YOUR NAME", 315, 180, 35, WHITE);
            DrawRectangleLines(250, 280, 500, 60, LIGHTGRAY);
            DrawText(tempName.c_str(), 270, 295, 30, YELLOW);

            DrawText("Press ENTER to continue", 350, 390, 20, GRAY);
            DrawText("Press ESC to go back", 370, 420, 20, GRAY);

            EndDrawing();
            continue;
        }

        if (currentScreen == SCOREBOARD)
        {
            if (musicLoaded && !musicStopped)
            {
                if (!IsMusicStreamPlaying(bgMusic))
                {
                    PlayMusicStream(bgMusic);
                }
                UpdateMusicStream(bgMusic);
            }

            if (IsKeyPressed(KEY_BACKSPACE))
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
                    DrawText(TextFormat("%d. %s - %d", i + 1, topNames[i].c_str(), topScores[i]), 310, 170 + i * 45, 28, LIGHTGRAY);
                }
            }

            DrawText("Press BACKSPACE for Menu", 330, 600, 20, GRAY);

            EndDrawing();
            continue;
        }

        if (musicLoaded && !musicStopped)
        {
            UpdateMusicStream(bgMusic);
        }

        if (gameOver && !scoreSaved)
        {
            SaveScoreToFile(playerName, score);
            scoreSaved = true;
        }

        if (gameOver && IsKeyPressed(KEY_R))
        {
            if (gameOverSoundLoaded)
            {
                StopSound(gameOverSound);
            }

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
                if (!IsMusicStreamPlaying(bgMusic))
                {
                    PlayMusicStream(bgMusic);
                }
            }
        }

        if (IsKeyPressed(KEY_M))
        {
            if (gameOverSoundLoaded)
            {
                StopSound(gameOverSound);
            }

            currentScreen = MENU;
            gameOver = false;
            deathSoundPlayed = false;
            musicStopped = false;
            scoreSaved = false;

            if (musicLoaded)
            {
                if (!IsMusicStreamPlaying(bgMusic))
                {
                    PlayMusicStream(bgMusic);
                }
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
        ClearBackground(modernLook ? Color{ 12, 12, 18, 255 } : BLACK);

        BeginMode3D(camera);

        DrawBoard(boardWidth, boardHeight, modernLook);
        snake.Draw(boardWidth, boardHeight, modernLook);
        food.Draw(boardWidth, boardHeight, modernLook);

        if (modernLook)
        {
            DrawCube({ -12.0f, 2.0f, 0.0f }, 0.5f, 4.0f, 0.5f, Color{ 50, 70, 120, 255 });
            DrawCube({ 12.0f, 2.0f, 0.0f }, 0.5f, 4.0f, 0.5f, Color{ 50, 70, 120, 255 });
            DrawCube({ 0.0f, 2.0f, -12.0f }, 0.5f, 4.0f, 0.5f, Color{ 120, 60, 100, 255 });
            DrawCube({ 0.0f, 2.0f,  12.0f }, 0.5f, 4.0f, 0.5f, Color{ 120, 60, 100, 255 });
        }

        EndMode3D();

        DrawText(TextFormat("Player: %s", playerName.c_str()), 20, 20, 20, WHITE);
        DrawText(TextFormat("Score: %d", score), 20, 50, 20, WHITE);
        DrawText(modernLook ? "Style: Modern" : "Style: Plain", 20, 80, 20, LIGHTGRAY);
        DrawText(orientation == DIAGONAL_VIEW ? "Orientation: Diagonal" : "Orientation: Top", 20, 110, 20, LIGHTGRAY);
        DrawText("Press M for Menu", 20, 140, 20, GRAY);

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