#include "Food.h"

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

void Food::Draw(int boardWidth, int boardHeight) const
{
    float worldX = x - boardWidth / 2.0f + 0.5f;
    float worldZ = z - boardHeight / 2.0f + 0.5f;

    DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, RED);
}