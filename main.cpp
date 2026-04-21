#include "raylib.h"
#include <string>
#include "Snake.h"
using namespace std;

int main()
{
    InitWindow(1000, 700, "3D Snake OOP");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 12.0f, 18.0f, 24.0f };
    camera.target = {10.0f, 0.0f, 10.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Snake snake;

    float moveTimer = 0.0f;
    float moveDelay = 0.2f;

    while (!WindowShouldClose())
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
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawGrid(20, 1.0f);
        DrawCube({ snake.GetX(), 0.5f, snake.GetZ() }, 1.0f, 1.0f, 1.0f, GREEN);

        EndMode3D();

        DrawText("Snake class created", 20, 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}