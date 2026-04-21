#include "Food.h"
#include <cstdlib>
#include <cmath>

Food::Food()
{
    x = 5.0f;
    z = 5.0f;
    animTime = 0.0f;
}

float Food::GetX() const
{
    return x;
}

float Food::GetZ() const
{
    return z;
}

void Food::SetPosition(float newX, float newZ)
{
    x = newX;
    z = newZ;
}

void Food::Respawn(int boardWidth, int boardHeight, const vector<Snake::Segment>& snakeBody)
{
    bool valid = false;

    while (!valid)
    {
        valid = true;

        x = rand() % boardWidth;
        z = rand() % boardHeight;

        for (int i = 0; i < (int)snakeBody.size(); i++)
        {
            if (snakeBody[i].x == x && snakeBody[i].z == z)
            {
                valid = false;
                break;
            }
        }
    }
}

void Food::Reset(int boardWidth, int boardHeight, const vector<Snake::Segment>& snakeBody)
{
    Respawn(boardWidth, boardHeight, snakeBody);
    animTime = 0.0f;
}

void Food::Update()
{
    animTime += GetFrameTime();
}

void Food::Draw(int boardWidth, int boardHeight, bool premiumMode) const
{
    float worldX = x - boardWidth / 2.0f + 0.5f;
    float worldZ = z - boardHeight / 2.0f + 0.5f;

    if (premiumMode)
    {
        float floatOffset = sin(animTime * 3.0f) * 0.22f;
        float glowSize = 1.28f + sin(animTime * 4.0f) * 0.13f;
        float ringOffset = 0.82f + floatOffset;

        DrawCube({ worldX, 0.05f, worldZ }, 1.0f, 0.1f, 1.0f, Color{ 0, 0, 0, 95 });
        DrawCube({ worldX, ringOffset, worldZ }, glowSize, glowSize, glowSize, Color{ 255, 70, 70, 45 });
        DrawCube({ worldX, ringOffset, worldZ }, 0.84f, 0.84f, 0.84f, RED);
        DrawCubeWires({ worldX, ringOffset, worldZ }, 0.90f, 0.90f, 0.90f, Color{ 255, 220, 220, 255 });
        DrawCube({ worldX, 1.18f + floatOffset, worldZ }, 0.25f, 0.08f, 0.25f, Color{ 255, 250, 180, 170 });
    }
    else
    {
        DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, RED);
    }
}