#include "raylib.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
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

struct ScoreEntry
{
    string name;
    int score;
    string date;
};

string GetCurrentDate()
{
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    return string(buffer);
}

void SaveScoreToFile(const string& playerName, int score)
{
    ofstream file("scores.txt", ios::app);
    if (file.is_open())
    {
        file << playerName << "|" << score << "|" << GetCurrentDate() << endl;
        file.close();
    }
}

void LoadTopScores(ScoreEntry entries[], int& count, int& highestScore)
{
    count = 0;
    highestScore = 0;

    ifstream file("scores.txt");
    if (!file.is_open())
    {
        return;
    }

    string line;
    while (count < 20 && getline(file, line))
    {
        size_t firstSep = line.find('|');
        size_t secondSep = line.find('|', firstSep + 1);

        if (firstSep == string::npos || secondSep == string::npos)
        {
            continue;
        }

        entries[count].name = line.substr(0, firstSep);
        entries[count].score = stoi(line.substr(firstSep + 1, secondSep - firstSep - 1));
        entries[count].date = line.substr(secondSep + 1);

        if (entries[count].score > highestScore)
        {
            highestScore = entries[count].score;
        }

        count++;
    }

    file.close();

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (entries[j].score > entries[i].score)
            {
                ScoreEntry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }
}

