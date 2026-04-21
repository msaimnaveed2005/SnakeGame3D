#include "Snake.h"

Snake::Snake()
{
    body.push_back({ 10.0f, 10.0f });
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
    for (int i = body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }

    if (direction == 4) body[0].x = body[0].x + 1.0f;
    if (direction == 3) body[0].x = body[0].x - 1.0f;
    if (direction == 1) body[0].z = body[0].z - 1.0f;
    if (direction == 2) body[0].z = body[0].z + 1.0f;
}

void Snake::Grow()
{
    body.push_back(body.back());
}

float Snake::GetX() const
{
    return body[0].x;
}

float Snake::GetZ() const
{
    return body[0].z;
}

bool Snake::CheckWallCollision(int width, int height) const
{
    if (body[0].x < 0 || body[0].x >= width || body[0].z < 0 || body[0].z >= height)
    {
        return true;
    }

    return false;
}

bool Snake::CheckSelfCollision() const
{
    for (int i = 1; i < body.size(); i++)
    {
        if (body[0].x == body[i].x && body[0].z == body[i].z)
        {
            return true;
        }
    }

    return false;
}

void Snake::Draw(int boardWidth, int boardHeight) const
{
    for (int i = 0; i < body.size(); i++)
    {
        float worldX = body[i].x - boardWidth / 2.0f + 0.5f;
        float worldZ = body[i].z - boardHeight / 2.0f + 0.5f;

        if (i == 0)
        {
            DrawCube({ worldX, 0.5f, worldZ }, 1.0f, 1.0f, 1.0f, GREEN);
        }
        else
        {
            DrawCube({ worldX, 0.5f, worldZ }, 0.9f, 0.9f, 0.9f, LIME);
        }
    }
}