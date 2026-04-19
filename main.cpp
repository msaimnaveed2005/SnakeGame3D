#include "raylib.h"
#include <string>
using namespace std;

int main()
{
    InitWindow(1000, 700, "3D Snake OOP");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 12.0f, 18.0f, 24.0f };
    camera.target = { 10.0f, 0.0f, 10.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float cubeX = 10.0f;
    float cubeZ = 10.0f;

    int direction = 0; // 0 = stop, 1 = up, 2 = down, 3 = left, 4 = right

    float moveTimer = 0.0f;
    float moveDelay = 0.2f;

    while (!WindowShouldClose())
    {
        // INPUT
        if (IsKeyPressed(KEY_RIGHT) && direction != 3)
        {
            direction = 4;
        }

        if (IsKeyPressed(KEY_LEFT) && direction != 4)
        {
            direction = 3;
        }

        if (IsKeyPressed(KEY_UP) && direction != 2)
        {
            direction = 1;
        }

        if (IsKeyPressed(KEY_DOWN) && direction != 1)
        {
            direction = 2;
        }

        // MOVEMENT TIMER
        moveTimer = moveTimer + GetFrameTime();

        if (moveTimer >= moveDelay)
        {
            if (direction == 4) cubeX = cubeX + 1.0f;
            if (direction == 3) cubeX = cubeX - 1.0f;
            if (direction == 1) cubeZ = cubeZ - 1.0f;
            if (direction == 2) cubeZ = cubeZ + 1.0f;

            moveTimer = 0.0f;
        }

        // DRAWING
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawGrid(20, 1.0f);
        DrawCube({ cubeX, 0.5f, cubeZ }, 1.0f, 1.0f, 1.0f, GREEN);

        EndMode3D();

        DrawText("Snake movement active", 20, 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}