void DrawBoard(int boardWidth, int boardHeight, bool modernLook)
{
    DrawPlane({ 0.0f, 0.0f, 0.0f }, { (float)boardWidth, (float)boardHeight }, modernLook ? Color{ 36, 36, 44, 255 } : DARKGRAY);

    float startX = -boardWidth / 2.0f;
    float startZ = -boardHeight / 2.0f;

    for (int i = 0; i <= boardWidth; i++)
    {
        float x = startX + i;
        DrawLine3D({ x, 0.01f, startZ }, { x, 0.01f, startZ + boardHeight }, modernLook ? Color{ 90, 90, 105, 255 } : GRAY);
    }

    for (int i = 0; i <= boardHeight; i++)
    {
        float z = startZ + i;
        DrawLine3D({ startX, 0.01f, z }, { startX + boardWidth, 0.01f, z }, modernLook ? Color{ 90, 90, 105, 255 } : GRAY);
    }

    if (modernLook)
    {
        DrawCubeWires({ 0.0f, 0.6f, 0.0f }, (float)boardWidth, 1.2f, (float)boardHeight, Color{ 170, 170, 190, 255 });

        float halfW = boardWidth / 2.0f;
        float halfH = boardHeight / 2.0f;

        DrawCube({ -halfW - 0.7f, 1.5f, -halfH - 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ halfW + 0.7f, 1.5f, -halfH - 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ -halfW - 0.7f, 1.5f,  halfH + 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });
        DrawCube({ halfW + 0.7f, 1.5f,  halfH + 0.7f }, 0.5f, 3.0f, 0.5f, Color{ 70, 70, 90, 255 });

        DrawCube({ 0.0f, -0.35f, 0.0f }, (float)boardWidth + 1.0f, 0.2f, (float)boardHeight + 1.0f, Color{ 20, 20, 28, 255 });

        DrawCube({ -12.5f, 1.0f,  0.0f }, 0.25f, 2.0f, 24.0f, Color{ 40, 80, 160, 120 });
        DrawCube({ 12.5f, 1.0f,  0.0f }, 0.25f, 2.0f, 24.0f, Color{ 40, 80, 160, 120 });
        DrawCube({ 0.0f, 1.0f, -12.5f }, 24.0f, 2.0f, 0.25f, Color{ 180, 70, 130, 100 });
        DrawCube({ 0.0f, 1.0f,  12.5f }, 24.0f, 2.0f, 0.25f, Color{ 180, 70, 130, 100 });
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

void DrawMenuItem(const string& text, int x, int y, bool selected)
{
    if (selected)
    {
        DrawRectangleRounded({ (float)(x - 20), (float)(y - 8), 360.0f, 42.0f }, 0.2f, 8, Color{ 255, 255, 255, 25 });
        DrawRectangleLinesEx({ (float)(x - 20), (float)(y - 8), 360.0f, 42.0f }, 2.0f, YELLOW);
    }

    DrawText(text.c_str(), x, y, 30, selected ? YELLOW : WHITE);
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
    Sound winSound = LoadSound("win.mp3");
    Music bgMusic = LoadMusicStream("bgmusic.mp3");

    SetSoundVolume(eatSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
    SetSoundVolume(winSound, 1.0f);
    SetMusicVolume(bgMusic, 0.4f);

    bool eatSoundLoaded = IsSoundValid(eatSound);
    bool gameOverSoundLoaded = IsSoundValid(gameOverSound);
    bool winSoundLoaded = IsSoundValid(winSound);
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
    int highestScore = 0;
    int recordToBeat = 0;
    bool newRecordAchievedThisRun = false;

    float newRecordEffectTimer = 0.0f;

    GameScreen currentScreen = MENU;
    int selectedOption = 0;
    bool modernLook = false;

    string playerName = "";
    string tempName = "";
    bool nameInputStartsGame = false;

    ScoreEntry topEntries[20];
    int scoreCount = 0;
    LoadTopScores(topEntries, scoreCount, highestScore);
    recordToBeat = highestScore;

    bool shouldExit = false;

    while (!WindowShouldClose() && !shouldExit)
    {
        if (newRecordEffectTimer > 0.0f)
        {
            newRecordEffectTimer -= GetFrameTime();
            if (newRecordEffectTimer < 0.0f)
            {
                newRecordEffectTimer = 0.0f;
            }
        }

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
                if (selectedOption < 0) selectedOption = 5;
            }

            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                selectedOption++;
                if (selectedOption > 5) selectedOption = 0;
            }

            if (selectedOption == 1 && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_ENTER)))
            {
                modernLook = !modernLook;
            }

            if (selectedOption == 2 && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_ENTER)))
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
                        nameInputStartsGame = true;
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
                        newRecordEffectTimer = 0.0f;
                        newRecordAchievedThisRun = false;

                        LoadTopScores(topEntries, scoreCount, highestScore);
                        recordToBeat = highestScore;

                        ApplyCameraOrientation(camera, orientation);
                        currentScreen = PLAYING;
                    }
                }
                else if (selectedOption == 3)
                {
                    LoadTopScores(topEntries, scoreCount, highestScore);
                    currentScreen = SCOREBOARD;
                }
                else if (selectedOption == 4)
                {
                    tempName = playerName;
                    nameInputStartsGame = false;
                    currentScreen = NAME_INPUT;
                }
                else if (selectedOption == 5)
                {
                    shouldExit = true;
                }
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("3D SNAKE", 390, 90, 42, WHITE);
            DrawText("Menu", 460, 150, 25, LIGHTGRAY);

            DrawMenuItem("Play Game", 340, 215, selectedOption == 0);
            DrawMenuItem(TextFormat("Visual Style: %s", modernLook ? "Modern" : "Plain"), 340, 270, selectedOption == 1);
            DrawMenuItem(TextFormat("Orientation: %s", orientation == DIAGONAL_VIEW ? "Diagonal" : "Top"), 340, 325, selectedOption == 2);
            DrawMenuItem("Scoreboard", 340, 380, selectedOption == 3);
            DrawMenuItem("Change Name", 340, 435, selectedOption == 4);
            DrawMenuItem("Exit", 340, 490, selectedOption == 5);

            if (!playerName.empty())
            {
                DrawText(TextFormat("Player: %s", playerName.c_str()), 380, 560, 24, LIGHTGRAY);
            }

            if (!winSoundLoaded)
            {
                DrawText("win.wav not loaded", 20, 20, 20, RED);
            }

            DrawText("Use UP / DOWN to move", 350, 610, 18, GRAY);
            DrawText("LEFT / RIGHT / ENTER changes settings", 295, 635, 18, GRAY);

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

                if (nameInputStartsGame)
                {
                    snake.Reset();
                    food.Reset(boardWidth, boardHeight, snake.GetBody());
                    score = 0;
                    gameOver = false;
                    deathSoundPlayed = false;
                    musicStopped = false;
                    scoreSaved = false;
                    moveDelay = baseSpeed;
                    newRecordEffectTimer = 0.0f;
                    newRecordAchievedThisRun = false;

                    LoadTopScores(topEntries, scoreCount, highestScore);
                    recordToBeat = highestScore;

                    ApplyCameraOrientation(camera, orientation);
                    currentScreen = PLAYING;
                }
                else
                {
                    currentScreen = MENU;
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentScreen = MENU;
            }

            BeginDrawing();
            ClearBackground(Color{ 15, 15, 20, 255 });

            DrawText("ENTER YOUR NAME", 315, 180, 35, WHITE);
            DrawRectangleLinesEx({ 250, 280, 500, 60 }, 2.0f, LIGHTGRAY);
            DrawText(tempName.c_str(), 270, 295, 30, YELLOW);

            DrawText("Press ENTER to confirm", 345, 390, 20, GRAY);
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

            DrawText("SCOREBOARD", 360, 70, 40, WHITE);

            if (scoreCount == 0)
            {
                DrawText("No scores yet", 410, 180, 25, GRAY);
            }
            else
            {
                int displayCount = (scoreCount > 5) ? 5 : scoreCount;

                DrawText("Rank", 170, 150, 24, LIGHTGRAY);
                DrawText("Name", 280, 150, 24, LIGHTGRAY);
                DrawText("Score", 520, 150, 24, LIGHTGRAY);
                DrawText("Date", 680, 150, 24, LIGHTGRAY);

                for (int i = 0; i < displayCount; i++)
                {
                    int y = 200 + i * 55;

                    DrawText(TextFormat("%d", i + 1), 190, y, 24, WHITE);
                    DrawText(topEntries[i].name.c_str(), 280, y, 24, WHITE);
                    DrawText(TextFormat("%d", topEntries[i].score), 540, y, 24, WHITE);
                    DrawText(topEntries[i].date.c_str(), 670, y, 24, WHITE);
                }
            }

            DrawText("Press BACKSPACE for Menu", 330, 620, 20, GRAY);

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
            LoadTopScores(topEntries, scoreCount, highestScore);
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
            newRecordEffectTimer = 0.0f;
            newRecordAchievedThisRun = false;

            LoadTopScores(topEntries, scoreCount, highestScore);
            recordToBeat = highestScore;

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
            newRecordEffectTimer = 0.0f;
            newRecordAchievedThisRun = false;

            LoadTopScores(topEntries, scoreCount, highestScore);
            recordToBeat = highestScore;

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

                    if (!newRecordAchievedThisRun && score > recordToBeat)
                    {
                        newRecordAchievedThisRun = true;
                        highestScore = score;
                        newRecordEffectTimer = 2.5f;

                        if (winSoundLoaded)
                        {
                            StopSound(winSound);
                            PlaySound(winSound);
                        }
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
            float t = (float)GetTime();

            DrawSphere({ -13.0f, 3.0f + sin(t * 1.2f) * 0.3f, -13.0f }, 0.25f, Color{ 80, 140, 255, 120 });
            DrawSphere({ 13.0f, 3.0f + sin(t * 1.0f) * 0.3f, -13.0f }, 0.25f, Color{ 80, 255, 180, 120 });
            DrawSphere({ -13.0f, 3.0f + sin(t * 1.4f) * 0.3f,  13.0f }, 0.25f, Color{ 255, 120, 180, 120 });
            DrawSphere({ 13.0f, 3.0f + sin(t * 1.1f) * 0.3f,  13.0f }, 0.25f, Color{ 255, 220, 120, 120 });
        }

        EndMode3D();

        DrawText(TextFormat("Player: %s", playerName.c_str()), 20, 20, 20, WHITE);
        DrawText(TextFormat("Score: %d", score), 20, 50, 20, WHITE);
        DrawText(TextFormat("Best: %d", highestScore), 20, 80, 20, WHITE);
        DrawText(modernLook ? "Style: Modern" : "Style: Plain", 20, 110, 20, LIGHTGRAY);
        DrawText(orientation == DIAGONAL_VIEW ? "Orientation: Diagonal" : "Orientation: Top", 20, 140, 20, LIGHTGRAY);
        DrawText("Press M for Menu", 20, 170, 20, GRAY);

        if (!winSoundLoaded)
        {
            DrawText("win.wav not loaded", 20, 200, 20, RED);
        }

        if (newRecordEffectTimer > 0.0f)
        {
            unsigned char alpha = (unsigned char)(140 + 80 * sin(GetTime() * 8.0));
            DrawRectangle(250, 210, 500, 80, Color{ 255, 215, 0, 35 });
            DrawRectangleLinesEx({ 250, 210, 500, 80 }, 3.0f, Color{ 255, 215, 0, alpha });
            DrawText("NEW HIGH SCORE!", 325, 232, 36, GOLD);
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
    if (winSoundLoaded) UnloadSound(winSound);
    if (musicLoaded) UnloadMusicStream(bgMusic);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}