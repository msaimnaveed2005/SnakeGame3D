#include "Snake.h"

Snake::Snake()
{
    Reset();
}

void Snake::Reset()
{
    body.clear();
    body.push_back({ 10.0f, 10.0f });
    direction = 0;
}

void Snake::SetDirection(int newDirection)
{
    if (newDirection == 4 && direction != 3) direction = 4;
    if (newDirection == 3 && direction != 4) direction = 3;
    if (newDirection == 1 && direction != 2) direction = 1;
    if (newDirection == 2 && direction != 1) direction = 2;
}

void Snake::Move()
{
    for (int i = (int)body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }

    if (direction == 4) body[0].x += 1;
    if (direction == 3) body[0].x -= 1;
    if (direction == 1) body[0].z -= 1;
    if (direction == 2) body[0].z += 1;
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

const vector<Snake::Segment>& Snake::GetBody() const
{
    return body;
}

bool Snake::CheckWallCollision(int width, int height) const
{
    return (body[0].x < 0 || body[0].x >= width || body[0].z < 0 || body[0].z >= height);
}

bool Snake::CheckSelfCollision() const
{
    for (int i = 1; i < (int)body.size(); i++)
    {
        if (body[0].x == body[i].x && body[0].z == body[i].z)
            return true;
    }
    return false;
}

void Snake::Draw(int boardWidth, int boardHeight, bool premiumMode) const
{
    for (int i = 0; i < (int)body.size(); i++)
    {
        float worldX = body[i].x - boardWidth / 2.0f + 0.5f;
        float worldZ = body[i].z - boardHeight / 2.0f + 0.5f;

        if (premiumMode)
        {
            DrawCube({ worldX, 0.05f, worldZ }, 1.0f, 0.1f, 1.0f, Color{ 0, 0, 0, 90 });

            if (i == 0)
            {
                DrawCube({ worldX, 0.65f, worldZ }, 1.0f, 1.1f, 1.0f, Color{ 0, 230, 130, 255 });
                DrawCubeWires({ worldX, 0.65f, worldZ }, 1.02f, 1.12f, 1.02f, Color{ 180, 255, 220, 255 });
            }
            else
            {
                int fade = 180 - i * 5;
                if (fade < 80) fade = 80;

                DrawCube({ worldX, 0.58f, worldZ }, 0.9f, 0.95f, 0.9f, Color{ 0, (unsigned char)fade, 110, 255 });
            }
        }
        else
        {
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
}