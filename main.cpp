#include "raylib.h"
#include <string>
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
    float moveDelay = 0.2f;

    bool gameOver = false;

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            if (IsKeyPressed(KEY_RIGHT))
            {
                snake.SetDirection(4);
            }

            if (IsKeyPressed(KEY_LEFT))
            {
                snake.SetDirection(3);
            }

            if (IsKeyPressed(KEY_UP))
            {
                snake.SetDirection(1);
            }

            if (IsKeyPressed(KEY_DOWN))
            {
                snake.SetDirection(2);
            }

            moveTimer = moveTimer + GetFrameTime();

            if (moveTimer >= moveDelay)
            {
                snake.Move();
                moveTimer = 0.0f;

                if (snake.CheckWallCollision(boardWidth, boardHeight))
                {
                    gameOver = true;
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

        DrawText("Food object added", 20, 20, 20, WHITE);

        if (gameOver)
        {
            DrawText("GAME OVER", 380, 320, 40, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}