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
    SetTargetFPS(60);

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
    int score = 0;

    while (!WindowShouldClose())
    {
        // Restart
        if (gameOver && IsKeyPressed(KEY_R))
        {
            snake.Reset();
            food.Reset(boardWidth, boardHeight, snake.GetBody());
            score = 0;
            gameOver = false;
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

                if (snake.CheckWallCollision(boardWidth, boardHeight)) gameOver = true;
                if (snake.CheckSelfCollision()) gameOver = true;

                if (snake.GetX() == food.GetX() && snake.GetZ() == food.GetZ())
                {
                    score += 10;
                    snake.Grow();
                    food.Respawn(boardWidth, boardHeight, snake.GetBody());

                    // Increase speed
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

        if (gameOver)
        {
            DrawText("GAME OVER", 380, 300, 40, RED);
            DrawText("Press R to Restart", 350, 350, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}