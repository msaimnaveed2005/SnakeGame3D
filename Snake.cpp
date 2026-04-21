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

void Snake::Draw() const
{
    DrawCube({ x, 0.5f, z }, 1.0f, 1.0f, 1.0f, GREEN);
}