#include "Food.h"
#include <cstdlib>

Food::Food()
{
    x = 5.0f;
    z = 5.0f;
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

void Food::Respawn(int boardWidth, int boardHeight)
{
    x = rand() % boardWidth;
    z = rand() % boardHeight;
}

void Food::Draw(int boardWidth, int boardHeight) const
{
    float worldX = x - boardWidth / 2.0f + 0.5f;
    float worldZ = z - boardHeight / 2.0f + 0.5f;

    DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, RED);
}