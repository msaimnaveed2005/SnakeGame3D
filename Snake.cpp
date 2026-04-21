#include "Snake.h"

Snake::Snake()
{
    x = 10.0f;
    z = 10.0f;
    direction = 0;
}

void Snake::SetDirection(int newDirection)
{
    if (newDirection == 4 && direction != 3)
    {
        direction = 4;
    }

    if (newDirection == 3 && direction != 4)
    {
        direction = 3;
    }

    if (newDirection == 1 && direction != 2)
    {
        direction = 1;
    }

    if (newDirection == 2 && direction != 1)
    {
        direction = 2;
    }
}

void Snake::Move()
{
    if (direction == 4) x = x + 1.0f;
    if (direction == 3) x = x - 1.0f;
    if (direction == 1) z = z - 1.0f;
    if (direction == 2) z = z + 1.0f;
}

float Snake::GetX() const
{
    return x;
}

float Snake::GetZ() const
{
    return z;
}

bool Snake::CheckWallCollision(int width, int height) const
{
    if (x < 0 || x >= width || z < 0 || z >= height)
    {
        return true;
    }

    return false;
}

void Snake::Draw(int boardWidth, int boardHeight) const
{
    float worldX = x - boardWidth / 2.0f + 0.5f;
    float worldZ = z - boardHeight / 2.0f + 0.5f;

    DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, GREEN);
}